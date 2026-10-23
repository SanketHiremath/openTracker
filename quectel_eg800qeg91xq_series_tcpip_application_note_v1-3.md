


EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                            0 / 56




EG800Q&EG91xQ Series
TCP/IP Application Note
LTE Standard Products
## Version: 1.3
## Date: 2026-04-30
## Status: Released


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                         1 / 56



At  Quectel,  our  aim  is  to  provide  timely  and  comprehensive  services  to  our  customers.  If  you
require any assistance, please contact our headquarters:

## Quectel Wireless Solutions Co., Ltd.
## No. 8 Waipojing Road, Sijing Town, Songjiang District, Shanghai 201601, China
## Tel: +86 21 5108 6236
Email: info@quectel.com

Or our local offices. For more information, please visit:
https://www.quectel.com/contact/.

For technical support, or to report documentation errors, please visit:
https://www.quectel.com/tech-support/.
Or email us at: support@quectel.com.

## Legal Notices
We provide this document to support your product design. You are required to design your products based
on  the  specifications  and  parameters  set  forth  herein. You  agree  that  you  are  responsible  for  using
independent analysis and evaluation in designing intended products, and we provide reference designs
for illustrative purposes only. Before using any hardware, software or service guided by this document,
please read this notice carefully. Even though we employ commercially reasonable efforts to provide the
best  possible  experience,  you  hereby  acknowledge  and  agree  that  this  document  and  related  services
hereunder are provided to you on an “as available” basis. You acknowledge and agree that we may add
to, amend, or restate this document at any time at our sole discretion without any prior notice to you, and
such additions, amendments, or restatements shall be binding upon you.
Use and Disclosure Restrictions
## License Agreements
The  recipient  of  any  hardware,  software,  materials,  or  documentation provided  by  us  shall keep such
content confidential, unless expressly authorized by us. The recipient shall not disclose, access, or use
any  part  of  the  received  content  for  any  purpose  other  than  the  execution  and  implementation  of  the
intended project.
## Copyright
Our  and  third-party  products  hereunder  may  contain copyrighted  materials,  including  but  not  limited  to
protected content, hardware, software, and documentation owned by us or applicable third parties. Unless
prior  written  consent  is  obtained,  you  shall  not  access,  use,  or  disclose  any  documents  or  information
provided by us, nor shall you copy, reproduce, republish, display, translate, distribute, merge, modify, or
create  derivative  works  from  any  such copyrighted  materials. We  and  the applicable third  party retain
exclusive rights to all copyrighted materials. No license to any patents, copyrights, trademarks, or service
marks shall be granted or transferred. For the avoidance of doubt, no form of purchase shall be construed
as  granting  any  license beyond a  normal, non-exclusive,  royalty-free  license  to  use  the  product.  We
reserve  the  right  to  pursue  legal  action  against  any violation  of  confidentiality  obligations,  unauthorized


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                                     2 / 56



use, or any other unlawful or malicious use of the aforementioned documents and information.
## Trademarks
Unless otherwise expressly provided, nothing in this document shall be construed as conferring any rights
to  use  any  trademark,  trade  name, name,  abbreviation,  or counterfeit thereof  owned by  us  or any  third
party in advertising, publicity, or any other contexts.
Third-Party Rights
You understand that this document may refer to hardware, software, and/or documentation owned by one
or more third parties (“third-party materials”). Use of such third-party materials is subject to all applicable
restrictions and obligations set forth herein.
We  make  no  warranty  or  representation,  either  express or  implied,  regarding  the  third-party  materials,
including but not limited to any implied or statutory, warranties of merchantability or fitness for a particular
purpose,  quiet  enjoyment,  system  integration,  information  accuracy,  and  non-infringement  of  any  third-
party  intellectual  property  rights  with  regard  to  the  licensed  technology  or  use  thereof.  Nothing  herein
constitutes a representation or warranty by us to either develop, enhance, modify, distribute, market, sell,
offer for sale, or otherwise maintain production of any our products or any other hardware, software, device,
tool,  information,  or  product.  We  moreover  disclaim  any  and  all  warranties  arising  from  the  course  of
dealing, course of performance, or usage of trade.
## Privacy Policy
To enable product functionality, certain device data may be uploaded to our or third-party servers, including
those  operated  by carriers,  chipset  suppliers, or servers  designated  by  you.  We  strictly comply  with
applicable laws and regulations and will retain, use, disclose, or otherwise process relevant data solely for
the purpose of enabling product functionality, or as permitted by applicable laws. Before interacting with
any third party regarding  data  exchange,  please  be  informed  of and  understand their  privacy  and  data
security policies.
## Disclaimer
a) We shall not be liable for any damages resulting from failure to comply with applicable operational or
design specifications.
b) We shall bear no liability for any inaccuracies or omissions in this document, nor for any damages
arising from the use of the information contained herein.
c) While  we make every  effort  to  ensure the  integrity,  accuracy,  and  timeliness  of  the features and
functions under development, errors or omissions may nevertheless occur. Unless otherwise provided
in a valid written agreement, we make no warranties of any kind, express, implied, or statutory, and
disclaim all  liability  for any  loss  or damage arising  from the  use  of any features or functions  under
development, to the maximum extent permitted by law, regardless of whether such loss or damage is
foreseeable.
d) We assume  no  legal  responsibility for  the  accessibility,  safety,  accuracy,  availability,  legality,  or
completeness  of any information,  content,  advertising,  commercial  offers,  products,  services, or
materials on third-party websites or third-party resources.
Copyright © Quectel Wireless Solutions Co., Ltd. 2026. All rights reserved.


LTE  Standard  Module  Series



EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                            3 / 56



About the Document

## Revision History
## Version  Date Author Description
- 2022-12-06 Orange Li Creation of the document
1.0 2023-02-13 Orange Li First official release
## 1.1 2024-07-30 Orange Li
- Updated applicable modules:
⚫ Added EG916Q-GL;
⚫ Updated EG800Q-EU to EG800Q Series;
⚫ Updated EG915Q-NA to EG915Q Series.
- Added the following Write Commands (Chapter
## 2.3.1):
⚫ AT+QICFG="send/auto"
⚫ AT+QICFG="formatcfg"
⚫ AT+QICFG="close/mode"
⚫ AT+QICFG="tcp/delay_ack"
⚫ AT+QICFG="tcp/twrecycle"
⚫ AT+QICFG="dns/retry"
## 1.2 2024-11-22 Elmo Huang
- Updated  the  declaration  of  AT command
examples (Chapter 2.2).
- Updated URLs in the examples (Chapter 3).
## 1.3 2026-04-30 Elmo Huang
Added  note  on  default  bearer  auto-activation  and
APN reconfiguration steps (Chapter 1.1, Figure 1).


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                                     4 / 56



## Contents
About the Document .................................................................................................................................. 3
Contents ...................................................................................................................................................... 4
Table Index .................................................................................................................................................. 6
1 Introduction ......................................................................................................................................... 7
1.1. Using TCP/IP AT Commands ..................................................................................................... 7
1.2. Description of Data Access Modes ............................................................................................ 9
2 Description of TCP/IP AT Commands ............................................................................................. 11
2.1. AT Command Introduction ........................................................................................................11
2.1.1. Definitions .......................................................................................................................11
2.1.2. AT Command Syntax .....................................................................................................11
2.2. Declaration of AT Command Examples ................................................................................... 12
2.3. Description of TCP/IP AT Commands ...................................................................................... 12
2.3.1. AT+QICFG    Configure Optional Parameters ............................................................... 12
2.3.2. AT+QICSGP    Configure Parameters of a TCP/IP Context ......................................... 21
2.3.3. AT+QIACT    Activate a PDP Context ........................................................................... 22
2.3.4. AT+QIDEACT    Deactivate a PDP Context .................................................................. 23
2.3.5. AT+QIOPEN    Open a Socket Service ......................................................................... 24
2.3.6. AT+QICLOSE    Close a Socket Service ....................................................................... 26
2.3.7. AT+QISTATE    Query Socket Service Status ............................................................... 27
2.3.8. AT+QISEND    Send Data ............................................................................................. 29
2.3.9. AT+QIRD    Retrieve Received TCP/IP Data ................................................................ 31
2.3.10. AT+QISENDEX    Send Hex String Data ...................................................................... 32
2.3.11. AT+QIACCEPT    Manually Accept New TCP Incoming Connection ............................ 33
2.3.12. AT+QISWTMD    Switch Data Access Mode ................................................................. 34
2.3.13. AT+QPING    Ping a Remote Host ................................................................................ 35
2.3.14. AT+QNTP    Synchronize Local Time with NTP Server ................................................ 36
2.3.15. AT+QIDNSCFG    Configure Address of DNS Server ................................................... 37
2.3.16. AT+QIDNSGIP    Get IP Address by Domain Name ..................................................... 38
2.3.17. AT+QISDE    Control Whether to Echo Data for AT+QISEND ...................................... 39
2.3.18. AT+QIGETERROR    Query Last Error Code ............................................................... 40
2.4. Description of URCs ................................................................................................................ 40
2.4.1. +QIURC: "closed"    Indicate Closed Connection ......................................................... 41
2.4.2. +QIURC: "recv"    Indicate Incoming Data .................................................................... 41
2.4.3. +QIURC: "incoming full"    Indicate Incoming Connection Reached Limit .................... 42
2.4.4. +QIURC: "incoming"    Indicate Incoming Connection in Automatic Acceptance Mode 42
2.4.5. +QIURC: "accept"    Indicate Incoming Connection in Manual Acceptance Mode ....... 43
2.4.6. +QIURC: "pdpdeact"    Indicate PDP Deactivation ....................................................... 43
3 Examples ........................................................................................................................................... 44
3.1. Configure and Activate a Context ............................................................................................ 44
3.1.1. Configure a Context ...................................................................................................... 44


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                         5 / 56



3.1.2. Activate a Context ......................................................................................................... 44
3.1.3. Deactivate a Context ..................................................................................................... 44
3.2. TCP Client Works in Buffer Access Mode under IPv4 ............................................................. 44
3.2.1. Set up a TCP Client Connection and Enter Buffer Access Mode ................................. 44
3.2.2. Send Data in Buffer Access Mode ................................................................................ 45
3.2.3. Retrieve Data from Remote Server in Buffer Access Mode ......................................... 45
3.2.4. Close a Connection ....................................................................................................... 46
3.3. TCP Client Works in Transparent Transmission Mode ............................................................ 46
3.3.1. Set up a TCP Client Connection and Enter Transparent Transmission Mode ............. 46
3.3.2. Send Data in Transparent Transmission Mode ............................................................ 47
3.3.3. Retrieve Data from Remote Server in Transparent Transmission Mode ...................... 47
3.3.4. Close a TCP Client ........................................................................................................ 47
3.4. TCP Client Works in Direct Push Mode ................................................................................... 47
3.4.1. Set up a TCP Client Connection and Enter Direct Push Mode .................................... 47
3.4.2. Send Data in Direct Push Mode.................................................................................... 48
3.4.3. Receive Data from Remote Server in Direct Push Mode ............................................. 48
3.4.4. Close a TCP Client ........................................................................................................ 48
3.5. TCP Server Works in Buffer Access Mode under IPv4 ........................................................... 48
3.5.1. Start a TCP Server ........................................................................................................ 48
3.5.2. Accept TCP Incoming Connection ................................................................................ 49
3.5.3. Retrieve Data from Incoming Connection ..................................................................... 49
3.5.4. Close a TCP Server ...................................................................................................... 49
3.6. UDP Service under IPv4 .......................................................................................................... 50
3.6.1. Start a UDP Service ...................................................................................................... 50
3.6.2. Send UDP Data to Remote Client ................................................................................. 50
3.6.3. Retrieve Data from Remote Client ................................................................................ 50
3.6.4. Close a UDP Service .................................................................................................... 51
3.7. PING under IPv4 ...................................................................................................................... 51
3.8. Synchronize Local Time ........................................................................................................... 51
3.9. Get Last Error Code ................................................................................................................. 52
4 Summary of Error Codes ................................................................................................................. 53
5 Appendix References ....................................................................................................................... 55



LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                                     6 / 56



## Table Index
Table 1: Types of AT Commands ................................................................................................................11
Table 2: Summary of Error Codes .............................................................................................................. 53
Table 3: Terms and Abbreviations .............................................................................................................. 55



LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                         7 / 56



## 1  Introduction

Quectel EG800Q series and  EG91xQ family (EG915Q series and  EG916Q-GL) modules feature an
embedded TCP/IP stack, which enables the host to access the Internet directly via AT commands. This
significantly reduces the dependence on external PPP and TCP/IP protocol stacks and thus minimizes the
cost.

EG800Q and EG91xQ family modules provide the following socket services:

⚫ TCP client
⚫ UDP client
⚫ TCP server
⚫ UDP server

1.1. Using TCP/IP AT Commands

Through TCP/IP AT commands, the host can configure a PDP context, activate/deactivate the PDP context,
open/close a socket service and send/retrieve data via the socket service. The following figure illustrates
how to use TCP/IP AT commands.



LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                                     8 / 56



Power on the module

Query (U)SIM card
status by
## AT+CPIN?
Query (U)SIM Card Status:
## Execute
## AT+CPIN
. Reboot the module if
## AT+CPIN?

fails to identify (U)SIM card in
## 20s.
Recommended Power on/off:
- Power off: Send
## AT+QPOWD

command,
and then cut off power after 12s or more.
- Power on: Keep power key as low for 2s,
and then pull it to high.
## Activate
the
## PDP
c
ontext

by
AT+QIACT=<contextID>
and
## AT+QIACT?
## Activ
ate

a PDP Context:
- Activate the PDP context by
## AT+QIACT
=<contextID>
## .
## 2.

Query IP address of
the
## PDP
context

by
## AT+QIACT?
## .
## Notes:
a) All above commands should be
executed together in sequence.
b)  Reboot the module if there is no
response for
## AT+QIACT

in 1
## 5
## 0s.
c)  If failed to deactivate the PDP
context for 3 times continuously,
then reboot the module.
## 3
. Deactivate the PDP context:
reboot the module if there is no
response for
## AT+QIDEACT

in
## 4
## 0s.
Open connection
by
## AT+QIOPEN
Send data by
## AT+QISEND
=<connectID>
Query sending
ACK by
## AT+QI
## SEND=
<connectID>,0
Close connection
by
## AT+QICLOSE
## =
<connectID>

## Deactivate
the PDP context by
## AT+QIDEACT
## =
<contextID>
Identify (U)SIM card
Successfully activate PDP context
Reboot the module if there is
no response in
## 10s
## .
+QIOPEN: <connectID>,<err>
Get ACK
in 90s
PS Service
## :
## 1
## . If
## <stat>

of
## AT+CGREG?
## /AT+CEREG?

equals to 1 or 5, it means that the module
has registered on
## PS
d
omain
s
ervice in
UMTS/LTE network
## .
- Go to next step no matter whether it is
registered on
## PS
d
omain
s
ervice

or not in
## 60s.
If failed to identify (U)SIM
card in 20s, then reboot the
module.
- Reboot the module if
there is no response in
## 1
## 5
## 0s.
- If failed to deactivate the
PDP context for 3 times
continuously, then reboot
the module.
## Notes:
- P lease note th at users need  to wa it for the  final  respo nse ( for  example  " OK", "CME ERRO R" , "CMS ERROR") of the last AT co m
mand you entered  b efo re yo u e nte r the ne xt A T command.
You  can rebo ot th e mod ule if the mod ule fai ls to  ge t response in 60s.
- Rebo ot the mo dule if the mo dule has not got response of AT+Q IACT i n 1 50s or respon se of A T+QICL OSE in 10s a nd  in 40s.
- It is NO T recommende d to fre quen tly r eboot the  modu le. When  the module  ha s been  continuo usl y rebo ote d for 3 times due to f
aile d A T comma nd exe cu tion , it can be rebo ote d i mmed iate ly
for the first time after  that. If it still fails, rebo ot th e mod ule after 10 minutes for the  second time , and rebo ot afte r 30

minutes for th e third time, one hou r for th e fourth ti me, etc.
## 4.
Ple ase note tha t accordi ng to  the 3G PP protocol, th e LTE mo dule will automatically acti va te  a de fau lt be arer (typically wi th
PDP context ID 1 ) a fter succe ssfully attachi ng to  the network. If
the activation fail s or  re sults in  a n une xp ecte d bea rer typ e due  to an  A PN issu e, p lease re co nfigur e it b y follo wing these st
ep s: fi rst, execute
## AT+CFUN=0

to disab le th e network; th en, use
## AT+QICS GP

to configur e the correct APN; final ly, e xe cute
## AT+CFUN=1

to re
boo t

the mo dule and re
## -
initiate the  activa tion .
## Query
## PS
s
ervice

in 60s by
## AT+CGREG?
## /
## AT+CEREG?
Failed to activate
the PDP context
- Failed to open connection
continuous
ly for 5 times.
- There is no response in
## 15
## 0s.
Failed to get ACK in 90s
## OK

The PDP
context is
deactivated
successfully
Reboot the
module if
there is no
response in
## 4
## 0s.
## Open Connection:
- If failed to get response of
## AT+QIOPEN

in
## 15
0s, then
close connection.
- If failed to open connection for 5 times continuously,
deactivate the PDP context first, and then re
## -
activate the
PDP context and open connection again.
Send AT command and wait for the module to respond OK
TCP Connection Maintenance and Detection:
- Send data by
## AT+QISEND
=<connectID>[,<send_length>]
## .
It is recommended to periodically send a small data packet to the remote
end to maintain and detect the TCP connection.
- Query sending ACK by
## AT+QIS
END=<connectID>
## ,0
## .
If the package is still not acknowledged after two minutes (query every 5
seconds, 24 times in total), the TCP connection may be abnormal.
## Execute
## AT+QICLOSE

to close the current connection, and then execute
## AT+QIOPEN

to re
## -
establish the TCP connection.
## Configure
the
## PDP
context

and
## Qos

by
## AT+QICSGP
## /
## AT+CGQREQ/
## AT+CGEQREQ
## /
## AT+CGQMIN/
## AT+CGEQMIN
## Configure
a
## PDP
## Context:
## 1.

Configure APN, user name, password

and
auth type by
## AT+QICSGP
## .
## 2.

Configure QoS settings by
## AT+CGQMIN/
## AT+CGEQMIN/ AT+CGQREQ/
## AT+CGEQREQ
## .
## If
it fails in getting the
## PS
regist
ration

status
## ,

configure the PDP
context

and

then

execute
## AT+
## CFUN
## =0

and
## AT+
## CFUN
## =1
. If this still
fails,
reboot the module.

Figure 1: Flow Chart of TCP/IP AT Command Use


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                         9 / 56



1.2. Description of Data Access Modes

EG800Q and EG91xQ family modules support three data access modes:

⚫ Buffer access mode
⚫ Direct push mode
⚫ Transparent transmission mode

When opening a  socket  service via AT+QIOPEN, you  can  specify the  data  access  mode with
<access_mode>. After the socket  service is  opened, the data access  mode  can  be  switched with
## AT+QISWTMD.

- In buffer access mode, data can be sent via AT+QISEND, and if the module receives the data from
the Internet, it will buffer them and report a URC +QIURC: "recv",<connectID>. Data can be read
via AT+QIRD.

- In direct push mode, data can be sent via AT+QISEND, and if the module receives the data from the
Internet, the data will be outputted to COM port directly in the following format:
+QIURC: "recv",<connectID>,<currectrecvlength><CR><LF><data>.

- In transparent transmission mode, the corresponding port (such as UART, USB modem port, etc.) is
exclusively used for sending/receiving data directly to the Internet. The data received from COM port
will be sent to the Internet directly, and the data received from Internet will be outputted via COM port
directly.

⚫ Exit transparent transmission mode
To make the module exit transparent transmission mode either:

1) Execute +++. Follow the requirements below to prevent +++ from being misinterpreted as data:
a) Do not input any character for at least 1 second before and after inputting +++.
b) Input +++ within 1 s, and wait until OK is returned. After OK is returned, the module switches to
buffer access mode.

## OR

2) Change DTR from LOW to HIGH to make the module enter command mode (the COM port can
now be used for running AT commands, as well as for sending/retrieving data). In this case, set
AT&D1 before the module enters transparent transmission mode.

⚫ Return back to transparent transmission mode
To return to transparent transmission mode either:



LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                                     10 / 56



1) Execute AT+QISWTMD. Before  execution specify <access_mode> as  2.  Once transparent
transmission mode is entered successfully, CONNECT is returned.

## OR

2) Execute ATO. After a connection exits transparent transmission mode, executing ATO switches
the data access mode back to transparent transmission mode. Once transparent transmission
mode is entered successfully, CONNECT is returned. If no connection has entered transparent
transmission mode before, ATO returns NO CARRIER.



- In buffer access mode, if the buffer is not empty, and the module receives data again, it does not
report a new URC until all the received data have been retrieved via AT+QIRD from the buffer.
- In transparent transmission mode, AT commands cannot be executed. If the socket connection is
closed because of a network error or other errors, the module reports NO CARRIER and exits the
transparent transmission mode. In this case, execute AT+QICLOSE to close the socket service.
## NOTE


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                         11 / 56



2 Description of TCP/IP AT Commands

This chapter describes AT commands related to TCP/IP.

2.1. AT Command Introduction
## 2.1.1. Definitions
⚫ <CR>   Carriage return character.
⚫ <LF>   Line feed character.
⚫ <...>   Parameter name. Angle brackets do not appear on the command line.
⚫ [...]   Optional  parameter  of  a  command  or  an  optional  part  of  TA  information  response.
Square brackets do not appear on the command line. When an optional parameter is
not given in a command, the new value equals its previous value or the default settings,
unless otherwise specified.
⚫ Underline  Default setting of a parameter.

2.1.2. AT Command Syntax
All command lines must start with AT or at and end with <CR>. Information responses and result codes
always   start   and   end   with a   carriage   return   character   and a   line   feed  character:
<CR><LF><response><CR><LF>.  In  tables  presenting  commands  and  responses  throughout  this
document, only the commands and responses are presented, and <CR> and <LF> are deliberately omitted.

Table 1: Types of AT Commands
## Command Type Syntax Description
Test Command AT+<cmd>=?
Test  the  existence  of  corresponding
command and return information about the
type, value, or range of its parameter.
Read Command AT+<cmd>?
Check  the  current  parameter  value  of  a
corresponding Write Command.
Write Command AT+<cmd>=<p1>[,<p2>[,<p3>[...]]] Set user-definable parameter value.
Execution Command AT+<cmd>
Return a specific information parameter or
perform a specific action.


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                                     12 / 56



2.2. Declaration of AT Command Examples

The AT command examples in this document are provided to help you learn about the use of the AT
commands introduced  herein.  The  examples,  however,  should  not  be  taken  as  Quectel’s
recommendations or suggestions about how to design a program flow or what status to set the module
into. Sometimes multiple examples may be provided for one AT command. However, this does not mean
that there is a correlation among these examples, or that they should be executed in a given sequence.
The URLs, domain names, IP addresses, usernames/accounts, and passwords (if any) in the AT command
examples are provided for illustrative and explanatory purposes only, and they should be modified to reflect
your actual usage and specific needs.

2.3. Description of TCP/IP AT Commands
2.3.1. AT+QICFG    Configure Optional Parameters
This command configures optional parameters.
AT+QICFG  Configure Optional Parameters
## Test Command
## AT+QICFG=?
## Response
+QICFG:  "transpktsize",(range  of  supported <transpktsize>s)
+QICFG:  "transwaittm",(range  of  supported <transwaittm>s)
+QICFG:  "tcp/retranscfg",(range  of  supported <max_backoffs>
s),(range  of  supported <max_rto>s)
+QICFG:  "tcp/accept",(list  of  supported <accept_mode>s)
+QICFG:  "passiveclosed",(list  of  supported <closed>s)
+QICFG:  "dataformat",(list  of  supported <send_data_format>s),
(list  of  supported <recv_data_format>s)
+QICFG:  "viewmode",(list  of  supported <viewmode>s)
+QICFG:  "qisend/timeout",(range  of  supported <time>s)
+QICFG: "send/auto",(range  of  supported <connectID>s),(range
of  supported <period>s),<msg_auto>,(range  of  supported <dat
a_format>s),(range  of  supported <try_times>s)
+QICFG:  "recvind",(list  of  supported <enable>s)
+QICFG: "formatcfg",(list of supported <format>s)
+QICFG: "close/mode",(list  of supported <close_mode>s)
+QICFG:  "udp/sendmode",(list  of  supported <send_mode>s)
+QICFG:  "udp/readmode",(list  of  supported <read_mode>s)
+QICFG:  "tcp/keepalive",(list  of  supported <enable>s,(range  of
supported <idle_time>s),(range  of  supported <interval_time>s),(r
ange  of  supported <probe_cnt>s)
+QICFG: "tcp/delay_ack",(list  of  supported <delay_ack>s)


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                         13 / 56



+QICFG:  "dns/cache",(list  of  supported <DNS_cache>s)
+QICFG: "tcp/twrecycle",(range  of  supported <timewait_timeou
t>s)
+QICFG: "dns/retry",(range  of  supported <retry_ack>s),(range
of  supported <retry_timeout>s)
+QICFG: "send/buffersize",(range  of  supported <send_buffersi
ze>s)
+QICFG: "window/scale",(list of  supported <window_scale_ena
ble>s)

## OK
## Write Command
Query/set the  packet  size  for
transparent transmission mode
AT+QICFG="transpktsize"[,<tr
anspktsize>]
## Response
If the optional parameter is omitted, query the current setting:
+QICFG: "transpktsize",<transpktsize>

## OK

If the  optional  parameter is  specified, set the  packet  size  for
transparent transmission mode:
## OK

If there is any error:
## ERROR
## Write Command
Query/set the  waiting  time  for
transparent transmission mode
AT+QICFG="transwaittm"[,<tr
answaittm>]
## Response
If the optional parameter is omitted, query the current setting:
+QICFG: "transwaittm",<transwaittm>

## OK

If the  optional  parameter is  specified, set the  waiting  time for
transparent transmission mode:
## OK

If there is any error:
## ERROR
## Write Command
Query/set the  maximum interval
and     the number     of TCP
retransmissions
AT+QICFG="tcp/retranscfg"[,<
max_backoffs>,<max_rto>]
## Response
If the optional parameter is omitted, query the current setting:
+QICFG: "tcp/retranscfg",<max_backoffs>,<max_rto>

## OK

If the optional parameter is specified, set the maximum interval and
the number of TCP retransmissions:
## OK


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                                     14 / 56




If there is any error:
## ERROR
## Write Command
Enable  or  disable  the  automatic
reception of the TCP connection
from the client
AT+QICFG="tcp/accept"[,<acc
ept_mode>]
## Response
If the optional parameter is omitted, query the current setting:
+QICFG: "tcp/accept",<accept_mode>

## OK
If the optional parameter is specified, enable or disable the automatic
reception of the TCP connection from the client:
## OK

If there is any error:
## ERROR
## Write Command
Enable  or  disable  the  passive
closing of TCP connection when
the server is closed
AT+QICFG="passiveclosed"[,
## <closed>]
## Response
If the optional parameter is omitted, query the current setting:
+QICFG: "passiveclosed",<closed>

## OK

If the parameter is specified, enable or disable the passive closing of
TCP connection when the server is closed:

## OK

If there is any error:
## ERROR
## Write Command
Query/set the format of the data
to be sent or received (only for
buffer  access  mode  and  direct
push mode)
AT+QICFG="dataformat"[,<se
nd_data_format>,<recv_data_
format>]

## Response
If the optional parameters are omitted, query the current setting:
+QICFG: "dataformat",<send_data_format>,<recv_data_forma
t>

## OK
If the optional parameters are specified, set the format of the data to
be sent or received:
## OK

If there is any error:
## ERROR
## Write Command
Query/set the output  format  of
received  data (only  for  buffer
access  mode  and  direct  push
mode)
## Response
If the optional parameter is omitted, query the current setting:
+QICFG: "viewmode",<view_mode>

## OK


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                         15 / 56



AT+QICFG="viewmode"[,<vie
w_mode>]


If the optional parameter is specified, set the output format of the
received data:
## OK

If there is any error:
## ERROR
## Write Command
Query/set the input data timeout
AT+QICFG="qisend/timeout"[,
## <time>]
## Response
If the optional parameter is omitted, query the current setting:
+QICFG: "qisend/timeout",<time>

## OK

If the parameter is specified, set the input data timeout:
## OK

If there is any error:
## ERROR
## Write Command
Set to send the specified data on
the specified channel
AT+QICFG="send/auto",<con
nectID>[,<period>[,<msg_aut
o>][,<data_format>][,<try_time
s>]]

## Response
If the  optional  parameters are  omitted,  query  the  current  setting:
+QICFG: "send/auto",<connectID>,<period>,<msg_auto>,<data
## _format>,<try_times>

## OK

If the optional parameters are specified, set to send the specified data
on the specified channel:
## OK

If there is any error:
## ERROR
## Write Command
Query/set whether to display the
data  length  in  the  URC format
reported   by   the   module   after
receiving  data  in buffer  access
mode
AT+QICFG="recvind"[,<enabl
e>]
## Response
If the parameter is omitted, query the current setting:
+QICFG: "recvind",<enable>

## OK
If the parameter is specified, set whether to display the data length in
the URC reported by the module after receiving data in buffer access
mode:
## OK

If there is any error:
## ERROR


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                                     16 / 56



## Write Command
Set the ">" format configuration
when AT+QISEND enters the
transparent transmission mode
AT+QICFG="formatcfg"[,<for
mat>]
## Response
If the  optional  parameter is  omitted,  query  the  current  setting:
+QICFG:  "formatcfg",<format>

## OK

If the optional parameter is specified, set the ">" format configuration
when AT+QISEND enters the transparent transmission mode:
## OK

If there is any error:
## ERROR
## Write Command
Enable  or disable  displaying  the
return information of
AT+QICLOSE in URC form
AT+QICFG="close/mode",<clo
se_mode>
## Response
If the  optional  parameter is  omitted,  query  the  current  setting:
+QICFG:  "close/mode",<close_mode>

## OK

If the optional parameter is specified, enable or disable displaying the
return information of AT+QICLOSE in the form of URC:
## OK

If there is any error:
## ERROR
## Write Command
Query/set the  mode for sending
UDP data
AT+QICFG="udp/sendmode"[,
## <send_mode>]
## Response
If the optional parameter is omitted, query the current setting:
+QICFG: "udp/readmode",<send_mode>

## OK

If the optional parameter is specified, set the UDP data sending mode:
## OK

If there is any error:
## ERROR
## Write Command
Query/set the mode for reading
UDP data
AT+QICFG="udp/readmode"[,
## <read_mode>]
## Response
If the optional parameter is omitted, query the current setting:
+QICFG: "udp/readmode",<read_mode>

## OK

If the optional parameter is specified, set the UDP data reading mode:
## OK



LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                         17 / 56



If there is any error:
## ERROR
## Write Command
Query/set whether  to  send  TCP
keep-alive information
AT+QICFG="tcp/keepalive",<e
nable>[,<idle_time>,<interval_
time>,<probe_cnt>]
## Response
If the optional parameters are omitted, query the current setting:
+QICFG: "tcp/keepalive",<enable>,<idle_time>,<interval_time>,
## <probe_cnt>

## OK

If the optional parameters are specified, set whether to send TCP
keep-alive information:
## OK

If there is any error:
## ERROR
## Write Command
Enable  or  disable the delayed
acknowledgment of TCP packets
AT+QICFG="tcp/delay_ack"[,<
delay_ack>]
## Response
If the  optional  parameters are  omitted,  query  the  current  setting:
+QICFG:  "tcp/delay_ack",<delay_ack>

## OK

If the optional parameters are specified, enable or disable the delayed
acknowledgment of TCP packets:
## OK

If there is any error:
## ERROR
## Write Command
Enable or disable the DNS cache
AT+QICFG="dns/cache"[,<DN
## S_cache>]
## Response
If the optional parameter is omitted, query the current setting:
+QICFG: "dns/cache",<DNS_cache>

## OK

If the  optional  parameter is  specified,  enable  or  disable the  DNS
cache:
## OK

If there is any error:
## ERROR
## Write Command
Set the MSL time for
TIME_WAIT after the TCP
fourth-way handshake
AT+QICFG="tcp/twrecycle"[,<t
## Response
If the  optional  parameter is  omitted,  query  the  current  setting:
+QICFG:  "tcp/twrecycle",<timewait_timeout>

## OK


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                                     18 / 56



imewait_timeout>]
If the  optional  parameter is  specified, set  the  MSL  time  for
TIME_WAIT after the TCP fourth-way handshake.
## OK

If there is any error:
## ERROR
## Write Command
Set the retry count and retry
time of DNS
AT+QICFG="dns/retry"[,<retry
## _ack>[,<retry_timeout>]]
## Response
If the  optional  parameter is  omitted,  query  the  current  setting:
+QICFG:  "dns/retry",<retry_ack>,<retry_timeout>

## OK

If the optional parameter is specified, set the retry count and retry time
of DNS.
## OK

If there is any error:
## ERROR
## Write Command
Set the  maximum  length  of  a
single transmission data
AT+QICFG="send/buffersize"[,
## <send_buffersize>]
## Response
If the optional parameter is omitted, query the current setting:
+QICFG:  "send/buffersize",<send_buffersize>

## OK

If the optional parameter is specified, set  the  maximum  length  of  a
single transmission data:
## OK

If there is any error:
## ERROR
## Write Command
Set  the  TCP  protocol  extension
option windows scale
AT+QICFG="windows/scale",[
## <window_scale_enable>]
## Response
If the optional parameters are omitted, query the current setting:
+QICFG: "windows/scale",<window_scale_enable>

## OK

If  the parameter is specified, set the TCP protocol extension option
windows scale:
## OK

If there is any error:
## ERROR


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                         19 / 56



## Parameter
## Maximum Response Time /
## Characteristic
The command takes effect immediately.
The configurations are not saved.
<transpktsize>             Integer type.  Maximum length of the data packet to be sent. Range: 1–1460.
Default value: 1024. Unit: byte.
<transwaittm>        Integer type. In transparent transmission mode, if the length of data received
from  the  port  is  less  than  the  specified  value  of <transpktsize>,  after
exceeding the time specified by <transwaittm>, data will be sent.
Range: 0–20. Default value: 2. Unit: 100 ms.
<max_backoffs>    Integer  type. Maximum number  of  TCP retransmissions.  Range: 3–20.
Default value: 12.
<max_rto> Integer  type. Maximum interval between TCP retransmissions. Range:
5–1000. Default value: 600. Unit: 100 ms.
<accept_mode>   Integer type. Acceptance mode for the incoming TCP connection from client.
0 Manually accept the incoming TCP connection from the client
1 Automatically accept the incoming TCP connection from the client
<closed>                             Integer type. Enable or disable the passive closing of TCP connection when
the server is closed
## 0 Disable
## 1 Enable
<send_data_format>   Integer type. Format of the data to be sent. The suffix “0x” is not needed when
the mode is set as Hex mode as the module will automatically form two bytes
to one ASCII code.
0 Text mode
1 Hex mode
<recv_data_format>    Integer type. Format of the data to be received. The suffix “0x” is not needed
when the mode is set as Hex mode as the module will automatically form two
bytes to one ASCII code.
0 Text mode
1 Hex mode
<view_mode>                  Integer type. Output format of received data.
0 data header\r\ndata.
1 data header,data.
<time>                                Integer type. Timeout of AT+QISEND. After > is returned, if no data is inputted
within  the  timeout  period, AT+QISEND will  be executed.  Range: 0–3600.
Default value: 0. Unit: ms.
<connectID> Integer type. The socket ID. Range: 1–11.
<period>    Integer type. The period that is configured for the specified channel to send
the specified data. Range: 0, 20–86400. Unit: second. 0 indicates disabling
automatic transmission; Default value: 60. Unit: seconds.


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                                     20 / 56



<msg_auto>    String type. Data sent in hexadecimal. The max length is 256 bytes.
<data_format>   Integer type. Set the data format of the input <msg_auto>.
0 The input data is in hexadecimal ASCII code (automatically converted to
hexadecimal string)
1 The input data is in hexadecimal string
2 The input data is in string
<try_times> Integer type. Retry times. Range: 0–10.
<enable>                            Integer type. In AT+QICFG="tcp/keepalive", it means whether to send TCP
keepalive. In AT+QICFG=”recvind”, it means whether to  display  the  data
length in the URC format of TCP/IP buffer access mode.
0 Disable corresponding function
1 Enable corresponding function
<format>    Integer  type.  Set  the  prompt  format  after AT+QISEND.  The  configuration
values of S3 and S4 are ATS3 and ATS4.
## 0 "S3S4>"
## 1 "S3S4>S3S4"
<close_mode>   Integer type. Enable or disable display AT+QISEND execution information in
URC form.
## 0 Disable
## 1 Enable
<send_mode>                  Integer type.
0 Block mode
1 Stream mode
<read_mode>                    Integer type.
0 Block mode
1 Stream mode
<idle_time>                         Integer type. Trigger keepalive time cycle. Range: 1–120. Default value: 1.
Unit: minute.
<interval_time>                Integer type. Interval between keepalive probes. If the sender does not receive
a  response  packet, it  continues  to  send  probe  packets  according  to  this
interval. Range: 25–100. Default value: 25. Unit: second.
<probe_cnt>              Integer type. Number of keepalive probes. Range: 3–10. Default: 3.
<delay_ack>    Integer type. Enable or disable the delayed acknowledgment of TCP packets.
## 0 Disable
## 1 Enable
<DNS_cache>                    Integer type. Enable or disable DNS cache.
## 0 Disable
## 1 Enable
<timewait_timeout> Integer  type. The  MSL  time  for  TIME_WAIT  after  the  TCP  fourth-way
handshake. Range: 2–60000. Default value: 2. Unit: ms.
<retry_ack>    Integer type. DNS retry count. Range: 1–4. Default value: 2. Unit: times.
<retry_timeout>   Integer type. DNS retry times. Range: 100–2000. Default value: 200. Unit: ms.
<send_buffersize>  Integer type. Maximum number of bytes sent at one time. Range: 1460–4096.
Default value: 1460.


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                         21 / 56






2.3.2. AT+QICSGP    Configure Parameters of a TCP/IP Context
This command configures <APN>, <username>, <password> and other parameters of a TCP/IP context.
## Parameter
<window_scale_enable> Integer type. Whether to enable the TCP protocol extension option windows
scale.
## 0 Disable
## 1 Enable
The settings of AT+QICFG="tcp/retranscfg"[,<max_backoffs>,<max_rto>] and +QICFG: "tcp/k
eepalive",<enable>,<idle_time>,<interval_time>,<probe_cnt> take  effect  on all  TCP  sockets
and  cannot  be  saved  to  NV.
AT+QICSGP  Configure Parameters of a TCP/IP Context
## Test Command
## AT+QICSGP=?
## Response
+QICSGP: (range of  supported <conntextID>s),(range  of
supported <context_type>s),<APN>,<username>,<passw
ord>,(range  of  supported <authentication>s)

## OK
## Write Command
Query  the  configuration  of  a specified
context
AT+QICSGP=<contextID>
## Response
+QICSGP: <context_type>,<APN>,<username>,<passwo
rd>,<authentication>

## OK
## Write Command
Configure the context
AT+QICSGP=<contextID>[,<context_
type>,<APN>[,<username>,<passwor
d>)[,<authentication>]]]
## Response
## OK

If there is any error:
## ERROR
## Maximum Response Time /
## Characteristics
The command takes effect immediately.
The configurations are saved automatically.
<contextID>          Integer type. Context ID. Range: 1–15.
<context_type>    Integer type. Protocol type.
## NOTE


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                                     22 / 56



## Example

2.3.3. AT+QIACT    Activate a PDP Context
Before activating a PDP context via AT+QIACT, the context should be configured with AT+QICSGP. After
activation, the IP address can be queried via AT+QIACT?.

The range of <contextID> is 1–15 and the module supports maximum three PDP contexts activated
simultaneously under LTE Cat M1. Depending on the network, it may take maximum 150 s to return OK
or ERROR after executing AT+QIACT. Other AT commands can be executed only after the response is
returned.
1 IPv4
2 IPv6
3 IPv4v6
<APN>      String type. Access point name.
<username>       String type. Username. Maximum length: 127 bytes.
<password>    String type. Password. Maximum length: 127 bytes.
<authentication>    Integer type. Authentication methods.
## 0 None
## 1 PAP
## 2 CHAP
3 PAP or CHAP
AT+QICSGP=1                                          //Query the configuration of context 1.
## +QICSGP: 1,"","","",0

## OK
AT+QICSGP=1,1,"UNINET","","",1    //Configure context 1. APN is "UNINET" for China Unicom.
## OK
AT+QIACT    Activate a PDP Context
## Test Command
## AT+QIACT=?
## Response
+QIACT: (range of supported <contextID>s)

## OK
Read command
## AT+QIACT?
## Response
Return the list of the currently activated contexts and their IP
addresses:
+QIACT: 1,<context_state>,<context_type>[,<IP_addres
s>]
## [.....


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                         23 / 56



## Parameter

2.3.4. AT+QIDEACT    Deactivate a PDP Context
This command deactivates a specific context and closes all TCP/IP connections set up in this context.
Depending on the network, it may take maximum 40 seconds to return OK or ERROR after executing
AT+QIDEACT. Other AT commands can be executed only after the response is returned.
+QIACT: 15,<context_state>,<context_type>[,<IP_addres
s>]]

## OK
## Write Command
Activate a specified PDP context
AT+QIACT=<contextID>
## Response
## OK

If there is any error:
## ERROR
Maximum Response Time 150 s, determined by the network.
## Characteristics /
<contextID>    Integer type. Context ID. Range: 1–15.
<context_state>            Integer type. Context state.
## 0  Deactivated
## 1  Activated
<context_type>                Integer type. Protocol type.
1  IPv4
2      IPv6
3  IPv4v6
<IP_address>                String type. Local IP address after the context is activated.
AT+QIDEACT  Deactivate a PDP Context
## Test Command
## AT+QIDEACT=?
## Response
+QIDEACT: (range of supported <contextID>s )

## OK
## Write Command
AT+QIDEACT=<contextID>
## Response
## OK

If there is any error:
## ERROR


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                                     24 / 56



## Parameter

2.3.5. AT+QIOPEN    Open a Socket Service
This command opens a socket service. The service type can be specified by <service_type>. The data
access mode (buffer access mode, direct push mode and transparent transmission mode) can be specified
by <access_mode>. The response +QIOPEN: <connectID>,<err> indicates whether the socket service
has been opened successfully.

- If <service_type> is "TCP LISTENER", the module works as a TCP server, and there are two kinds
of <accept_mode>, automatic acceptance and manual acceptance. In automatic acceptance mode,
after accepting a new TCP connection, the module automatically specifies <connectID> and report
the URC +QIURC: "incoming",<connectID>,<serverID>,<remoteIP>,<remote_port>. The range
of <connectID> is 0–11. The type of this new incoming connection is "TCP INCOMING" and the
<access_mode> of "TCP INCOMING" is  the  same as that of "TCP  LISTENER". In manual
acceptance mode, if there is a new incoming connection to be accessed, the module will report the
URC +QIURC: "accept",<connectID>, and <connectID> is the ID of "TCP LISTENER".

- If <service_type> is "UDP SERVICE", UDP data can be sent to or received from the remote IP via
## <local_port>.

⚫ Send data: execute AT+QISEND=<connectID>,<send_length>,<remoteIP>,<remote_port>.
⚫ Receive   data   in   direct   push   mode:   the   module   reports the URC +QIURC:
"recv",<connectID>,<currentrecvlength>,<remoteIP>,<remote_port><CR><LF><data>.
⚫ Receive data in buffer access mode: the module reports the URC +QIURC: "recv",<connectID>,
and then the received data can be retrieved via AT+QIRD=<connectID>.

- It is suggested to wait for 150 s for +QIOPEN: <connectID>,<err> to be outputted after executing the
Write Command. If the response cannot be received in 150 s , use AT+QICLOSE to close the socket.
Maximum Response Time 40 s, determined by the network.
## Characteristics /
<contextID>                  Integer type. Context ID. Range: 1–15.
AT+QIOPEN    Open a Socket Service
## Test Command
## AT+QIOPEN=?
## Response
+QIOPEN: (range   of   supported <contextID>s),(range   of
supported <connectID>s),"TCP/UDP/TCP LISTENER/UDP
SERVICE","<IP_address>/<domain_name>",<remote_por


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                         25 / 56



## Parameter
t>,<local_port>,(range of supported <access_mode>s)

## OK
## Write Command
AT+QIOPEN=<contextID>,<connectI
D>,<service_type>,<IP_address>/<d
omain_name>,<remote_port>[,<local
## _port>[,<access_mode>]]
## Response
If the  service is in transparent  transmission  mode
(<access_mode>=2) and is opened successfully:
## CONNECT

If there is any error:
## ERROR
Error description can be retrieved via AT+QIGETERROR.

If the service is in buffer access mode (<access_mode>=0)
or direct push mode (<access_mode>=1):
## OK

+QIOPEN: <connectID>,<err>

<err> is 0 when the service is opened successfully. In other
cases, <err> is not 0.
Maximum Response Time 150 s, determined by the network.
## Characteristics /
<contextID>         Integer type. Context ID. Range: 1–15.
<connectID>        Integer type. Socket service index. Range: 0–11.
<service_type>  String type. Socket service type.
"TCP"     Start a TCP connection as a client
"UDP"     Start a UDP connection as a client
"TCP LISTENER"   Start a TCP server to listen to TCP connection
"UDP SERVICE"    Start a UDP service
<IP_address>        String type.
If <service_type> is "TCP" or "UDP", it indicates the IP address of remote server,
such as 192.0.2.2.
If <service_type> is "TCP LISTENER" or "UDP SERVICE", it can be omitted.
<domain_name>  String type. Domain name address of the remote server.
<remote_port> Integer type. Port number of the remote server. Range: 0-65535.
If <service_type> is "TCP" or "UDP" this parameter must be specified.
If <service_type> is "TCP LISTENER" or "UDP SERVICE", specify this parameter as
## 0.


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                                     26 / 56




2.3.6. AT+QICLOSE    Close a Socket Service
This command closes a specified  socket  service. Depending  on  the network, it  takes maximum 10  s
(default  value,  can  be  modified  by <timeout>) to  return OK or ERROR after executing AT+QICLOSE.
Other AT commands can be executed only after the response is returned.
## Parameter

<local_port>               Local port. Range: 0–65535.
If <service_type> is "TCP LISTENER" or "UDP SERVICE", this parameter must be
specified.
If <service_type> is "TCP" or "UDP" and <local_port> is 0, the local port will be
assigned automatically. Otherwise, the local port is assigned as specified.
<access_mode>   Integer type. Data access mode of the socket service.
0   Buffer access mode
1   Direct push mode
2   Transparent transmission mode
<err>             Integer type. Err code. See Chapter 4 for details.
AT+QICLOSE  Close a Socket Service
## Test Command
## AT+QICLOSE=?
## Response
+QICLOSE: (range of supported <connnectID>s),(range
of supported <timeout>s)

## OK
## Write Command
AT+QICLOSE=<connectID>[,<timeout>]
## Response
If the socket service is closed successfully:
## OK

If the command failed to close the socket service:
## ERROR
Maximum Response Time Determined by <timeout>
## Characteristics /
<connectID> Integer type. Socket service index. Range: 0–11.
<timeout> Integer type. Timeout value for the response to be outputted. If the FIN ACK of the other
peer is not received within <timeout>, the module will be forced to close the socket.
Range: 0–65535. Default value: 10. Unit: second.


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                         27 / 56



2.3.7. AT+QISTATE    Query Socket Service Status
This command  queries the  socket  service  status.  If  the <query_type> is  0,  it  returns the  status  of  all
existing socket services in the specified context. If the <query_type> is 1, it returns the status of a specified
socket service.
## Parameter
AT+QISTATE    Query Socket Service Status
## Test Command
## AT+QISTATE=?
## Response
## OK
Read/Execution Command
AT+QISTATE? or AT+QISTATE
## Response
Return the status of all existing connections:
+QISTATE: <connectID>,<service_type>,<IP_address>,<
remote_port>,<local_port>,<socket_state>,<contextID>,<
serverID>,<access_mode>,<AT_port>
## [...]

## OK
## Write Command
If <query_type> is  0,  query the
connection status of a specified context
AT+QISTATE=<query_type>,<contex
tID>
## Response
Return  the  status  of  all  existing connections  in a specified
context:
+QISTATE: <connectID>,<service_type>,<IP_address>,<
remote_port>,<local_port>,<socket_state>,<contextID>,<
serverID>,<access_mode>,<AT_port>
## [...]

## OK
## Write Command
If <query_type> is  1, query the
connection status of a specified socket
service
AT+QISTATE=<query_type>,<connec
tID>
## Response
+QISTATE: <connectID>,<service_type>,<IP_address>,<
remote_port>,<local_port>,<socket_state>,<contextID>,<
serverID>,<access_mode>,<AT_port>

## OK
## Maximum Response Time /
## Characteristics /
<query_type>     Integer type. Query type.
0 Query connection status of all socket services in a specified context
1 Query connection status of a specified socket service


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                                     28 / 56




<contextID>       Integer type. Context ID. Range: 1–15.
<connectID>    Integer type. Socket service index. Range: 0–11.
<service_type>  String type. Socket service type.
"TCP"    Start a TCP connection as a client
"UDP"    Start a UDP connection as a client
"TCP LISTENER"   Start a TCP server to listen for TCP incoming connection
"TCP INCOMING"   Start a TCP connection accepted by a TCP server
"UDP SERVICE"         Start a UDP service
"SSL Client"         Start an SSL connection as a client
<IP_address> String type. IP address.
If <service_type>="TCP" or "SSL Client”" or "UDP", it is the IP address of a remote
server.
If <service_type>="TCP LISTENER" or "UDP SERVICE", it is the local IP address.
If <service_type>="TCP INCOMING", it is the IP address of a remote client.
<remote_port>  Integer type. Remote port number.
If <service_type>="TCP" or "SSL Client" or "UDP", it is the port of a remote server.
If <service_type>="TCP LISTENER" or "UDP SERVICE", the port is invalid.
If <service_type>="TCP INCOMING", it is the port of a remote client.
<local_port>        Integer type. Local port number.
If <local_port> is 0, then the local port is assigned automatically.
<socket_state>  Integer type. Socket service status.
0  "Initial": connection has not been established
1  "Opening": client is connecting or server is trying to listen
2  "Connected": client/incoming connection has been established
3  "Listening": server is listening
4  "Closing": connection is closing
<serverID>  Integer  type. It is  valid  only when <service_type> is "TCP  INCOMING".
<serverID> represents which server accepts this TCP incoming connection, and the
value is the same as <connectID>  when the <service_type> is "TCP LISTENER".
<access_mode> Integer type. Data access mode.
0      Buffer access mode
1       Direct push mode
2       Transparent transmission mode
<AT_port>   String type. COM port of socket service.
"usbmodem"   USB modem port
"usbat"              USB AT port
"uart1"                UART port1
"cmux1"            MUX port 1
"cmux2"            MUX port 2
"cmux3"            MUX port 3
"cmux4"            MUX port 4


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                         29 / 56



2.3.8. AT+QISEND    Send Data
In buffer access mode (<access_mode>=0) or direct push mode (<access_mode>=1), then the data can
be sent via AT+QISEND. If the data is sent to the module successfully, SEND OK is returned. Otherwise,
SEND FAIL or ERROR is returned.

⚫ SEND FAIL indicates the sending buffer is full. In this case, the data can be resent.
⚫ ERROR indicates an error in the data sending process. In this case, wait for some time before you
resend the data. Maximum data length: 1460 bytes.
⚫ SEND OK means that the data have been sent to the peer, but it does not mean they have reached
the   peer successfully. You can   query   whether   the   data have reached   the peer by
AT+QISEND=<connectID>,0.
AT+QISEND    Send Data
## Test Command
## AT+QISEND=?
## Response
+QISEND: (range  of  supported <connectID>s),(range  of
supported <send_length>s)

## OK
## Write Command
Send   variable-length   data   when
<service_type> is  "TCP",  "UDP"  or
## "TCP INCOMING"
AT+QISEND=<connectID>
## Response
## >
After the response >, input the data to be sent. Tap Ctrl+Z to
send the data, and tap Esc to cancel the sending operation

If the connection  has  been  established  and the  data  is  sent
successfully:
## SEND OK

If the connection has been established but the sending buffer
is full:
## SEND FAIL

If the connection is not established, abnormally closed, or any
parameter is incorrect:
## ERROR
## Write Command
Send    fixed-length    data when
<service_type> is  "TCP",  "UDP"  or
## "TCP INCOMING"
AT+QISEND=<connectID>,<send_len
gth>

## Response
## >
After the response >, input the data until the data length equals
## <send_length>.

If  the  connection  has  been  established  and  the  data  is  sent
successfully:
## SEND OK


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                                     30 / 56





If the connection has been established but the sending buffer
is full:
## SEND FAIL

If the connection has not been established, abnormally closed,
or any parameter is incorrect:
## ERROR
## Write Command
If <service_type> is "UDP SERVICE"
AT+QISEND=<connectID>,<send_len
gth>,<remoteIP>,<remote_port>

## Response
Sends fixed length data to a specified remote IP address and
remote port. <service_type> must be "UDP SERVICE".
## >
After response >,  input the  data until the  data  length  equals
## <send_length>

If  the  connection  has  been  established  and  the  data  is  sent
successfully:
## SEND OK

If the connection has been established but the sending buffer
is full:
## SEND FAIL

If the connection has not been established, abnormally closed,
or any parameter is incorrect:
## ERROR
## Write Command
When <send_length> is  0,  query  the
sent data
AT+QISEND=<connectID>,0

## Response
If the specified connection exists:
+QISEND:  <total_send_length>,<ackedbytes>,<unacked
bytes>

## OK

If the specified connection does not exist, or if there is any
error:
## ERROR
## Maximum Response Time /
## Characteristics /


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                         31 / 56



## Parameter

2.3.9. AT+QIRD   Retrieve Received TCP/IP Data
In  buffer  access  mode, after receiving data,  the  module buffers it  and reports URC +QIURC:
"recv",<connectID>, then the data can be retrieved with AT+QIRD.

Please note that if the buffer is not empty, and the module receives data again, it will not report a new URC
until all received data has been retrieved via AT+QIRD from the buffer.
<connectID>            Integer type. Socket service index. Range: 0–11.
<send_length>  Integer type. Length of data to be sent. Range: 0–1460. Unit: byte.
<remoteIP>          String  type. Remote  IP  address (must  be  dot  format). It  is  valid  only when
<service_type> is "UDP SERVICE".
<remote_port>          Integer type. Remote port. It is only valid when <service_type> is "UDP SERVICE".
<total_send_length> Integer type. Total length of sent data. Unit: byte.
<ackedbytes>   Integer type. Total length of acknowledged data. Unit: byte.
<unackedbytes>        Integer type. Total length of unacknowledged data. Unit: byte.
AT+QIRD  Retrieve Received TCP/IP Data
## Test Command
## AT+QIRD=?
## Response
+QIRD:   (range   of   supported <connectID>s),(range   of
supported <read_length>s)

## OK
## Write Command
When <service_type> is "TCP", "UDP",
## "TCP INCOMING"
AT+QIRD=<connectID>[,<read_length>]
## Response
If the specified connection has received the data:
+QIRD: <read_actual_length><CR><LF><data>

## OK

If there is no data:
## +QIRD: 0

## OK

If the connection does not exist:
## ERROR
## Write Command
When <service_type> is
## "UDP SERVICE"
AT+QIRD=<connectID>[,<read_length>]
## Response
If data exists:
+QIRD:  <read_actual_length>,<remoteIP>,<remote_po
rt><CR><LF><data>



LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                                     32 / 56



## Parameter

2.3.10. AT+QISENDEX    Send Hex String Data
This command sends hex string data and cannot be applied to "UDP SERVICE" and "TCP LISTENER"
sockets.
## OK

If there is no data:
## +QIRD: 0

## OK

If the connection does not exist:
## ERROR
## Write Command
When <read_length> is   0,   query   the
retrieved data length
AT+QIRD=<connectID>,0
## Response
If the specified connection exists:
+QIRD: <total_receive_length>,<have_read_length>,<
unread_length>

## OK

If there is any error:
## ERROR
## Maximum Response Time /
## Characteristics /
<connectID>                    Integer type. Socket service index. Range: 0–11.
<read_length>                Integer type. Maximum length of data to be retrieved. Range: 0–1500.
Unit: byte.
<read_actual_length>   Integer type. Length of actually retrieved data. Unit: byte.
<remoteIP>              String type. Remote IP address. It is valid only when <service_type> is "UDP
## SERVICE".
<remote_port>                Integer type. Remote port number. It is valid only when <service_type> is
## "UDP SERVICE".
<data>     Integer type. Retrieved data.
<total_receive_length>  Integer type. Total length of received data. Unit: byte.
<have_read_length>     Integer type. Length of data that has been retrieved. Unit: byte.
<unread_length>              Integer type. Length of data that has not been retrieved. Unit: byte.


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                         33 / 56



## Parameter

2.3.11. AT+QIACCEPT    Manually Accept New TCP Incoming Connection
If <accept_mode> is set  to manually accept a new  TCP  incoming  connection,  after  URC +QIURC:
"accept",<connectID> is returned, the  new  TCP incoming  connection  can  be  accepted  via
## AT+QIACCEPT.
AT+QISENDEX    Send Hex String Data
## Test Command
## AT+QISENDEX=?
## Response
+QISENDEX: (range of supported <connectID>s),<hex_string>

## OK
## Write Command
AT+QISENDEX=<connectID>,<
hex_string>
## Response
If the hex string is sent successfully:
## SEND OK

If the sending buffer is full:
## SEND FAIL

If the connection does not exist:
## ERROR
## Maximum Response Time /
## Characteristics /
<connectID>    Integer type. Socket service index. Range: 0–11.
<hex_string> String type. Hex string data. Max length: 512 bytes.
AT+QIACCEPT  Manually Accept New TCP Incoming Connection
## Test Command
## AT+QIACCEPT=?
## Response
+QIACCEPT:  (range  of  supported <TCP_listener_connec
tID>s),(list  of  supported <accept>s),(range  of  supported <
incoming_connectID>s)

## OK
## Write Command
AT+QIACCEPT=<TCP_listener_conn
ectID>,<accept>[,<incoming_connec
## Response
If <accept> is 0:
## OK


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                                     34 / 56



## Parameter

2.3.12. AT+QISWTMD    Switch Data Access Mode
This command  switches the  data  access  mode among buffer  access  mode,  direct  push  mode  and
transparent transmission mode. When starting a socket service, the data access mode can be specified
via the <access_mode> parameter of AT+QIOPEN. After opening a socket, the data access mode can
be changed via AT+QISWTMD.
tID>]
If <accept> is 1:
## OK

+QIACCEPT: <incoming_connectID>,<remote_addr>,<re
mote_port>

If there is any error:
## ERROR
## Maximum Response Time /
## Characteristics /
<TCP_listener_connectID>       Integer type. ID of "TCP LISTENER". Range: 0–11.
<accept>                                          Integer type. Accept or refuse the incoming connection.
## 0 Refuse
## 1 Accept
<incoming_connectID>              Integer type. ID of the incoming connection to be accepted.
<remote_addr>     String type. Remote client address.
<remote_port>     Integer type. Remote client port. Range: 1–65535.
AT+QISWTMD    Switch Data Access Mode
## Test Command
## AT+QISWTMD=?
## Response
+QISWTMD:  (range  of  supported <connectID>s),(range  of
supported <access_mode>s)

## OK
## Write Command
AT+QISWTMD=<connectID>,<access
## _mode>
## Response
If    data    access    mode    is    switched    successfully    and
<access_mode> is 0 or 1:
## OK



LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                         35 / 56



## Parameter

2.3.13. AT+QPING    Ping a Remote Host
The command tests the reachability of a host on an Internet protocol network. Before using the ping utility,
the host should activate the context of the corresponding <contextID> via AT+QIACT. The command
returns the result within <timeout> and the default value of <timeout> is 4 seconds.
If    data    access    mode    is    switched    successfully    and
<access_mode> is 2, the module will enter the intended data
mode:
## CONNECT

If there is any error:
## ERROR
## Maximum Response Time /
## Characteristics /
<connectID>       Integer type. Socket service index. Range: 0–11.
<access_mode>   Integer type. Data access modes of the connection.
0    Buffer access mode
1    Direct push mode
2    Transparent transmission mode
AT+QPING    Ping a Remote Host
## Test Command
## AT+QPING=?
## Response
+QPING: (range of supported <connectID>s),<host>,(range
of supported <timeout>s),(range of supported <pingnum>s)

## OK
## Write Command
AT+QPING=<contextID>,<host>[,<ti
meout>[,<pingnum>]]
## Response
If a remote server is pinged successfully:
## OK

[+QPING: <ping_result>[,<IP_address>,<bytes>,<time>,<
ttl>]<CR><LF>...]

## [...]

+QPING: <finresult>[,<sent>,<rcvd>,<lost>,<min>,<ma


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                                     36 / 56



## Parameter

2.3.14. AT+QNTP    Synchronize Local Time with NTP Server
This command synchronizes the local time with Universal Time Coordinated (UTC) through the NTP server.
Before  time  synchronization,  the host  should  activate  the  context  corresponding  to <contextID> via
AT+QIACT. Depending on the network, it will take maximum 125 seconds to return the result.
x>,<avg>]

If there is any error:
## ERROR
## Maximum Response Time /
## Characteristics /
<contextID>   Integer type. Context ID. Range: 1–15.
<host> String type. Host address. It is a domain name or a dotted decimal IP address.
<timeout>  Integer type. Maximum time to wait for the response of each ping request.
Range: 1–255. Default value: 4. Unit: second.
<pingnum> Integer type. Maximum number of times for sending ping request. Range: 1–10.
Default value: 4.
<ping_result> Integer type. Result of each ping request.
0    Received the ping response from the host. In this case, it is followed
by ,<IP_address>,<bytes>,<time>,<ttl>.
Other values  Err code. See Chapter 4 for details.
<IP_address> String type. IP address of the remote host formatted as a dotted decimal IP.
<bytes>   Integer type. Length of each sent ping request. Unit: byte.
<time>         Integer type. Waiting time for the response to ping request. Unit: ms.
<ttl>   Integer type. Time to live value of the response packet for the ping request.
<finresult>  Integer type. Final result of the command.
0    Pinged successfully. It succeeded in activating the context and finding
the host. In this case, it is followed by
## <sent>,<rcvd>,<lost>,<min>,<max>,<avg>.
Other values  Err code. See Chapter 4 for details.
<sent>   Integer type. Total number of sent ping requests.
<rcvd>   Integer type. Total number of the ping requests that received responses.
<lost>         Integer type. Total number of timed out ping requests.
<min>   Integer type. Minimum response time. Unit: ms.
<max>   Integer type. Maximum response time. Unit: ms.
<avg>   Integer type. Average response time. Unit: ms.


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                         37 / 56



## Parameter

2.3.15. AT+QIDNSCFG    Configure Address of DNS Server
This command configures the address of DNS server. Before setting the DNS address, the host must
activate the context of corresponding <contextID> via AT+QIACT.
AT+QNTP    Synchronize Local Time with NTP Server
Test command
## AT+QNTP=?
## Response
+QNTP: (range of supported <connectID>s),<server>,(range of
supported <port>s),(list of supported <autosettime>s)

## OK
Read command
## AT+QNTP?
## Response
If in the process of local time synchronization:
+QNTP: <server>,<port>

## OK
Write command
AT+QNTP=<contextID>,<server>
## [,<port>[,<autosettime>]]
## Response
If the local time is synchronized with NTP server successfully:
## OK

+QNTP: <err>,<time>

If there is any error:
## ERROR
Maximum Response Time 125 seconds, determined by the network.
## Characteristics /
<contextID>        Integer type. Context ID. Range: 1–15.
<server>             String type. NTP server address.
<port>            Integer type. NTP server port. Range:1–65535.
<autosettime>   Integer type. Whether to automatically set synchronized time as local time.
0 Do not set
## 1 Set
<err>            Integer type. Err code. See Chapter 4 for details.
<time>               String type. Time synchronized from NTP server.
Format: “YYYY/MM/DD,hh:mm:sszz”. Range of zz: -48 to +56.
AT+QIDNSCFG    Configure Address of DNS Server


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                                     38 / 56



## Parameter

2.3.16. AT+QIDNSGIP    Get IP Address by Domain Name
This command gets an IP address by domain name. Before querying the DNS, the host should activate
the context of corresponding <contextID> via AT+QIACT. Depending on the network, it will take maximum
60 seconds to return the result.
Test command
## AT+QIDNSCFG=?
## Response
+QIDNSCFG: (range  of  supported <connectID>s),<pridnsadd
r>,<secdnsaddr>

## OK
## Write Command
AT+QIDNSCFG=<contextID>[,<pr
idnsaddr>[,<secdnsaddr>]]
## Response
If <pridnsaddr> and <secdnsaddr> are omitted, query the current
DNS server addresses of a specified PDP context:
+QIDNSCFG: <contextID>,<pridnsaddr>,<secdnsaddr>

## OK

If <pridnsaddr> and <secdnsaddr> are specified,  configure  the
primary and secondary DNS server addresses of a specified PDP
context:
## OK

If there is any error:
## ERROR
## Maximum Response Time /
## Characteristics /
<contextID>         Integer type. PDP context ID. Range: 1–15.
<pridnsaddr>         String type. Primary DNS server address.
<secdnsaddr>      String type. Secondary DNS server address.
.AT+QIDNSGIP    Get IP Address by Domain Name
## Test Command
## AT+QIDNSGIP=?
## Response
+QIDNSGIP: (range of supported <connectID>s),<hostname>

## OK
## Write Command
AT+QIDNSGIP=<contextID>,<ho
## Response
## OK


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                         39 / 56



## Parameter

2.3.17. AT+QISDE    Control Whether to Echo Data for AT+QISEND
This command controls whether to echo the data for AT+QISEND.
stname>
If there is any error:
## ERROR

The result will be returned as URC.
+QIURC: "dnsgip",<err>,<IP_count>,<DNS_ttl>
## [.....
+QIURC: "dnsgip",<hostIPaddr>]
Maximum Response Time 60 seconds, determined by the network.
## Characteristics /
<contextID>  Integer type. PDP context ID. Range: 1–15.
<hostname>  String type. Domain name.
<err>       Integer type. Err code. See Chapter 4 for details.
<IP_count>  Integer type. Number of IP addresses corresponding to <hostname>.
<DNS_ttl>  Integer type. Time to live of the DNS. Unit: second.
<hostIPaddr>   String type. IP address of <hostname>.
AT+QISDE    Control Whether to Echo Data for AT+QISEND
## Test Command
## AT+QISDE=?
## Response
+QISDE: (list of supported <echo>s)

## OK
## Read Command
## AT+QISDE?
## Response
+QISDE: <echo>

## OK
## Write Command
AT+QISDE=<echo>
## Response
## OK

If there is any error:
## ERROR
## Maximum Response Time /


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                                     40 / 56



## Parameter

2.3.18. AT+QIGETERROR    Query Last Error Code
If ERROR is returned after executing TCP/IP commands, the detailed information about an error code can
be queried via AT+QIGETERROR. Please note that AT+QIGETERROR just returns the error code of the
last TCP/IP AT command.
## Parameter

2.4. Description of URCs

The URC of TCP/IP AT commands will be reported to the host in the format that begins with +QIURC:. It
contains the reports about incoming data, closed connection and incoming connection and so on. Actually,
there is <CR><LF> both before and after URC, but <CR><LF> is intentionally not presented.

## Characteristics /
<echo>  Numeric type. Whether to echo the data for AT+QISEND.
0  Do not echo the data
1  Echo the data
AT+QIGETERROR    Query Last Error Code
Test command
## AT+QIGETERROR=?
## Response
## OK
## Execution Command
## AT+QIGETERROR
## Response
+QIGETERROR: <err>,<err_code_description>

## OK
## Maximum Response Time /
## Characteristics /
<err>                           Integer type. Error codes of the operation. See Chapter 4 for details.
<err_code_description>   String type. String parameter indicates the details of error information. See
Chapter 4 for details.


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                         41 / 56



2.4.1. +QIURC: "closed"    Indicate Closed Connection
When TCP socket service is closed by remote peer or due to network error, the URC will be outputted,
and the status of socket service will be “closing” (<socket_state>=4). AT+QICLOSE=<connectID> can
be used to change the <socket_state> to initial.
## Parameter

2.4.2. +QIURC: "recv"   Indicate Incoming Data
In buffer access mode or direct push mode, after receiving data, the module reports a URC to the host.

In buffer access mode, after receiving data, the module reports +QIURC: "recv",<connectID> to notify
the host. Then host can retrieve data via AT+QIRD. If the buffer is not empty, and the module receives
data again, it does not report a new URC until all the received data have been retrieved with AT+QIRD
from the buffer.

In direct push mode, the received data are outputted to COM port directly.
## Parameter
+QIURC: "closed"    Indicate Closed Connection
+QIURC: "closed",<connectID> Socket service connection is closed.
<connectID>     Integer type. Socket service index. Range: 0–11.
+QIURC: "recv"   Indicate Incoming Data
+QIURC: "recv",<connectID> Indicates incoming data in buffer access mode. The host can
retrieve data via AT+QIRD.
+QIURC: "recv",<connectID>,<curre
ntrecvlength><CR><LF><data>
Indicates  the incoming  data  in  direct  push  mode  when
<service_type> is "TCP", "UDP", "UPD INCOMING" or "TCP
## INCOMING".
+QIURC: "recv",<connectID>,<curre
ntrecvlength>,<remoteIP>,<remote_
port><CR><LF><data>
Indicates data    incoming    in    direct    push    mode    when
<service_type> is "UDP SERVICE".
<connectID>           Integer type. Socket service index. Range: 0–11.
<currentrecvlength>  Integer type. Length of actually received data. Range: 0-1500. Unit:byte.
<remoteIP>                         String type. Remote IP address.
<remote_port>                   Integer type. Remote port number.


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                                     42 / 56




2.4.3. +QIURC: "incoming full"   Indicate Incoming Connection Reached Limit
If the incoming connection reaches the limit, or no socket system resources can be allocated, then the
module will report the URC as +QIURC: "incoming full" for the new incoming connection request.

2.4.4. +QIURC: "incoming"  Indicate Incoming Connection in Automatic
## Acceptance Mode
If <service_type> is "TCP  LISTENER",  when a remote  client  connects to  this server, and  if
<accept_mode> is automatic acceptance, the module will automatically assign a free <connectID> for
the new connection. The range of <connectID> is 0 to 11. In such a case, the module will report the URC.
<service_type> of the new connection will be "TCP INCOMING", and <access_mode> will be assigned
as <access mode> of "TCP LISENTER".
## Parameter


<data>                                  Received data. Unit: byte.
+QIURC: "incoming full"   Indicate Incoming Connection Reached Limit
+QIURC: "incoming full"
Indicates  the number  of incoming connections  has  reached  the
limit.
+QIURC: "incoming"  Indicate Incoming Connection in Automatic Acceptance Mode
+QIURC: "incoming",<connectI
D>,<serverID>,<remoteIP>,<re
mote_port>
When the new incoming connection is accepted by <serverID>, the
allocated <connectID>, <remoteIP> and <remote_port> are
informed by this URC.
<connectID> Integer type. Index of the socket service assigned to the incoming connection, which is
automatically specified by the module. Range: 0–11.
<serverID>       Integer  type. It is  valid  only when <service_type> is "TCP  INCOMING".
<serverID> represents which server accepts this TCP incoming connection, and the
value is the same as <connectID>  when the <service_type> is "TCP LISTENER".
<remoteIP>         String type. Remote IP address of the incoming <connectID>.
<remote_port>  Integer type. Remote port number of the incoming <connectID>.


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                         43 / 56



2.4.5. +QIURC: "accept"  Indicate Incoming Connection in Manual Acceptance
## Mode
If  the <service_type> is  "TCP  LISTENER",  when a remote  client  connects to  this  server,  and  if  the
<accept_mode> is manual acceptance, the module will report the URC.
## Parameter

2.4.6. +QIURC: "pdpdeact"   Indicate PDP Deactivation
PDP context may be deactivated by the network. The module reports the URC to the host about PDP
deactivation. In this case, the host must execute AT+QIDEACT to deactivate the context and reset all
connections.
## Parameter


+QIURC: "accept"  Indicate Incoming Connection in Manual Acceptance Mode
+QIURC: "accept",<connectI
## D>
Indicates  an  incoming  connection  and  you  can  execute
AT+QIACCEPT to accept or refuse the incoming connection.
<connectID> Integer type. ID of "TCP LISTENER". Range: 0–11.
+QIURC: "pdpdeact"   Indicate PDP Deactivation
+QIURC: "pdpdeact",<contextID> <contextID> context is deactivated.
<contextID>       Integer type. Context ID. Range: 1–15.


LTE  Standard  Module  Series



EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                            44 / 56



## 3  Examples

3.1. Configure and Activate a Context
3.1.1. Configure a Context

3.1.2. Activate a Context

3.1.3. Deactivate a Context

3.2. TCP Client Works in Buffer Access Mode under IPv4
3.2.1. Set up a TCP Client Connection and Enter Buffer Access Mode
AT+QICSGP=1,1,"UNINET","","",1       //Configure context 1. APN is "UNINET" for China Unicom.
## OK
AT+QIACT=1                                                 //Activate context 1. Depending on the network, the maximum
response time is 150 s.
OK                                                                  //Activated the context successfully.
AT+QIACT?                                              //Query  the  context state, protocol  type  and IP  address  of
context 1
## +QIACT: 1,1,1,"10.7.157.1"

## OK
AT+QIDEACT=1                                             //Deactivate context 1.
OK                                                              //Deactivated the context successfully. Depending on the
network, the maximum response time is 40 s.
AT+QIOPEN=1,0,"TCP","192.0.2.2",8009,0,0      //Context is 1 and <connectID> is 0. Before
using AT+QIOPEN, the host should activate


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                         45 / 56




3.2.2. Send Data in Buffer Access Mode

3.2.3. Retrieve Data from Remote Server in Buffer Access Mode
the context with AT+QIACT first.
## OK

+QIOPEN: 0,0                                                        //TCP client is connected successfully. It is
suggested  to  wait  for  150  s  for  the  URC
+QIOPEN:  <connectID>,<err>.  If  the  URC
cannot be received in 150 s, the host could
use AT+QICLOSE to close the socket.
AT+QISTATE=1,0                                                                  //Query connection status of socket service 1.
+QISTATE: 0,"TCP","192.0.2.2",8009,65514,2,1,0,0,"usbmodem"

## OK
AT+QISEND=0                                    //Send variable-length data.
> test1<ctrl+Z>
SEND OK                                             //SEND OK does not mean the data has been sent to the server
successfully. The host can query whether the data has reached the
server via AT+QISEND=0,0.

AT+QISEND=0,4                                //Send fixed-length data and the data length is 4 bytes.
> test
## SEND OK
AT+QISEND=0,0                               //Query the length of sent data, acknowledged data and
unacknowledged data.
## +QISEND: 9,9,0

## OK
AT+QISENDEX=0,"3132333435"   //Send Hex string data.
## SEND OK
AT+QISEND=0,0                               //Query the length of sent data, acknowledged data and
unacknowledged data.
## +QISEND: 14,14,0

## OK
+QIURC: "recv",0                    //The received data when <connectID>=0.

AT+QIRD=0,1500                    //Retrieve data, and the maximum length of data to be retrieved is 1500


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                                     46 / 56




3.2.4. Close a Connection

3.3. TCP Client Works in Transparent Transmission Mode
3.3.1. Set up a TCP Client Connection and Enter Transparent Transmission Mode

bytes.
+QIRD: 5                                      //The length of actually retrieved data is 5 bytes.
test1

## OK
AT+QICFG="recvind",1
## OK
+QIURC: "recv",0,5                 //Received data is 5 bytes when <connectID>=0.
AT+QIRD=0,1500                    //Retrieve data, and the length is 1500 bytes.
+QIRD: 5                                      //The length of actual received data is 5 bytes.
test1

## OK
## AT+QIRD=0,1500
+QIRD: 0                                    //No data in buffer.

## OK
AT+QIRD=0,0                           //Query the total length of received data, including read and unread data.
## +QIRD: 10,10,0

## OK
AT+QICLOSE=0                       //Close  a  connection  whose <connectID> is  0.  Depending  on  the
network, the maximum response time is 10 s by default.
## OK
AT+QIOPEN=1,0,"TCP","192.0.2.2",8009,0,2  //Context  is  1  and <connectID> is  0. Before
using AT+QIOPEN, the host should activate the
context with AT+QIACT.
CONNECT                                      //TCP  client  is  connected successfully.  It  is
suggested  to  wait for 150  s  for  the  URC
CONNECT.  If  the  URC cannot  be received  in
150 s, the host could use AT+QICLOSE to close
the socket.


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                         47 / 56



3.3.2. Send Data in Transparent Transmission Mode

3.3.3. Retrieve Data from Remote Server in Transparent Transmission Mode

3.3.4. Close a TCP Client

3.4. TCP Client Works in Direct Push Mode
3.4.1. Set up a TCP Client Connection and Enter Direct Push Mode

<All data received from COM port will be sent directly to the Internet>
Test 1                                                                          //All data received from the Internet are outputted via
COM port directly.
AT+QICLOSE=0                                                    //After using +++ to exit the transparent transmission
mode, the host could use AT+QICLOSE to close the
TCP link. Depending on the network, the maximum
response time is 10 s by default.
## OK
AT+QIOPEN=1,0,"TCP","192.0.2.2",8009,0,1   //Context  is  1  and <connectID> is  0. Before
using AT+QIOPEN, the host should activate
the context via AT+QIACT.
## OK

+QIOPEN: 0,0                                                              //TCP  client  is  connected successfully.  It  is
suggested  to  wait  for  150  s  for  the  URC
+QIOPEN:  <connectID>,<err>.  If  the  URC
cannot be received in 150 s, the host could use
AT+QICLOSE to close the socket.
AT+QISTATE=1,0                                                             //Query if the connection state socket service 0.
+QISTATE: 0,"TCP","192.0.2.2",8009,65344,2,1,0,1,"usbmodem"

## OK


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                                     48 / 56



3.4.2. Send Data in Direct Push Mode

3.4.3. Receive Data from Remote Server in Direct Push Mode

3.4.4. Close a TCP Client

3.5. TCP Server Works in Buffer Access Mode under IPv4
3.5.1. Start a TCP Server
AT+QISEND=0                                                    //Send variable-length data.
> test1<ctrl+Z>
SEND OK                                                                 //SEND OK does not mean the data has been sent to
the server successfully. Host can query whether the
data has reached the server via AT+QISEND=0,0.
AT+QISEND=0,5                                                    //Send fixed-length data and the data length is 5 bytes.
> test2
## SEND OK
AT+QISEND=0,0                                                    //Query the length of sent data, acknowledged data and
unacknowledged data.
+QISEND: 10,10,0        //A total of 10 bytes of data have been sent, and all 10
bytes have been acknowledged.

## OK
+QIURC: "recv",0,4                                             //Retrieve data from remote server.
test
AT+QICLOSE=0                                         //Close  the  connection  whose <connectID> is  0.
Depending  on  the  network,  the  maximum  response
time is 10 s by default.
## OK
AT+QIOPEN=1,1,"TCP LISTENER","127.0.0.1",0,2020,0 //Context is 1 and <connectID> is 1. Before
using AT+QIOPEN,  the  host  should
activate the context with AT+QIACT.
## OK

+QIOPEN: 1,0                                                                          //TCP server is opened successfully.


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                         49 / 56




3.5.2. Accept TCP Incoming Connection

3.5.3. Retrieve Data from Incoming Connection

3.5.4. Close a TCP Server

AT+QISTATE=0,1                                                         //Query the connection the connection status
of context 1.
+QISTATE: 1,"TCP LISTENER","10.7.157.1",0,2020,3,1,1,0,"usbmodem"

## OK
+QIURC:  "incoming",11,1,"172.31.242.222",54091   //A  new  TCP  connection  is  accepted.
<service_type> is "TCP  incoming",  and
<connectID> is 11.
+QIURC: "recv",11        //Received data from remote incoming connection.
AT+QIRD=11,1500         //Retrieve data received from incoming connection.
+QIRD: 4          //Length of actually retrieved data is 4 bytes.
test

## OK
## AT+QIRD=11,1500
+QIRD: 0                                                                      //No data in buffer.

## OK
AT+QIRD=11,0                               //Query the total length of received data, including
read and unread data.
## +QIRD: 4,4,0

## OK
AT+QICLOSE=11                                                          //Close the incoming connection. Depending on the
network, the maximum response time is 10 s by
default.
## OK
AT+QICLOSE=1                                                             //Close the listening TCP server.
## OK


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                                     50 / 56



3.6. UDP Service under IPv4
3.6.1. Start a UDP Service

3.6.2. Send UDP Data to Remote Client

3.6.3. Retrieve Data from Remote Client
+QIURC: "recv",2                                                              //Received data from remote client.
AT+QIRD=2                                                                        //Retrieve UDP data. One whole UDP packet will
be outputted. There is no need to specify the
read length.
+QIRD: 4,"10.7.76.34",7687                        //Retrieved data length is 4. The remote IP
address is 10.7.76.34 and remote port is 7687.
## AAAA

## OK
AT+QIRD=2                                                                        //Retrieve data.
+QIRD: 0                                                                           //No data in buffer.

## OK
AT+QISEND=2,10,"10.7.76.34",7687                      //Send  data  to the remote client whose  IP  is
10.7.76.34 and remote port is 7687.
## >1234567890
AT+QIOPEN=1,2,"UDP SERVICE","127.0.0.1",0,3030,0 //Start a UDP service whose <connectID> is 2
and <contextID> is  1.  Before  using
AT+QIOPEN,  the  host  should  activate  the
context with AT+QIACT.
## OK

+QIOPEN: 2,0                                                                          //UDP service is opened successfully.
AT+QISTATE=0,1                                                                    //Query the connection status of context 1.
+QISTATE: 2,"UDP SERVICE","10.7.157.1",0,3030,2,1,2,0,"usbmodem"

## OK
AT+QISEND=2,10,"10.7.89.10",6969                 //Send 10 bytes data to remote client whose IP
is 10.7.89.10 and the remote port is 6969.
## >1234567890
## SEND OK


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                         51 / 56



## SEND OK

3.6.4. Close a UDP Service

3.7. PING under IPv4


## 3.8. Synchronize Local Time

AT+QICLOSE=2                                                           //Close the service.
## OK
AT+QPING=1,"www.example.com"                   //Ping www.example.com in context 1. Before pinging
the destination IP address, the host should activate
the context via AT+QIACT.
## OK

## +QPING: 0,"192.0.2.2",32,192,255

## +QPING: 0,"192.0.2.2",32,240,255

## +QPING: 0,"192.0.2.2",32,241,255

## +QPING: 0,"192.0.2.2",32,479,255

## +QPING: 0,4,4,0,192,479,288
AT+QNTP=1,"192.0.2.2",123              //Synchronize  local  time  with  NTP  server "192.0.2.2:123".
Before synchronizing the time, the host should activate the
context with AT+QIACT.
## OK

## +QNTP: 0,"2019/07/21,06:10:59+00"
## AT+CCLK?
## +CCLK: "19/07/21,06:11:05+00"

## OK


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                                     52 / 56



## 3.9. Get Last Error Code


AT+QIOPEN=1,"TCP","192.0.2.2",8009,0,1  //Send AT+QIOPEN with missing <connectID>.
## ERROR
## AT+QIGETERROR
+QIGETERROR: 552, invalid parameters

## OK


LTE  Standard  Module  Series



EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                            53 / 56



4  Summary of Error Codes

If an ERROR is returned after executing TCP/IP AT commands, the detailed information about errors can
be queried via AT+QIGETERROR. Please note that AT+QIGETERROR just returns error code of the last
TCP/IP AT command.

Table 2: Summary of Error Codes
## <err> <errcode_description>
0 operate success
550 Unknown error
551 Operation blocked
552 Invalid parameters
553 Memory not enough
554 Socket creation failed
555 Operation not supported
556 Socket bind failed
557 Socket listen failed
558 Socket write failed
559 Socket read failed
560 Socket accept failed
561 Activate PDP context failed
562 Deactivate PDP context failed
563 Socket identity has been used
564 Dns busy


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                                     54 / 56



565 Dns parse failed
566 Socket connect failed
567 Socket has been closed
568 Operation busy
569 Operation timeout
570 PDP context broken down
571 Cancel sending
572  Operation not allowed
573  APN not configured
574 Port busy


LTE  Standard  Module  Series



EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                            55 / 56



## 5  Appendix  References

Table 3: Terms and Abbreviations
## Abbreviation  Description
3GPP 3rd Generation Partnership Project
ACK Acknowledge
APN Access Point Name
ASCII American Standard Code for Information Interchange
CHAP Challenge Handshake Authentication Protocol
CS Circuit Switching
DNS Domain Name System
DTR Data Terminal Ready
FIN Finish
ID Identifier
IP Internet Protocol
IPv4 Internet Protocol version 4
IPv6 Internet Protocol version 6
LTE Long-Term Evolution
NB-IoT Narrowband Internet of Things
NTP Network Time Protocol
NVM Non-Volatile Memory
PAP Password Authentication Protocol l
PDP Packet Data Protocol
PPP Point-to-Point Protocol


LTE  Standard  Module  Series

EG800Q&EG91xQ_Series_TCP/IP_Application_Note                                                                     56 / 56




PSSACK Packet SwitchingSelective Acknowledgment
PS Packet Switching
QoS Quality of Service
TCP Transmission Control Protocol
TTL Time To Live
UART Universal Asynchronous Receiver& Transmitter
UDP User Datagram Protocol
URC  Unsolicited Result Code
USB Universal Serial Bus
(U)SIM (Universal) Subscriber Identity Module
UTC Coordinated Universal Time