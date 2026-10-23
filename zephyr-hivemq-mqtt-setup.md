# Connecting a Zephyr RTOS MCU to HiveMQ Cloud (MQTT over TLS)

## Broker details

- Host: `e45fd90d47864dda9e1a795374df5566.s1.eu.hivemq.cloud`
- Port: `8883` (MQTT over TLS — this broker does not accept unencrypted connections)
- Plan: HiveMQ Cloud Serverless
- Auth: username + password credential created in HiveMQ Access Management (Connect → broker → Configure → Access Management)

## 1. Kconfig (prj.conf)

```
CONFIG_NETWORKING=y
CONFIG_NET_SOCKETS=y
CONFIG_NET_SOCKETS_SOCKOPT_TLS=y
CONFIG_MQTT_LIB=y
CONFIG_MQTT_LIB_TLS=y
CONFIG_MBEDTLS=y
CONFIG_MBEDTLS_ENABLE_HEAP=y
CONFIG_MBEDTLS_HEAP_SIZE=32768
CONFIG_NET_SOCKETS_TLS_MAX_CREDENTIALS=4
CONFIG_TLS_CREDENTIALS=y
```

Add whatever the board's actual network transport needs on top of this (Wi-Fi driver Kconfig, Ethernet, etc.) — this list only covers the MQTT/TLS stack.

## 2. Provision the CA certificate

HiveMQ Cloud's TLS cert chains to a public CA. Fetch the chain once:

```sh
openssl s_client -showcerts -connect e45fd90d47864dda9e1a795374df5566.s1.eu.hivemq.cloud:8883 </dev/null
```

Take the root/issuer PEM from that output and embed it as a Zephyr TLS credential at boot:

```c
#include <zephyr/net/tls_credentials.h>

#define CA_CERTIFICATE_TAG 1

static const unsigned char ca_certificate[] = {
    #include "hivemq_ca.pem.inc"  /* PEM bytes converted to a C array */
};

tls_credential_add(CA_CERTIFICATE_TAG, TLS_CREDENTIAL_CA_CERTIFICATE,
                    ca_certificate, sizeof(ca_certificate));
```

## 3. MQTT client setup

```c
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>

#define MQTT_BROKER_HOST "e45fd90d47864dda9e1a795374df5566.s1.eu.hivemq.cloud"
#define MQTT_BROKER_PORT 8883

static struct mqtt_client client;
static struct sockaddr_storage broker;
static uint8_t rx_buf[512], tx_buf[512];

static sec_tag_t sec_tag_list[] = { CA_CERTIFICATE_TAG };

void mqtt_setup(void)
{
    struct sockaddr_in *b = (struct sockaddr_in *)&broker;
    b->sin_family = AF_INET;
    b->sin_port = htons(MQTT_BROKER_PORT);
    /* Resolve MQTT_BROKER_HOST via getaddrinfo() first, then: */
    zsock_inet_pton(AF_INET, resolved_ip, &b->sin_addr);

    mqtt_client_init(&client);
    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = (uint8_t *)"stm32-node1";
    client.client_id.size = strlen("stm32-node1");
    client.password = &(struct mqtt_utf8){ .utf8 = (uint8_t *)"YOUR_PASSWORD", .size = strlen("YOUR_PASSWORD") };
    client.user_name = &(struct mqtt_utf8){ .utf8 = (uint8_t *)"YOUR_USERNAME", .size = strlen("YOUR_USERNAME") };
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.rx_buf = rx_buf; client.rx_buf_size = sizeof(rx_buf);
    client.tx_buf = tx_buf; client.tx_buf_size = sizeof(tx_buf);

    client.transport.type = MQTT_TRANSPORT_SECURE;
    struct mqtt_sec_config *tls_config = &client.transport.tls.config;
    tls_config->peer_verify = TLS_PEER_VERIFY_REQUIRED;
    tls_config->cipher_list = NULL;
    tls_config->sec_tag_list = sec_tag_list;
    tls_config->sec_tag_count = ARRAY_SIZE(sec_tag_list);
    tls_config->hostname = MQTT_BROKER_HOST; /* required for SNI + cert CN/SAN check */

    mqtt_connect(&client);
}
```

Pump the connection in your main loop or a dedicated thread: poll the socket fd with `zsock_poll()`, call `mqtt_input()` when data is ready (non-blocking), and call `mqtt_live()` periodically to send keepalive pings.

## 4. Publish a message

```c
struct mqtt_publish_param param = {
    .message.topic.topic.utf8 = (uint8_t *)"home/node1/temp",
    .message.topic.topic.size = strlen("home/node1/temp"),
    .message.topic.qos = MQTT_QOS_1_AT_LEAST_ONCE,
    .message.payload.data = payload,
    .message.payload.len = payload_len,
    .message_id = sys_rand32_get(),
};
mqtt_publish(&client, &param);
```

## Gotchas

- **mbedTLS heap size** (`CONFIG_MBEDTLS_HEAP_SIZE`): 32KB is a safe starting point for the TLS handshake; tune down later if RAM-constrained.
- **DNS resolution is not automatic** — resolve `MQTT_BROKER_HOST` to an IP via `getaddrinfo()` before filling in the `broker` sockaddr.
- **`tls_config->hostname` must be set** or the cert's CN/SAN check will fail even with a correct CA credential loaded.
- No built-in reconnect/backoff — wrap `mqtt_connect()` in retry logic with backoff if the link is expected to drop (Wi-Fi roaming, cellular, etc.).

## Quick test from a Linux host (before wiring up the MCU)

```sh
mosquitto_sub -h e45fd90d47864dda9e1a795374df5566.s1.eu.hivemq.cloud \
  -p 8883 --capath /etc/ssl/certs \
  -u YOUR_USERNAME -P YOUR_PASSWORD \
  -t test/topic -d

mosquitto_pub -h e45fd90d47864dda9e1a795374df5566.s1.eu.hivemq.cloud \
  -p 8883 --capath /etc/ssl/certs \
  -u YOUR_USERNAME -P YOUR_PASSWORD \
  -t test/topic -m '{"temp": 22.5}'
```
