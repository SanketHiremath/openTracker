# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

# Devicetree bindings for out-of-tree compatibles (quectel,eg916q) live
# under dts/bindings/ in this module. Must be set before find_package.
list(APPEND DTS_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(car_tracker CXX)

target_sources(app PRIVATE
  src/main.cpp
  src/channels.cpp
  src/tasks/ble_task.cpp
  src/tasks/gps_task.cpp
  src/tasks/supervisor_task.cpp
  src/services/selftest.cpp
  src/services/fw_update.cpp
)

# fw_update.cpp calls bootutil's boot_set_next() to mark the inactive A/B slot
# pending; the library target also carries the bootutil/ include path.
if(CONFIG_MCUBOOT_BOOTUTIL_LIB)
  target_link_libraries(app PRIVATE MCUBOOT_BOOTUTIL)
endif()

# Out-of-tree EG916Q-GL modem driver. Compiled straight into the app target
# (rather than as a separate zephyr_library()) -- a plain add_subdirectory()
# from the app's own CMakeLists.txt does not reliably register a library
# into Zephyr's global whole-archive link list, so DEVICE_DT_INST_DEFINE's
# device struct silently never reached the final image. target_sources(app)
# is guaranteed to link, same as every other conditional source below.
if(CONFIG_MODEM_QUECTEL_EG916Q)
  target_sources(app PRIVATE drivers/modem/quectel_eg916q.c)
endif()

# Out-of-tree L3GD20 gyroscope driver, same compile-into-app pattern and for
# the same reason as the modem driver above. CONFIG_L3GD20 is only ever set
# when a devicetree node with compatible "st,l3gd20" exists AND CONFIG_SENSOR
# is on (see drivers/sensor/l3gd20/Kconfig). The GPS-only build leaves
# CONFIG_SENSOR off, so it never gets this driver.
if(CONFIG_L3GD20)
  target_sources(app PRIVATE drivers/sensor/l3gd20/l3gd20.c)
endif()

# IMU task (bench probe); needs the sensor drivers, so only with CONFIG_SENSOR.
if(CONFIG_SENSOR)
  target_sources(app PRIVATE src/tasks/imu_task.cpp src/services/spectrum.cpp)
endif()

# Idle sleep (System OFF, wake on the accelerometer interrupt).
if(CONFIG_CAR_TRACKER_IDLE_SLEEP)
  target_sources(app PRIVATE src/tasks/power_task.cpp)
endif()

# Cellular telemetry is compiled out entirely for the GPS-only bring-up build.
if(NOT CONFIG_CAR_TRACKER_GPS_ONLY)
  target_sources(app PRIVATE
    src/tasks/cellular_task.cpp
    src/services/modem_link.cpp
  )
endif()
