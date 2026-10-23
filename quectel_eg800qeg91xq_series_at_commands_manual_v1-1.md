

<!-- Start of picture text -->
QUECCTEL<br><!-- End of picture text -->

## **EG800Q&EG91xQ Series AT Commands Manual** 

**LTE Standard Module Series** 

Version: 1.1 

Date: 2024-11-03 

Status: Released 

i 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

**At Quectel, our aim is to provide timely and comprehensive services to our customers. If you require any assistance, please contact our headquarters:** 

###### **Quectel Wireless Solutions Co., Ltd.** 

Building 5, Shanghai Business Park Phase III (Area B), No.1016 Tianlin Road, Minhang District, Shanghai 200233, China 

Tel: +86 21 5108 6236 

Email: info@quectel.com 

**Or our local offices. For more information, please visit:** <u>http://www.quectel.com/support/sales.htm.</u> 

**For technical support, or to report documentation errors, please visit:** <u>http://www.quectel.com/support/technical.htm.</u> Or email us at: support@quectel.com. 

#### **Legal Notices** 

We offer information as a service to you. The provided information is based on your requirements and we make every effort to ensure its quality. You agree that you are responsible for using independent analysis and evaluation in designing intended products, and we provide reference designs for illustrative purposes only. Before using any hardware, software or service guided by this document, please read this notice carefully. Even though we employ commercially reasonable efforts to provide the best possible experience, you hereby acknowledge and agree that this document and related services hereunder are provided to you on an “as available” basis. We may revise or restate this document from time to time at our sole discretion without any prior notice to you. 

#### **Use and Disclosure Restrictions** 

##### **License Agreements** 

Documents and information provided by us shall be kept confidential, unless specific permission is granted. They shall not be accessed or used for any purpose except as expressly provided herein. 

##### **Copyright** 

Our and third-party products hereunder may contain copyrighted material. Such copyrighted material shall not be copied, reproduced, distributed, merged, published, translated, or modified without prior written consent. We and the third party have exclusive rights over copyrighted material. No license shall be granted or conveyed under any patents, copyrights, trademarks, or service mark rights. To avoid ambiguities, purchasing in any form cannot be deemed as granting a license other than the normal non-exclusive, royalty-free license to use the material. We reserve the right to take legal action for noncompliance with abovementioned requirements, unauthorized use, or other illegal or malicious use of the material. 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    1 / 207** 

**LTE Standard Module Series** 



##### **Trademarks** 

Except as otherwise set forth herein, nothing in this document shall be construed as conferring any rights to use any trademark, trade name or name, abbreviation, or counterfeit product thereof owned by Quectel or any third party in advertising, publicity, or other aspects. 

##### **Third-Party Rights** 

This document may refer to hardware, software and/or documentation owned by one or more third parties (“third-party materials”). Use of such third-party materials shall be governed by all restrictions and obligations applicable thereto. 

We make no warranty or representation, either express or implied, regarding the third-party materials, including but not limited to any implied or statutory, warranties of merchantability or fitness for a particular purpose, quiet enjoyment, system integration, information accuracy, and non-infringement of any third-party intellectual property rights with regard to the licensed technology or use thereof. Nothing herein constitutes a representation or warranty by us to either develop, enhance, modify, distribute, market, sell, offer for sale, or otherwise maintain production of any our products or any other hardware, software, device, tool, information, or product. We moreover disclaim any and all warranties arising from the course of dealing or usage of trade. 

#### **Privacy Policy** 

To implement module functionality, certain device data are uploaded to Quectel’s or third-party’s servers, including carriers, chipset suppliers or customer-designated servers. Quectel, strictly abiding by the relevant laws and regulations, shall retain, use, disclose or otherwise process relevant data for the purpose of performing the service only or as permitted by applicable laws. Before data interaction with third parties, please be informed of their privacy and data security policy. 

#### **Disclaimer** 

- a) We acknowledge no liability for any injury or damage arising from the reliance upon the information. b) We shall bear no liability resulting from any inaccuracies or omissions, or from the use of the information contained herein. 

- c) While we have made every effort to ensure that the functions and features under development are free from errors, it is possible that they could contain errors, inaccuracies, and omissions. Unless otherwise provided by valid agreement, we make no warranties of any kind, either implied or express, and exclude all liability for any loss or damage suffered in connection with the use of features and functions under development, to the maximum extent permitted by law, regardless of whether such loss or damage may have been foreseeable. 

- d) We are not responsible for the accessibility, safety, accuracy, availability, legality, or completeness of information, advertising, commercial offers, products, services, and materials on third-party websites and third-party resources. 

**_Copyright © Quectel Wireless Solutions Co., Ltd. 2024. All rights reserved._** 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    2 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

### **About the Document** 

#### **Revision History** 

|**Version**|**Date**|**Author**|**Description**|
|---|---|---|---|
|-|2022-09-15|Bronson ZHAN/<br>Obuntu QIN|Creation of the document|
|1.0|2023-02-21|Bronson ZHAN/<br>Obuntu QIN|First official release|
||||1.<br>Updated the applicable modules:<br>⚫<br>Added applicable module EG916Q-GL;<br>⚫<br>Updated EG800Q-EU to EG800Q Series;<br>⚫<br>Updated EG915Q-NA to EG915Q Series.<br>2.<br>Updated the declaration of AT command examples<br>(Chapter 1.7).<br>3.<br>Updated the AT&V response (Chapter 2.11)<br>4.<br>Added the default value of <urc_port_value> in<br>AT+QURCCFG (Chapter 2.24).<br>5.<br>Added AT+QPPPDROOP (Chapter 2.26).<br>6.<br>Updated the value of <rate> in AT+IPR (Chapter<br>3.5).|
|1.1|2024-11-03|Obuntu QIN|7.<br>Updated the description of AT+CEER (Chapter<br>4.2).<br>8.<br>Added the following commands (Chapters 4.3 &<br>4.4):<br>AT+QINDCFG="datastatus"<br>AT+QINDCFG="mode"<br>AT+QINDCFG="sqi"<br>AT+QCFG="urc/ri/level"<br>AT+QCFG="cmux/urcport"<br>AT+QCFG="cmux/baudratework"<br>AT+QCFG="aprstlevel"<br>AT+QCFG="modemrstlevel"<br>AT+QCFG="ledmode"<br>AT+QCFG="nat"|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    3 / 207** 



   - AT+QCFG="ppp/termframe" AT+QCFG="rf/tuner_cfg" 

   - AT+QCFG="fast/poweroff" 

   - AT+QCFG="fota/cid" 

   - AT+QCFG="qcautoconnect" AT+QCFG="sms/listmsgmap" 

   - AT+QCFG="sms/MeNum" 

9. Updated AT+QCFG="urc/ri/sms" to AT+QCFG="urc/ri/smsincoming" (Chapter 4.4.2). 

10. Updated the value of <ltebandval> in AT+QCFG="band" (Chapter 4.4.12). 

11. Updated the default value of <interval> in AT+QCFG="nwoptmz/acq" (Chapter 4.4.13). 

12. Updated the response of the write command AT+CPIN=<pin>[,<new_pin>] (Chapter 5.3). 

13. Updated the value of <band> in AT+QNWINFO (Chapter 6.9). 

14. Deleted ATA and ATH commands. 

15. Updated ATD command (Chapter 7.1). 

16. Deleted “MT” of <mem1>, <mem2> and <mem3> (Chapter 8.4–Chapter 8.7). 

17. Specified the default value of <stat> in AT+CMGL (Chapter 8.6). 

18. Added AT+QNETDEVCTL (Chapter 9.13). 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    4 / 207** 

**LTE Standard Module Series** 



##### **Contents** 

|**About the**|**Document ................................................................................................................................... 3**|
|---|---|
|**Contents .**|**...................................................................................................................................................... 5**|
|**Table Ind**|**ex ................................................................................................................................................... 9**|
|**1**<br>**Introd**|**uction ........................................................................................................................................ 10**|
|1.1.|Definitions ................................................................................................................................ 10|
|1.2.|AT Command Syntax ............................................................................................................... 10|
|1.3.|Supported Character Sets ....................................................................................................... 11|
|1.4.|AT Command Interfaces .......................................................................................................... 12|
|1.5.|Unsolicited Result Code .......................................................................................................... 12|
|1.6.|Turn-off Procedure ................................................................................................................... 12|
|1.7.|Declaration of AT Command Examples ................................................................................... 12|
|**2**<br>**Gene**|**ral Commands ........................................................................................................................... 13**|
|2.1.|ATI  Display MT Identification ................................................................................................ 13|
|2.2.|AT+GMI  Request Manufacturer Identification ...................................................................... 14|
|2.3.|AT+GMM  Request TA Model Identification ........................................................................... 14|
|2.4.|AT+GMR  Request TA Firmware Revision Identification ....................................................... 15|
|2.5.|AT+CGMI  Request Manufacturer Identification .................................................................... 15|
|2.6.|AT+CGMM  Request MT Model Identification ....................................................................... 16|
|2.7.|AT+CGMR  Request MT Firmware Revision Identification ................................................... 16|
|2.8.|AT+GSN  Request IMEI and SN ............................................................................................ 17|
|2.9.|AT+CGSN  Request IMEI and SN ......................................................................................... 18|
|2.10.|AT&F  Reset AT Command Settings to Factory Defaults ...................................................... 19|
|2.11.|AT&V  Display Current Configuration .................................................................................... 20|
|2.12.|AT&W  Store Current AT Command Settings to User-Defined Profile .................................. 20|
|2.13.|ATZ  Restore All AT Command Settings from User-Defined Profile ...................................... 21|
|2.14.|ATQ  Set Result Code Presentation Mode ............................................................................ 22|
|2.15.|ATV  AT Response Format .................................................................................................... 22|
|2.16.|ATE  Set Command Echo Mode ............................................................................................ 24|
|2.17.|ATS3  Set Command Line Termination Character ................................................................. 24|
|2.18.|ATS4  Set Response Formatting Character .......................................................................... 25|
|2.19.|ATS5  Set Command Line Editing Character ........................................................................ 26|
|2.20.|ATX  Set CONNECT Result Code Format and Monitor Call Progress ................................. 26|
|2.21.|AT+CFUN  Set UE Functionality ............................................................................................ 27|
|2.22.|AT+CMEE  Error Message Format ........................................................................................ 29|
|2.23.|AT+CSCS  Select TE Character Set ...................................................................................... 30|
|2.24.|AT+QURCCFG  Configure URC Output Port ........................................................................ 31|
|2.25.|AT+QDIAGPORT  Configure Debug UART as AT Port ......................................................... 32|
|2.26.|AT+QPPPDROOP  Terminate PPP Connection .................................................................... 33|
|**3**<br>**Serial**|**Interface Control Commands ................................................................................................ 35**|
|3.1.|AT&C  Set DCD Function Mode ............................................................................................ 35|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    5 / 207** 

**LTE Standard Module Series** 



|3.2.<br>AT&|D  Set DTR Function Mode ............................................................................................. 35|
|---|---|
|3.3.<br>AT+I|FC  Set TE-TA Local Data Flow Control ......................................................................... 36|
|3.4.<br>AT+I|CF  Set TE-TA Control Character Framing .................................................................... 37|
|3.5.<br>AT+I|PR  Set TE-TA Fixed Local Rate .................................................................................... 38|
|**4**<br>**Status Cont**|**rol Commands ............................................................................................................... 40**|
|4.1.<br>AT+|CPAS  Mobile Equipment Activity Status ........................................................................ 40|
|4.2.<br>AT+|CEER  Extended Error Report ........................................................................................ 41|
|4.3.<br>AT+|QINDCFG  Configure URC Indication ............................................................................. 42|
|4.4.<br>AT+|QCFG  Extended Configuration Settings ........................................................................ 45|
|4.4.1.|AT+QCFG="urc/ri/ring"  Set RI Behavior When RING URC is Presented .................. 46|
|4.4.2.|AT+QCFG="urc/ri/smsincoming"  Set RI Behavior When Incoming SMS URCs are|
||Presented ....................................................................................................................... 47|
|4.4.3.|AT+QCFG="urc/ri/other"  Set RI Behavior When Other URCs are Presented ............ 48|
|4.4.4.|AT+QCFG="urc/ri/level"  Set Default Level of MAIN_RI Pin ....................................... 49|
|4.4.5.|AT+QCFG="cmux/urcport"  Set Output Port of CMUX URCs ..................................... 50|
|4.4.6.|AT+QCFG="cmux/baudratework"  Set Baud Rate in CMUX Mode............................. 50|
|4.4.7.|AT+QCFG="urc/delay"  Set Delay Time of URC Indication ......................................... 51|
|4.4.8.|AT+QCFG="urc/cache"  Enable/Disable URC Cache ................................................. 52|
|4.4.9.|AT+QCFG="urc/port"  Set URC Output Port ............................................................... 53|
|4.4.10.|AT+QCFG="risignaltype"  RI Signal Output Carrier .................................................... 54|
|4.4.11.|AT+QCFG="roamservice"  Enable/Disable Roaming Service ..................................... 55|
|4.4.12.|AT+QCFG="band"  Configure Band ............................................................................ 56|
|4.4.13.|AT+QCFG="nwoptmz/acq"  Save Searched Cell Database to NV Automatically ....... 57|
|4.4.14.|AT+QCFG="usbnet"  Set Network Card Interface Type .............................................. 59|
|4.4.15.|AT+QCFG="aprstlevel"  Set AP Dump Level ............................................................... 59|
|4.4.16.|AT+QCFG="modemrstlevel"  Set CP Dump Level ...................................................... 60|
|4.4.17.|AT+QCFG="ntp"  Set Maximum NTP Re-transmission Count and Interval ................ 61|
|4.4.18.|AT+QCFG="ledmode"  Set NET_STATUS Output Mode ............................................ 62|
|4.4.19.|AT+QCFG="nat"  Set NIC Working Mode  .................................................................. 63|
|4.4.20.|AT+QCFG="netmaskset"  Enable/Disable Customized Netmask ............................... 63|
|4.4.21.|AT+QCFG="ppp/termframe"  Enable/Disable PPP TERM Frame Sending ................ 64|
|4.4.22.|AT+QCFG="rf/tuner_cfg"  Set Mapping Between RF Tuner and RF Bands ............... 66|
|4.4.23.|AT+QCFG="tcp/windowsize"  Set TCP Window Available Size .................................. 67|
|4.4.24.|AT+QCFG="TCP/SendMode"  Set TCP Sending Mode .............................................. 68|
|4.4.25.|AT+QCFG="fast/poweroff"  Enable/Disable Fast Power-Off ....................................... 69|
|4.4.26.|AT+QCFG="fota/cid"  Set PDP Context ID for FOTA Download ................................. 70|
|4.4.27.|AT+QCFG="qcautoconnect"  Disable/enable Auto Connect Function in ECM Mode . 70|
|4.4.28.|AT+QCFG="sms/listmsgmap"  List Message Map ...................................................... 71|
|4.4.29.|AT+QCFG="sms/MeNum"  Set the Number of Short Messages Stored by ME ......... 72|
|**5**<br>**(U)SIM Rela**|**ted Commands ............................................................................................................... 74**|
|5.1.<br>AT+|CIMI  Request International Mobile Subscriber Identity (IMSI) ....................................... 74|
|5.2.<br>AT+|CLCK  Facility Lock ......................................................................................................... 75|
|5.3.<br>AT+|CPIN  Enter PIN .............................................................................................................. 76|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    6 / 207** 

**LTE Standard Module Series** 



|5.4.|AT+CPWD  Change Password .............................................................................................. 78|
|---|---|
|5.5.|AT+CSIM  Generic (U)SIM Access ........................................................................................ 79|
|5.6.|AT+CRSM  Restricted (U)SIM Access ................................................................................... 80|
|5.7.|AT+QCCID  Show ICCID ....................................................................................................... 81|
|5.8.|AT+QPINC  Display PIN Remainder Counter........................................................................ 82|
|5.9.|AT+QINISTAT  Query Initialization Status of (U)SIM Card .................................................... 83|
|5.10.|AT+QSIMDET  (U)SIM Card Detection.................................................................................. 84|
|5.11.|AT+QSIMSTAT  (U)SIM Card Insertion Status Report .......................................................... 85|
|5.12.|AT+CCHO  Open Logical Channel ........................................................................................ 86|
|5.13.|AT+CGLA  UICC Logical Channel Access ............................................................................. 87|
|5.14.|AT+CCHC  Close Logical Channel ........................................................................................ 88|
|**6**<br>**Netw**|**ork Service Commands ............................................................................................................ 90**|
|6.1.|AT+COPS  Operator Selection .............................................................................................. 90|
|6.2.|AT+CREG  Network Registration Status ............................................................................... 92|
|6.3.|AT+CSQ  Signal Quality Report ............................................................................................. 94|
|6.4.|AT+CPOL  Preferred Operator List ........................................................................................ 95|
|6.5.|AT+COPN  Read Operator Names ........................................................................................ 96|
|6.6.|AT+CTZU  Automatic Time Zone Update .............................................................................. 97|
|6.7.|AT+CTZR  Time Zone Reporting ........................................................................................... 98|
|6.8.|AT+QLTS  Obtain the Latest Time Synchronized Through Network ................................... 100|
|6.9.|AT+QNWINFO  Query Network Information ........................................................................ 102|
|6.10.|AT+QSPN  Display Name of Registered Network ............................................................... 103|
|6.11.|AT+QENG  Switch on/off Engineering Mode ....................................................................... 104|
|6.12.|AT+CIND  Command of Control Indicator ........................................................................... 107|
|**7**<br>**Dial R**|**elated Commands .................................................................................................................. 109**|
|7.1.|ATD  PPP Dial ...................................................................................................................... 109|
|7.2.|+++  Switch from Data Mode to Command Modeg ............................................................. 110|
|7.3.|ATO  Switch from Command Mode to Data Mode .............................................................. 110|
|**8**<br>**Short**|**Message Service Commands ............................................................................................... 112**|
|8.1.|AT+CSMS  Select Message Service ................................................................................... 112|
|8.2.|AT+CMGF  Message Format ............................................................................................... 113|
|8.3.|AT+CSCA  Service Center Address ..................................................................................... 114|
|8.4.|AT+CPMS  Preferred Message Storage .............................................................................. 115|
|8.5.|AT+CMGD  Delete Message ............................................................................................... 117|
|8.6.|AT+CMGL  List Message ..................................................................................................... 118|
|8.7.|AT+CMGR  Read Message ................................................................................................. 122|
|8.8.|AT+CMGS  Send Message .................................................................................................. 126|
|8.9.|AT+CMMS  More Messages to Send .................................................................................. 128|
|8.10.|AT+CMGW  Write Message to Memory............................................................................... 129|
|8.11.|AT+CMSS  Send Message from Storage ............................................................................ 131|
|8.12.|AT+CNMA  New Message Acknowledgement to UE/TE ..................................................... 133|
|8.13.|AT+CNMI  SMS Event Reporting Configuration .................................................................. 134|
|8.14.|AT+CSCB  Select Cell Broadcast Message Types ............................................................. 137|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    7 / 207** 

**LTE Standard Module Series** 



|8.15.|AT+CSDH  Show SMS Text Mode Parameters ................................................................... 138|
|---|---|
|8.16.|AT+CSMP  Set SMS Text Mode Parameters ...................................................................... 139|
|8.17.|AT+QCMGS  Send Concatenated Messages ..................................................................... 140|
|8.18.|AT+QCMGR  Read Concatenated Messages ..................................................................... 142|
|**9**<br>**Packe**|**t Domain Commands ............................................................................................................ 144**|
|9.1.|AT+CGATT  Attachment or Detachment of PS .................................................................... 144|
|9.2.|AT+CGDCONT  Define PDP Context .................................................................................. 145|
|9.3.|AT+CGACT  Activate or Deactivate PDP Context ............................................................... 147|
|9.4.|AT+CGDATA  Enter Data State............................................................................................ 148|
|9.5.|AT+CGPADDR  Show PDP Address ................................................................................... 149|
|9.6.|AT+CGREG  Network Registration Status .......................................................................... 150|
|9.7.|AT+CGEREP  Packet Domain Event Reporting .................................................................. 152|
|9.8.|AT+CEREG  EPS Network Registration Status .................................................................. 154|
|9.9.|AT+QGDCNT  Packet Data Counter ................................................................................... 155|
|9.10.|AT+QAUGDCNT  Auto Save Packet Data Counter ............................................................ 156|
|9.11.|AT+QNETDEVSTATUS  Query USBNet Device Status ...................................................... 158|
|9.12.|AT+CGCONTRDP  PDP Context Read Dynamic Parameters ............................................ 160|
|9.13.|AT+QNETDEVCTL  Make/Disconnect PDP Call ................................................................. 162|
|**10** **Hardw**|**are Related Commands ........................................................................................................ 164**|
|10.1.|AT+QPOWD  Power off ....................................................................................................... 164|
|10.2.|AT+CCLK  Clock .................................................................................................................. 165|
|10.3.|AT+CBC  Battery Charge ..................................................................................................... 166|
|10.4.|AT+QADC  Read ADC Value ............................................................................................... 167|
|10.5.|AT+QSCLK  Enable/Disable Low Power Mode ................................................................... 168|
|**11** **Other**|**Related Commands ............................................................................................................... 169**|
|11.1.|GNSS Related AT Commands (Not Supported Currently) .................................................... 169|
|11.2.|DFOTA Related AT Command ............................................................................................... 170|
|11.3.|FTP(S) Related AT Commands ............................................................................................. 170|
|11.4.|HTTP(S) Related AT Commands .......................................................................................... 171|
|11.5.|TCP(IP) Related AT Commands ............................................................................................ 172|
|11.6.|SSL Related AT Commands .................................................................................................. 173|
|**12** **Appe**|**ndix References ...................................................................................................................... 174**|
|12.1.|Factory Default Settings Restorable with AT&F .................................................................... 181|
|12.2.|AT Command Settings Storable with AT&W .......................................................................... 182|
|12.3.|AT Command Settings Storable with ATZ ............................................................................. 183|
|12.4.|Summary of CME ERROR Codes ......................................................................................... 183|
|12.5.|Summary of CMS ERROR Codes ......................................................................................... 185|
|12.6.|Summary of URC................................................................................................................... 187|
|12.7.|SMS Character Sets Conversions ......................................................................................... 189|
|12.8.|Release Cause Text List of AT+CEER .................................................................................. 195|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    8 / 207** 

**LTE Standard Module Series** 



##### **Table Index** 

|Table 1: Type of AT Commands and Responses ........................................................................................11|
|---|
|Table 2: AT&V Response............................................................................................................................ 20|
|Table 3: Numeric Equivalents and Brief Descriptions of ATV0&ATV1 Result Codes ................................ 23|
|Table 4: GNSS Related AT Commands ................................................................................................... 169|
|Table 5: DFOTA Related AT Command .................................................................................................... 170|
|Table 6: FTP(S) Related AT Commands .................................................................................................. 170|
|Table 7: HTTP(S) Related AT Commands ............................................................................................... 171|
|Table 8: TCP(IP) Related AT Commands................................................................................................. 172|
|Table 9: SSL Related AT Commands ....................................................................................................... 173|
|Table 10: Related Documents .................................................................................................................. 174|
|Table 11: Terms and Abbreviations .......................................................................................................... 174|
|Table 12: Factory Default Settings Restorable with AT&F ....................................................................... 181|
|Table 13: AT Command Settings Storable with AT&W ............................................................................. 182|
|Table 14: AT Command Settings Storable with ATZ ................................................................................ 183|
|Table 15: Different Coding Schemes of +CME ERROR: <err> ............................................................... 184|
|Table 16: Different Coding Schemes of +CMS ERROR: <err> ............................................................... 186|
|Table 17: Summary of URC ..................................................................................................................... 187|
|Table 18: The Way of SMS Text Input or Output ...................................................................................... 189|
|Table 19: The Input Conversions Table (DCS = GSM 7 bit and AT+CSCS="GSM") ............................... 189|
|Table 20: The Output Conversions Table (DCS = GSM 7 bit and AT+CSCS="GSM") ............................ 190|
|Table 21: GSM Extended Characters ...................................................................................................... 191|
|Table 22: The Input Conversions Table (DCS = GSM 7 bit and AT+CSCS="IRA") ................................. 192|
|Table 23: IRA Extended Characters ......................................................................................................... 192|
|Table 24: The Output Conversions Table (DCS = GSM 7 bit and AT+CSCS="IRA") .............................. 193|
|Table 25: GSM Extended Characters ...................................................................................................... 194|
|Table 26: List of Location ID List .............................................................................................................. 195|
|Table 27: List of Cause ............................................................................................................................. 195|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    9 / 207** 

**LTE Standard Module Series** 



# **1 Introduction** 

This document gives details of the AT Command Set supported by Quectel EG800Q series and EG91xQ family (EG915Q series and EG916Q-GL) modules. 

##### **1.1. Definitions** 

- ⚫ **<CR>** Carriage return character. 

- ⚫ **<LF>** Line feed character. 

- ⚫ **<...>** Parameter name. Angle brackets do not appear on the command line. 

- ⚫ **[...]** Optional parameter of a command or an optional part of TA information response. 

   - Square brackets do not appear on the command line. When an optional parameter is not given in a command, the new value equals its previous value or the default settings, unless otherwise specified. 

- ⚫ **<u>Underline</u>** Default setting of a parameter. 

##### **1.2. AT Command Syntax** 

All command lines must start with **AT** or **at** and end with **<CR>** . Information responses and result codes always start and end with a carriage return character and a line feed character: **<CR><LF><response><CR><LF>** . Throughout this document, only the commands and responses are presented, while carriage return and line feed characters are deliberately omitted. 

AT commands implemented by EG800Q and EG91xQ family modules can be separated into two categories syntactically: “basic command”, and “extended command”, as listed below: 

- ⚫ **Basic Command** 

- ⚫ Basic command format is **AT<x><n>** , or **AT&<x><n>** , where **<x>** is the command, and **<n>** is/are the argument(s) for that command. An example of this is **ATE<n>** , which tells the DCE (Data Circuit-terminating Equipment) whether received characters should be echoed back to the DTE (Data Terminal Equipment) according to the value of **<n>** . **<n>** is optional and a default will be used if it is omitted. 

- ⚫ **Extended Command** 

- ⚫ These commands can be executed in several modes, as shown in the following table: 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    10 / 207** 

**LTE Standard Module Series** 



**Table 1: Type of AT Commands and Responses** 

|**Command Type**|**Syntax**|**Description**|
|---|---|---|
|||Test the existence of the corresponding|
|Test Command|**AT+<cmd>=?**|command and return information about the<br>type, value, or range of its parameter.|
|Read Command|**AT+<cmd>?**|Check the current parameter value of the<br>correspondingcommand.|
|Write Command|**AT+<cmd>=<p1>[,<p2>[,<p3>[...]]]**|Set user-definable parameter value.|
|Execution Command|**AT+<cmd>**|Return a specific information parameter or<br>perform a specific action.|



Multiple commands can be placed on a single line using a semi-colon ( **;** ) between commands. In such cases, only the first command should have **AT** prefix. Commands can be in upper or lower case. 

When entering AT commands, spaces should be ignored except in the following cases: 

- ⚫ Within quoted strings, where spaces are preserved; 

- ⚫ Within an unquoted string or numeric parameter; 

- ⚫ Within an IP address; 

- ⚫ Within the AT command name up to and including a **=** , **?** or **=?** . 

On input, at least a carriage return is required. A newline character is ignored so it is permissible to use carriage return/line feed pairs on the input. 

If no command is entered after the **AT** token, **OK** will be returned. If an invalid command is entered, **ERROR** will be returned. 

Optional parameters, unless explicitly stated, need to be provided up to the last parameter being entered. 

##### **1.3. Supported Character Sets** 

The AT command interface of LTE standard modules defaults to the **GSM** character set. LTE standard modules support the following character sets: 

- ⚫ GSM format 

- ⚫ UCS2 

- ⚫ IRA 

The character set can be configured and querried with **AT+CSCS** ( _3GPP TS 27.007_ ) and it is defined in 3GPP TS 27.005. The character set affects transmission and reception of SMS and SMS Cell Broadcast 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    11 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

Messages, as well as the entry and display of phone book entry text fields. 

##### **1.4. AT Command Interfaces** 

The main UART port and two USB ports (USB modem port and USB AT port) support AT command communication and data transfer. 

##### **1.5. Unsolicited Result Code** 

Unsolicited Result Code (URC) is not issued in response to an executed AT command, but as a report message issued by the modules without being requested by the TE. It is automatically issued by the module in response to a certain event. Typical URC triggering events include incoming calls (RING), received short messages, etc. 

##### **1.6. Turn-off Procedure** 

The safest and best way to turn off the module is to execute **AT+QPOWD** . This procedure is performed by letting the module log off from the network and allowing the software to enter a secure and safe data state before disconnecting the power supply. 

After sending **AT+QPOWD** , do not enter any other AT commands. Once the command is executed successfully, the module will output **POWERED DOWN** and then enter the power down mode. To avoid data loss, it is suggested to wait for 1s to disconnect the power supply after the URC **POWERED DOWN** is outputted. If **POWERED DOWN** cannot be received within 65 s, the power supply shall be disconnected compulsorily. 

##### **1.7. Declaration of AT Command Examples** 

The AT command examples in this document are provided to help you learn about the use of the AT commands introduced herein. The examples, however, should not be taken as Quectel’s recommendations or suggestions about how to design a program flow or what status to set the module into. Sometimes multiple examples may be provided for one AT command. However, this does not mean that there is a correlation among these examples, or that they should be executed in a given sequence. The URLs, domain names, IP addresses, usernames/accounts, and passwords (if any) in the AT command examples are provided for illustrative and explanatory purposes only, and they should be modified to reflect your actual usage and specific needs. 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    12 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

# **2 General Commands** 

##### **2.1. ATI  Display MT Identification** 

This Execution Command returns the MT identification. 

###### **ATI  Display Product Identification Information** 

Execution Command Response **ATI Quectel <objectID> Revision: <revision> OK** Maximum Response Time 300 ms Characteristics / Reference V.25ter 

###### **Parameter** 

**<objectID>** String type. Device type identifier. **<revision>** String type. Product firmware version identifier. 

###### **Example** 

**<mark>ATI Quectel EG800Q Revision: EG800QEULCR01A03M04 OK</mark>** 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    13 / 207** 

**LTE Standard Module Series** 



##### **2.2. AT+GMI  Request Manufacturer Identification** 

This command returns the manufacturer identification. It is identical to **AT+CGMI** . 

|**AT+GMI  Request Manufacturer Identification**|
|---|
|Test Command<br>Response|
|**AT+GMI=?**<br>**OK**|
|Execution Command<br>**AT+GMI**<br>Response<br>**Quectel**<br>**OK**|
|Maximum Response Time<br>300 ms|
|Characteristics<br>/|
|Reference|
|V.25ter|



##### **2.3. AT+GMM  Request TA Model Identification** 

This command returns the TA model identification. It is identical to **AT+CGMM** in **_Chapter 2.6_** . 

|**AT+GMM  Request TA Model Identification**|
|---|
|Test Command<br>Response|
|**AT+GMM=?**<br>**OK**|
|Execution Command<br>Response|
|**AT+GMM**<br>**<objectID>**|
|**OK**|
|Maximum Response Time<br>300 ms|
|Characteristics<br>/|
|Reference|
|V.25ter|



###### **Parameter** 

**<objectID>** String type. Device type identifier. 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    14 / 207** 

**LTE Standard Module Series** 



##### **2.4. AT+GMR  Request TA Firmware Revision Identification** 

This command returns the product firmware version. It is identical to **AT+CGMR** . 

|**AT+GMR  Request TA Firmware Revision Identification**|
|---|
|Test Command<br>Response|
|**AT+GMR=?**<br>**OK**|
|Execution Command<br>Response|
|**AT+GMR**<br>**<revision>**|
|**OK**|
|Maximum Response Time<br>300 ms|
|Characteristics<br>/|
|Reference|
|V.25ter|



###### **Parameter** 

**<revision>** String type. product firmware version identifier. 

###### **Example** 

**<mark>AT+GMR EG800QEULCR01A03M04 OK</mark>** 

##### **2.5. AT+CGMI Request Manufacturer Identification** 

This command returns the manufacturer identification. It is identical to **AT+GMI** . 

|**AT+CGMI  Request Manufacturer**|**Identification**|
|---|---|
|Test Command|Response|
|**AT+CGMI=?**|**OK**|
|Execution Command|Response|
|**AT+CGMI**|**Quectel**|
||**OK**|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    15 / 207** 

**LTE Standard Module Series** 



|Maximum Response Time|300 ms|
|---|---|
|Characteristics|/|
|Reference||
|3GPP TS 27.007||



##### **2.6. AT+CGMM  Request MT Model Identification** 

This command returns the product model information. It is identical to the above **AT+GMM** . 

|**AT+CGMM  Request MT Model Identification**|
|---|
|Test Command<br>Response|
|**AT+CGMM=?**<br>**OK**|
|Execution Command<br>Response|
|**AT+CGMM**<br>**<objectID>**<br>**OK**|
|Maximum Response Time<br>300 ms|
|Characteristics<br>/|
|Reference<br>3GPP TS 27.007|



**Parameter** 

**<objectID>** String type. Product model identifier. 

##### **2.7. AT+CGMR  Request MT Firmware Revision Identification** 

This command returns the MT firmware version identification. It is identical to **AT+GMR** in **_Chapter 2.4_** . 

|**AT+CGMR** **Request MT Firmware Revision Identification**|
|---|
|Test Command<br>Response|
|**AT+CGMR=?**<br>**OK**|
|Execution Command<br>Response|
|**AT+CGMR**<br>**<revision>**|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    16 / 207** 

**LTE Standard Module Series** 



||**OK**|
|---|---|
|Maximum Response Time|300 ms|
|Characteristics|/|
|Reference||
|3GPP TS 27.007||



###### **Parameter** 

**<revision>** String type. MT firmware version identifier. 

##### **2.8. AT+GSN  Request IMEI and SN** 

This command returns the International Mobile Equipment Identity (IMEI) number, which permits the user to identify individual ME device, and the Serial Number (SN) of the ME. It is identical to the **AT+CGSN** in **_Chapter 2.9_** . 

###### **AT+GSN  Request International Mobile Equipment Identity (IMEI) and SN** 

|Test Command|Response|
|---|---|
|**AT+GSN=?**|**+GSN: (**list of supported**<snt>**s**)**|
||**OK**|
|Write Command<br>**AT+GSN=<snt>**|Response<br>If**<snt>**=0, query SN of the ME:<br>**+GSN: <SN>**|
||**OK**<br>If**<snt>**=1, query IMEI of the ME:<br>**+GSN: <IMEI>**|
||**OK**|
|Execution Command<br>**AT+GSN**|Response<br>**<IMEI>**|
||**OK**<br>Or<br>**ERROR**|
||If there is any error related to ME functionality:<br>**+CMS ERROR: <err>**|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    17 / 207** 

**LTE Standard Module Series** 



|Maximum Response Time|300 ms|
|---|---|
|Characteristics|/|
|Reference||
|3GPP TS 27.007||



###### **Parameter** 

|**<snt>**|Integer type. Whether to query SN or IMEI of the ME.|
|---|---|
||0<br>Query SN of the ME|
||1<br>Query IMEI of the ME|
|**<SN>**|String type. SN of the ME.|
|**<IMEI>**|String type. IMEI of the ME.|
|**<err>**|Error codes. For more details, see**_Chapter 12.5_**_._|



##### **2.9. AT+CGSN  Request IMEI and SN** 

This command returns the International Mobile Equipment Identity (IMEI) number, which permits the user to identify individual ME device, and the Serial Number (SN) of ME. It is identical to the **AT+GSN** in **_Chapter 2.8_** . 

###### **AT+CGSN  Request International Mobile Equipment Identity (IMEI)** 

|Test Command|Response|
|---|---|
|**AT+CGSN=?**|**+CGSN: (**list of supported**<snt>**s**)**|
||**OK**|
|Write Command<br>**AT+CGSN=<snt>**|Response<br>If**<snt>**=0, query SN of the ME:<br>**+CGSN: <SN>**|
||**OK**<br>If**<snt>**=1, query IMEI of the ME:<br>**+CGSN: <IMEI>**|
||**OK**|
|Execution Command<br>**AT+CGSN**|Response<br>**<IMEI>**|
||**OK**<br>Or<br>**ERROR**|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    18 / 207** 

**LTE Standard Module Series** 



||If there is any error related to ME functionality:<br>**+CMS ERROR: <err>**|
|---|---|
|Maximum Response Time|300 ms|
|Characteristics|/|
|Reference||
|3GPP TS 27.007||



###### **Parameter** 

**<snt>** Integer type. Whether to query SN or IMEI of the ME. 0 Query SN of the ME 1 Query IMEI of the ME **<SN>** String type. SN of the ME. **<IMEI>** String type. IMEI of the ME. **<err>** Error codes. For more details, see **_Chapter 12.5_** _._ 

##### **2.10. AT&F  Reset AT Command Settings to Factory Defaults** 

This command resets all AT command settings to the default values specified by the manufacturer. (See **_Table 12_** ). 

###### **AT&F  Reset AT Command Settings to Factory Defaults** 

|Execution Command|Response|
|---|---|
|**AT&F[<value>]**|**OK**|
|Maximum Response Time|300 ms|
|Characteristics|/|
|Reference||
|V.25ter||



###### **Parameter** 

**<value>** Integer type. <u>0</u> Reset all AT command settings to factory defaults 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    19 / 207** 

**LTE Standard Module Series** 



##### **2.11. AT&V  Display Current Configuration** 

This command displays the current settings of certain AT command parameters (see **_Table 2_** ), including the single-letter AT command parameters that are not readable. 

|**AT&V  Display Current Configuration**|
|---|
|Execution Command<br>Response|
|**AT&V**<br>**OK**|
|Maximum Response Time<br>300 ms|
|Characteristics<br>/|
|Reference|
|V.25ter|



**Table 2: AT&V Response** 

**AT&V** &C: 1 &D: 2 &F: 0 &W: 0 E: 1 Q: 0 V: 1 X: 4 Z: 0 S3: 13 S4: 10 S5: 8 S12: 50 OK 

##### **2.12. AT&W  Store Current AT Command Settings to User-Defined Profile** 

This command stores the current AT command settings to a user-defined profile in non-volatile memory. (See **_Table 13_** ). The AT command settings will be automatically restored from the user-defined profile during power-up or if ATZ is executed. 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    20 / 207** 

**LTE Standard Module Series** 



|**AT&W  Store Current AT Co**|**mmand Settings to User-Defined Profile**|
|---|---|
|Execution Command|Response|
|**AT&W[<n>]**|**OK**|
|Maximum Response Time|300 ms|
|Characteristics|/|
|Reference<br>V.25ter||



###### **Parameter** 

**<n>** Integer type. <u>0</u> Profile number to store current AT command settings. 

##### **2.13. ATZ  Restore All AT Command Settings from User-Defined Profile** 

This command first resets the AT command settings to factory defaults. Afterwards, the AT command settings are restored from the user-defined profile in the non-volatile memory, if they have been stored with **AT&W** before (See **_Table 14_** ). 

Any additional AT command on the same command line may be ignored. 

|**ATZ  Restore All AT Comma**|**nd Settings from User-Defined Profile**|
|---|---|
|Execution Command|Response|
|**ATZ[<value>]**|**OK**|
|Maximum Response Time|300 ms|
|Characteristics|/|
|Reference||
|V.25ter||



###### **Parameter** 

**<value>** Integer type. <u>0</u> Reset to profile number 0. 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    21 / 207** 

**LTE Standard Module Series** 



##### **2.14. ATQ  Set Result Code Presentation Mode** 

This command controls whether the result code is transmitted to the TE. Other information text transmitted as response is not affected. 

|**ATQ  Set Result Code Prese**|**ntation Mode**|
|---|---|
|Execution Command|Response|
|**ATQ<n>**|If**<n>**=0:<br>**OK**|
||If**<n>**=1:<br>(none)|
|Maximum Response Time|300 ms|
|Characteristics|The command takes effect immediately.<br>The configuration is not saved.|
|Reference<br>V.25ter||



###### **Parameter** 

|**<n>**|Integer|type.|
|---|---|---|
||0|TA transmits result code|
||1|Result codes are suppressed and not transmitted|



##### **2.15. ATV  AT Response Format** 

This command determines the contents of header and trailer transmitted with AT command result codes and information responses. 

The numeric equivalents and brief descriptions of results code are listed in **_Table 3_** . 

|**ATV  AT Response Format**||
|---|---|
|Execution Command|Response|
|**ATV<value>**|When**<value>**=0:<br>**0**|
||When**<value>**=1:<br>**OK**|
|Maximum Response Time|300 ms|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    22 / 207** 

**LTE Standard Module Series** 



|Characteristics|The command takes effect immediately.<br>The configuration is not saved.|
|---|---|
|Reference||
|V.25ter||



###### **Parameter** 

|**<value>**|Integer type.|
|---|---|
||0<br>Information response:**<text><CR><LF>**|
||Short result code format:**<numeric code><CR>**|
||1<br>Information response:**<CR><LF><text><CR><LF>**|
||Longresult code format:**<CR><LF><verbose code><CR><LF>**|



###### **Example** 

**<mark>ATV1</mark>** <mark>//Set</mark> **<mark><value></mark>** <mark>=1</mark> **<mark>OK AT+CSQ +CSQ: 30,99 OK</mark>** <mark>//When</mark> **<mark><value></mark>** <mark>=1, the result code is</mark> **<mark>OK</mark>** <mark>.</mark> **<mark>ATV0</mark>** <mark>//Set</mark> **<mark><value></mark>** <mark>=0</mark> **<mark>0 AT+CSQ +CSQ: 30,99 0</mark>** <mark>//When</mark> **<mark><value></mark>** <mark>=0, the result code is</mark> **<mark>0</mark>** <mark>.</mark> 

**Table 3: Numeric Equivalents and Brief Descriptions of ATV0&ATV1 Result Codes** 

|**ATV1**|**ATV0**|**Description**|
|---|---|---|
|OK|0|Acknowledges execution of a command|
|CONNECT|1|A connection has been established; the DCE is switching from<br>command mode to data mode|
|RING|2|DCE has detected an incoming call signal from network|
|NO CARRIER|3|The connection has been terminated or the attempt to establish<br>a connection failed|
|ERROR|4|Command not recognized, command line maximum length<br>exceeded, parameter value invalid, or other problem with<br>processingthe command line|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    23 / 207** 

**LTE Standard Module Series** 



|NO DIALTONE|6|No dial tone detected|
|---|---|---|
|BUSY|7|Engaged (busy) signal detected|
|||"@" (Wait for Quiet Answer) dial modifier was used, but remote|
|NO ANSWER|8|ringing followed by 5 s of silence was not detected before|
|||connection timer(S7)expired|



##### **2.16. ATE  Set Command Echo Mode** 

This command controls whether TA echoes characters received from TE or not in AT command mode. 

###### **ATE  Set Command Echo Mode** 

|Execution Command|Response|
|---|---|
|**ATE<value>**|**OK**|
|Maximum Response Time|300 ms|
|Characteristics|The command takes effect immediately.<br>The configuration is not saved.|
|Reference||
|V.25ter||



###### **Parameter** 

**<value>** Integer type. Whether to echo the characters received from TE. 

- 0 Echo mode OFF 

- <u>1</u> Echo mode ON 

##### **2.17. ATS3  Set Command Line Termination Character** 

This command determines the character that terminates an incoming command line, which is recognized by TA. It is also generated for result codes and information text, along with character value set via **ATS4** . 

###### **ATS3  Set Command Line Termination Character** 

|Read Command|Response|
|---|---|
|**ATS3?**|**<n>**|
||**OK**|
|Write Command|Response|
|**ATS3=<n>**|**OK**|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    24 / 207** 

**LTE Standard Module Series** 



|Maximum Response Time|300 ms|
|---|---|
|Characteristics|The command takes effect immediately.<br>The configuration is not saved.|
|Reference||
|V.25ter||



###### **Parameter** 

**<n>** Integer type. Command line termination character. Range: 0–127. Default: 13. 

##### **2.18. ATS4  Set Response Formatting Character** 

This command determines the character generated by TA for result code and information text, along with the command line termination character set via **ATS3** . 

|**ATS4  Set Response Forma**|**tting Character**|
|---|---|
|Read Command<br>**ATS4?**|Response<br>**<n>**|
||**OK**|
|Write Command<br>**ATS4=<n>**|Response<br>**OK**|
|Maximum Response Time|300 ms|
|Characteristics|The command takes effect immediately.<br>The configuration is not saved.|
|Reference||
|V.25ter||



###### **Parameter** 

**<n>** Integer type. Response formatting character. Range: 0–127. Default: 10. 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    25 / 207** 

**LTE Standard Module Series** 



##### **2.19. ATS5  Set Command Line Editing Character** 

This command determines the character value used by the module to delete the immediately preceding character from the AT command line (i.e. equates to backspace key). 

|**ATS5  Set Command Line E**|**diting Character**|
|---|---|
|Read Command<br>**ATS5?**|Response<br>**<n>**|
||**OK**|
|Write Command|Response|
|**ATS5=<n>**|**OK**|
|Maximum Response Time|300 ms|
|Characteristics|The command takes effect immediately.<br>The configuration is not saved.|
|Reference<br>V.25ter||



###### **Parameter** 

**<n>** Integer type. Command line editing character. Range: 0–127. Default: 8. 

##### **2.20. ATX  Set CONNECT Result Code Format and Monitor Call Progress** 

This command determines whether TA transmits particular result codes to the TE or not. It also controls whether TA detects the presence of a dial tone when it begins dialing and the engaged tone (busy signal) or not. 

|**ATX  Set CONNECT Result**|**Code Format and Monitor Call Progress**|
|---|---|
|Execution Command|Response|
|**ATX<value>**|**OK**|
|Maximum Response Time|300 ms|
|Characteristics|The command takes effect immediately.<br>The configurations can be saved with**AT&W**.|
|Reference||
|V.25ter||



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    26 / 207** 

**LTE Standard Module Series** 



###### **Parameter** 

**<value>** Integer type. 

- 0 Only **CONNECT** result code returned, dial tone and busy detection are both disabled. 

- 1 Only **CONNECT<text>** result code returned, dial tone and busy detection are both disabled. 

- 2 **CONNECT<text>** result code returned, dial tone detection is enabled, and busy 

   - detection is disabled. 

- 3 **CONNECT<text>** result code returned, dial tone detection is disabled, and busy 

   - detection is enabled. 

- <u>4</u> **CONNECT<text>** result code returned, and dial tone and busy detection are both enabled. 

##### **2.21. AT+CFUN Set UE Functionality** 

This command controls the UE functionality level. It can also be used to reset the UE. 

|**AT+CFUN  Set UE Functionality**|
|---|
|Test Command<br>**AT+CFUN=?**<br>Response<br>**+CFUN: (**list of supported**<fun>**s**),(**list of supported**<rst>**s**)**|
|**OK**|
|Read Command<br>**AT+CFUN?**<br>Response<br>**+CFUN: <fun>**|
|**OK**|
|Write Command<br>Response|
|**AT+CFUN=<fun>[,<rst>]**<br>**OK**|
|If there is any error related to ME functionality:<br>**+CME ERROR: <err>**|
|Maximum Response Time<br>15 s, determined by network.|
|Characteristics<br>The command takes effect immediately.<br>The configuration is not saved.|
|Reference<br>3GPP TS 27.007|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    27 / 207** 

**LTE Standard Module Series** 



###### **Parameter** 

|**<fun>**|Integ|er|type.|
|---|---|---|---|
||0||Minimum functionality|
||1||Full functionality|
||4||Disable the ME from both transmitting and receiving RF signals|
|**<rst>**|Integ|er|type.|
||0||Do not reset the ME before setting it to**<fun>**functionality level.|
||1||Reset the ME. Device is fully functional after the reset. This value is available only|
||||for**<fun>**=1|
|**<err>**|Error|co|des. For more details, see**_Chapter 12.4_**.|



###### **Example** 

**<mark>AT+CFUN=0</mark>** <mark>//Switch UE to minimum functionality.</mark> **<mark>OK AT+COPS? +COPS: 0</mark>** <mark>//No operator is registered.</mark> **<mark>OK AT+CPIN? +CME ERROR: 13</mark>** <mark>//(U)SIM failure.</mark> **<mark>AT+CFUN=1</mark>** <mark>//Switch UE to full functionality.</mark> **<mark>OK +CPIN: SIM PIN AT+CPIN=1234 OK +CPIN: READY +QUSIM: 1 +QIND: PB DONE +QIND: SMS DONE AT+CPIN? +CPIN: READY OK AT+COPS? +COPS: 0,0,"CHINA MOBILE",7</mark>** <mark>//Operator is registered.</mark> 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    28 / 207** 

**LTE Standard Module Series** 



###### **<mark>OK</mark>** 

##### **2.22. AT+CMEE  Error Message Format** 

This command controls the format of error result codes: **ERROR** , error numbers or verbose messages as **+CME ERROR: <err>** and **+CMS ERROR: <err>** . This command disables or enables the use of final result code **+CME ERROR: <err>** as the indication of an error. 

###### **AT+CMEE  Error Message Format** 

|Test Command|Response|
|---|---|
|**AT+CMEE=?**|**+CMEE: (**list of supported**<n>**s**)**|
||**OK**|
|Read Command<br>**AT+CMEE?**|Response<br>**+CMEE: <n>**|
||**OK**|
|Write Command<br>**AT+CMEE=<n>**|Response<br>**OK**|
|Maximum Response Time|300 ms|
|Characteristics|The command takes effect immediately.<br>The configuration is not saved.|
|Reference||
|3GPP TS 27.007||



###### **Parameter** 

**<n>** Integer type. 

- 0 Disable result code and use ERROR instead 

- <u>1</u> Enable result code and use numeric values 2 Enable result code and use verbose values 

###### **Example** 

|**AT+CMEE=0**|//Disable result code.|
|---|---|
|**OK**||
|**AT+CPIN?**||
|**ERROR**|//Only**ERROR**is displayed|
|**AT+CMEE=1**|//Enable error result code with numeric values.|
|**OK**||



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    29 / 207** 



**LTE Standard Module Series** 

**<mark>AT+CPIN? +CME ERROR: 10 AT+CMEE=2</mark>** <mark>//Enable error result code with verbose (string) values.</mark> **<mark>OK AT+CPIN? +CME ERROR: SIM not inserted</mark>** 

##### **2.23. AT+CSCS  Select TE Character Set** 

This command informs the MT whose character set is used by the TE. TA is then able to convert character strings correctly between TE and MT character sets. 

###### **AT+CSCS  Select TE Character Set** 

|Test Command|Response|
|---|---|
|**AT+CSCS=?**|**+CSCS:** **(**list of supported**<chset>**s**)**|
||**OK**|
|Read Command<br>**AT+CSCS?**|Response<br>**+CSCS: <chset>**|
||**OK**|
|Write Command<br>**AT+CSCS=<chset>**|Response<br>**OK**|
|Maximum Response Time|300 ms|
|Characteristics|The command takes effect immediately.<br>The configuration is not saved.|
|Reference||
|3GPP TS 27.007||



###### **Parameter** 

**<chset>** String type. Character set "GSM"      GSM default alphabet "IRA" International reference alphabet "UCS2" UCS2 alphabet 

###### **Example** 

**<mark>AT+CSCS?</mark>** <mark>//Query the current character set.</mark> **<mark>+CSCS: "GSM"</mark>** <mark>//The character set is GSM.</mark> 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    30 / 207** 

**LTE Standard Module Series** 



**<mark>OK AT+CSCS="UCS2"</mark>** <mark>//Set the character set to "UCS2".</mark> **<mark>OK AT+CSCS? +CSCS: "UCS2"</mark>** <mark>//The character set is UCS2 after the configuration.</mark> **<mark>OK</mark>** 

##### **2.24. AT+QURCCFG  Configure URC Output Port** 

This command configures the URC output port. 

|**AT+QURCCFG  Configure URC I**|**ndication Option**|
|---|---|
|Test Command<br>**AT+QURCCFG=?**|Response<br>**+QURCCFG: "urcport",(**list of supported**<urc_port_valu**<br>**e>**s**)**<br>**OK**|
|Write Command|Response|
|**AT+QURCCFG="urcport"[,<urc_port**<br>**_value>]**|If the optional parameter is omitted, query the current setting:<br>**+QURCCFG: "urcport",<urc_port_value>**|
||**OK**|
||If the optional parameter is specified, set the URC output port:<br>**OK**<br>Or<br>**ERROR**|
|Maximum Response Time|300 ms|
|Characteristics|The command takes effect immediately.<br>The configuration is saved automatically.|



###### **Parameter** 

|**<urc_port_value>**String type. URC out|put port.|
|---|---|
|"usbat"|USB AT port|
|"usbmodem"|<br>USB modem port|
|"uart1"|Main UART|
|"uart2"|Debug UART|
|"all"|Allports|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    31 / 207** 

**LTE Standard Module Series** 



###### **Example** 

**<mark>AT+QURCCFG=? +QURCCFG: "urcport",("usbat","usbmodem","uart1","uart2","all") OK AT+QURCCFG="urcport" +QURCCFG: "urcport","usbat" OK AT+QURCCFG="urcport","usbmodem" OK AT+QURCCFG="urcport" +QURCCFG: "urcport","usbmodem" OK</mark>** 

##### **2.25. AT+QDIAGPORT  Configure Debug UART as AT Port** 

This commans configures debug UART as AT port. 

|**AT+QDIAGPORT  Configura**|**te Debug UART as AT Port**|
|---|---|
|Read Command|Response|
|**AT+QDIAGPORT=?**|**+QDIAGPORT: (**list of supported**<num>**s**)**|
||**OK**|
|Write Command|Response|
|**AT+QDIAGPORT=<num>**|**OK**<br>Or<br>**ERROR**|
|Maximum Response Time|12 s|
|Characteristics|The command takes effect after module is rebooted.<br>The configuration is saved automatically.|



###### **Parameter** 

**<num>** Integer type. <u>0</u> Debug UART port 1 AT port 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    32 / 207** 

**LTE Standard Module Series** 



###### **NOTE** 

1. When Debug UART is configured to AT port, the baud rate is fixed to 115200 bps. 

2. Outputting module booting message remains enabled when Debug UART is configured to AT port. 

3. It is recommended not to set up data connection on Debug UART due to limited baud rate. 

##### **2.26. AT+QPPPDROOP  Terminate PPP Connection** 

This command terminates the PPP connection. 

|**AT+QPPPDROOP Terminate PPP Connection**<br> <br>|
|---|
|Test Command<br>**AT+QPPPDROP=?**<br>Response<br>**+ QPPPDROP : (**list of supported**<op>**s**)**|
|**OK**|
|Write Command<br>Response|
|**AT+QPPPDROP[=<op>]**<br>**OK**|
|Maximum Response Time<br>300 ms|
|Characteristics<br>The command takes effect immediately.<br>The configuration is not saved.|



###### **Parameter** 

- **<op>** Integer Type. The operation about dropping PPP connection. 

   - 0  Hung up PPP connection without sending TERM REQ frame to peer. 

   - 1  Hung up PPP connection and automatically send TERM REQ frame to peer. 

   - 2 Hung up PPP connection with sending TERM REQ frame to peer. 

###### **NOTES** 

1. This command will return OK immediately and then perform following procedures: Step1: Check if PPP link exists. Run step2 if exists. 

   - Step2: Hung up PPP connection and then output NO CARRIER to the port that’s used for dialing up. 

2. if **<op>** is 1, TERM REQ frame will be sent to peer only when PPP connection exists and dial-up port is in data mode. 

3. If **<op>** is omitted, the hung-up procedure is the same as one that **<op>** is set to 1. 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    33 / 207** 



###### **Example** 

**<mark>AT+QPPPDROP</mark>** <mark>//send command from USB_AT port</mark> **<mark>OK</mark>** <mark>//receive result from USB_AT port //MCU’s Dial-up port receive the TERM REQ frame from module</mark> **<mark>7E FF 7D 23 C0 21 7D 25 7D 25 7D 20 7D 24 5C A4 7E</mark>** 

**<mark>NO CARRIER</mark>** <mark>//PPP has been terminated and output this URC from dial-up port. If</mark> **<mark>ATV0</mark>** <mark>is set, module will show</mark> **<mark>NO CARRIER</mark>** <mark>as number format(3).</mark> 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    34 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
RUECCTEL<br><!-- End of picture text -->

# **3 Serial Interface Control Commands** 

##### **3.1. AT&C  Set DCD Function Mode** 

This command controls the behavior of the UE’s DCD (data carrier detection) line. 

|**AT&C  Set DCD Function Mode**||
|---|---|
|Execution Command|Response|
|**AT&C[<value>]**|**OK**|
|Maximum Response Time|300 ms|
|Characteristics|The command takes effect immediately.<br>The configurations can be saved with**AT&W**.|
|Reference||
|V.25ter||



###### **Parameter** 

- **<value>** Integer type. It determines how the state of circuit (DCD) relates to the detection of 

   - received line signal from the distant end. 

   - 0 DCD line is always ON 

   - <u>1</u> DCD line is ON only in the presence of data carrier 

##### **3.2. AT&D  Set DTR Function Mode** 

This command determines how the UE responds if DTR line is changed from low to high level in data mode. 

|**AT&D  Set DTR Function Mode**||
|---|---|
|Execution Command|Response|
|**AT&D[<value>]**|**OK**|
|Maximum Response Time|300 ms|
|Characteristics|The command takes effect immediately.|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    35 / 207** 

**LTE Standard Module Series** 



The configurations can be saved with **AT&W** . Reference V.25ter 

###### **Parameter** 

- **<value>** Integer type. 

   - 0 TA ignores status on DTR. 

   - 1 Low→High on DTR: Switch to command mode while the call is still connected. 

   - <u>2   Low→High on DTR: Disconnect data call, and switch to command mode. When DTR</u> is at high level, auto-answer function is disabled. 

##### **3.3. AT+IFC  Set TE-TA Local Data Flow Control** 

This command sets the serial port flow control for data mode. 

###### **AT+IFC  Set TE-TA Local Data Flow Control** 

|Test Command|Response|
|---|---|
|**AT+IFC=?**|**+IFC: (**list of supported**<dce_by_dte>**s**),(**list of supported<br>**<dte_by_dce>**s**)**|
||**OK**|
|Read Command<br>**AT+IFC?**|Response<br>**+IFC: <dce_by_dte>,<dte_by_dce>**|
||**OK**|
|Write Command|Response|
|**AT+IFC=<dce_by_dte>,<dte_by_dce>**|**OK**<br>Or<br>**ERROR**|
|Maximum Response Time|300 ms|
|Characteristics|The command takes effect immediately.<br>The configurations can be saved with**AT&W**.|
|Reference||
|V.25ter||



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    36 / 207** 

**LTE Standard Module Series** 



###### **Parameter** 

|**<dce_by_dte>**|Integer|type. Specifies the method that will be used by TE when receiving data from|
|---|---|---|
||TA.||
||0|None|
||2|RTS flow control|
|**<dte_by_dce>** <br>|Integer<br>TE.|type. Specifies the method that will be used by TA when receiving data from|
||0|None|
||2|CTS flow control|



**NOTE** Flow control is only applicable for data mode. 

###### **Example** 

**<mark>AT+IFC=2,2</mark>** <mark>//Enable the hardware flow control.</mark> **<mark>OK AT+IFC? +IFC: 2,2 OK</mark>** 

##### **3.4. AT+ICF  Set TE-TA Control Character Framing** 

This command determines the serial interface character framing format and parity received by TA from TE. 

|**AT+ICF  Set TE-TA Control Ch**|**aracter Framing**|
|---|---|
|Test Command|Response|
|**AT+ICF=?**|**+ICF: (**list of supported**<format>**s**),(**list of supported<br>**<parity>**s**)**|
||**OK**|
|Read Command|Response|
|**AT+ICF?**|**+ICF: <format>,<parity>**|
||**OK**|
|Write Command|Response|
|**AT+ICF=[<format>,[<parity>]]**|**OK**|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    37 / 207** 

**LTE Standard Module Series** 



||Or<br>**ERROR**|
|---|---|
|Maximum Response Time|300 ms|
|Characteristics|The command takes effect immediately.<br>The configurations can be saved with**AT&W**.|
|Reference||
|V.25ter||



###### **Parameter** 

|**<format>**|Integer|type.|
|---|---|---|
||3|8 data 0 parity 1 stop|
|**<parity>**|Integer|type.|
||0|Odd|
||1|Even|
||2|Mark (1)|
||3|Space(0)|



###### **NOTE** 

1. The command is valid only in command mode. 

2. **<parity>** field is omitted if no parity is specified in **<format>** field. 

##### **3.5. AT+IPR  Set TE-TA Fixed Local Rate** 

This command queries and sets the UART baud rate. 

###### **AT+IPR  Set TE-TA Fixed Local Rate** 

|Test Command|Response|
|---|---|
|**AT+IPR=?**|**+IPR: (**list of supported auto detectable**<rate>**s**),(**list of<br>supported fixed-only**<rate>**s**)**|
||**OK**|
|Read Command|Response|
|**AT+IPR?**|**+IPR: <rate>**|
||**OK**|
|Write Command|Response|
|**AT+IPR=<rate>**|**OK**|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    38 / 207** 

**LTE Standard Module Series** 



|Maximum Response Time|300 ms|
|---|---|
|Characteristics|The command takes effect immediately.<br>The configurations can be saved with**AT&W**.|
|Reference||
|V.25ter||



###### **Parameter** 

|**<rate>**|String type.|Baud rate per second.|
|---|---|---|
||4800||
||9600||
||19200||
||38400||
||57600||
||115200||
||230400||
||460800||
||921600||



###### **NOTE** 

<mark>1. If a fixed baud rate is set, make sure that both TE (DTE, usually external processor) and TA (DCE,</mark> Quectel module) are configured to the same rate. 

2. **AT+IPR** value cannot be restored with **AT&F** and **ATZ** ; but it is still storable with **AT&W** . 

3. In multiplex mode, the baud rate cannot be changed with the Write Command **AT+IPR=<rate>** ; and the setting is invalid and cannot be stored even if **AT&W** is executed after the Write Command. 

4. A selected baud rate takes effect after the Write Commands are executed and acknowledged by **OK** . 

5. After the baud rate is changed, it is necessary to wait for 500 ms to send the next command. 

###### **Example** 

|**AT+IPR=115200**|//Set fixed baud rate to 115200 bps|
|---|---|
|**OK**||
|**AT&W**|//Store current setting, that is, the serial communication speed is|
|**OK**|115200 bps after restarting module|
|**AT+IPR?**||
|**+IPR: 115200**||
|**OK**||
|**AT+IPR=115200;&W**|//Set fixed baud rate to 115200 bps and store current setting|
|**OK**||



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    39 / 207** 

**LTE Standard Module Series** 



# **4 Status Control Commands** 

##### **4.1. AT+CPAS  Mobile Equipment Activity Status** 

This command queries the module’s activity status. 

|**AT+CPAS  Mobile Equipme**|**nt Activity Status**|
|---|---|
|Test Command|Response|
|**AT+CPAS=?**|**+CPAS: (**list of supported**<pas>**s**)**|
||**OK**|
|Execution Command|Response|
|**AT+CPAS**|TA returns the activity status of ME:<br>**+CPAS: <pas>**|
||**OK**<br>Or<br>**ERROR**|
||If there is any error related to ME functionality:<br>**+CME ERROR: <err>**|
|Maximum Response Time|300 ms|
|Characteristics|/|
|Reference||
|3GPP TS 27.007||



###### **Parameter** 

**<pas>** Integer type. ME activity status. <u>0</u> Ready 2       Unknow 3 Ringing 4 Call in progress or call on hold 

**<err>** Error codes. For more details, see **_Chapter 12.4_** . 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    40 / 207** 

**LTE Standard Module Series** 



###### **Example** 

**<mark>AT+CPAS +CPAS: 0</mark>** <mark>//The module is idle</mark> **<mark>OK RING AT+CLCC +CLCC: 1,1,4,0,0,"15695519173",161</mark>** 

**<mark>OK AT+CPAS +CPAS: 3</mark>** <mark>//The module is ringing</mark> **<mark>OK AT+CLCC +CLCC: 1,0,0,0,0,"10010",129 OK AT+CPAS +CPAS: 4</mark>** <mark>//Call in progress</mark> **<mark>OK</mark>** 

##### **4.2. AT+CEER  Extended Error Report** 

This command queries an extended error and reports the cause of the last failed operation, such as: 

- ⚫ Failure to activate, register, query, deactivate or deregister a supplementary service 

- ⚫ Failure to activate or deactivate a PDP context 

The release cause **<text>** is a text that describes the cause information given by the network. 

###### **AT+CEER Extended Error Report** 

Test Command Response **AT+CEER=? OK** Execution Command Response **AT+CEER +CEER: <text> OK** Or **ERROR** 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    41 / 207** 

**LTE Standard Module Series** 



||If error is related to ME functionality:|
|---|---|
||**+CME ERROR: <err>**|
|Maximum Response Time|300 ms|
|Characteristics|/|



###### **Parameter** 

**<text>** Release cause text. Reason for the last call failure to setup or release (listed in **_Chapter 12.8_** ). Both CS and PS domain call types are reported. Cause data is captured from Call Manager events and cached locally for later use by this command. **<err>** Error codes. For more details, see **_Chapter 12.4_** . 

##### **4.3. AT+QINDCFG  Configure URC Indication** 

This command controls URC indication. 

|**AT+QINDCFG** **Configure URC Ind**|**ication**|
|---|---|
|Test Command|Response|
|**AT+QINDCFG=?**|**+QINDCFG: "all",(**list of supported**<enable>**s**),(**list of<br>supported**<save_to_nvram>**s**)**<br>**+QINDCFG: "csq",(**list of supported**<enable>**s**),(**list of<br>supported**<save_to_nvram>**s**)**|
||**+QINDCFG:**<br>**"datastatus",(**list<br>of<br>supported<br>**<enable>**s**),(**list of supported**<save_to_nvram>**s**)**<br>**+QINDCFG: "mode",(**list of supported**<enable>**s**),(**list of<br>supported**<save_to_nvram>**s**)**<br>**+QINDCFG: "smsfull",(**list of supported**<enable>**s**),(**list of<br>supported**<save_to_nvram>**s**)**|
||**+QINDCFG:**<br>**"smsincoming",(**list<br>of<br>supported|
||**<enable>**s**),(**list of supported**<save_to_nvram>**s**)**<br>**+QINDCFG: "act",(**list of supported**<enable>**s**),(**list of<br>supported**<save_to_nvram>**s**)**<br>**+QINDCFG: "sqi",(**list of supported**<enable>**s**),(**list of<br>supported**<save_to_nvram>**s**)**<br>**OK**|
|Write Command|Response|
|**AT+QINDCFG=<urctype>[,<enable>[,<**<br>**save_to_nvram>]]**|If the optional parameters are omitted, query the current<br>setting:|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    42 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

||**+QINDCFG: <urctype>,<enable>**|
|---|---|
||**OK**|
||If the optional parameters are specified, set the URC<br>indication configurations:<br>**OK**<br>Or|
||**ERROR**|
||If there is any error related to ME functionality:<br>**+CME ERROR: <err>**|
|Maximum Response Time|300 ms|



###### **Parameter** 

- **<urctype>** String type. URC type. "all" Main switch of all URCs. Default: ON. "csq" Indication of signal strength and channel bit error rate change (similar to **AT+CSQ** ). Default: OFF. If this configuration is ON, present: 

- **+QIND: "csq",<rssi>,<ber>** 

- "datastatus" The URC of data service status. Default: OFF. If **<enable>** is set to ON, the following URC is reported: **+QIND: "datastatus",<suspended>,<reason>** (the URC is currently not supported) **<suspended>** Reserved. **<reason>** Integer type. The values are as follows: 0 SUSPEND_NO_CAUSE 1 SUSPEND_BY_RAU_ATTACH 2 SUSPEND_BY_LAU 3 SUSPEND_BY_TAU 4 SUSPEND_BY_CS_SERVICE 5 SUSPEND_BY_DS_OPERATION 6 SUSPEND_BY_POWERUP 

- "mode" The URC of network main mode and sub mode. Default: OFF. If **<enable>** is set to ON, the following URC is reported: **^MODE: <main_mode>,<sub_mode> <main_mode>** Integer type. Network main mode. 0 SYSINFO_SYSTEMO_MODE_NO_SERVICE 1 SYSINFO_SYSTEMO_MODE_RESERVED_1 2 SYSINFO_SYSTEMO_MODE_RESERVED_2 3 SYSINFO_SYSTEMO_MODE_GSM_GPRS 5 SYSINFO_SYSTEMO_MODE_WCDMA 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    43 / 207** 

**LTE Standard Module Series** 



17 SYSINFO_SYSTEMO_MODE_LTE 

**<sub_mode>** Integer type. Network sub mode. 0 SYSINFO_SYSTEMO_SUBMODE_NO_SERVICE 1 SYSINFO_SYSTEMO_SUBMODE_GSM 3 SYSINFO_SYSTEMO_SUBMODE_GSM_EGPRS 5 SYSINFO_SYSTEMO_SUBMODE_UTRAN_HSDPA 6 SYSINFO_SYSTEMO_SUBMODE_UTRAN_HSUPA 

7 SYSINFO_SYSTEMO_SUBMODE_UTRAN_HSPA 8 SYSINFO_SYSTEMO_SUBMODE_UTRAN 17 SYSINFO_SYSTEMO_SUBMODE_EUTRAN "smsfull" SMS storage full indication. Default: OFF. If this configuration is ON, present: **+QIND: "smsfull",<storage> <storage>** String type. Internal storage mode. "ME" UE mode "SM" SIM card mode "smsincoming"  Incoming message indication. Default: ON. Related URCs list: **+CMTI** , **+CMT** , **+CDS** "act"           Indication of network access technology change. Default: OFF. If this configuration is ON, present: **+QIND: "act",<actvalue> <actvalue>** is string format. Values: "LTE" "UNKNOWN" URC examples: **+QIND: "act","LTE" +QIND: "act","UNKNOWN"** Description of "act": 

1. If module does not register on network, **<actvalue>** is "UNKNOWN". 2. If this configuration is ON, the URC of "act" is reported immediately. Only when the network access technology changes, a new URC is reported. "sqi" The URC of reference signal receiving power, reference signal receiving quality and signal to interference plus noise ratio. Default is OFF. If **<enable>** is set to ON, the following URC is reported: **+QIND: "SQI",<RSRP>,<RSRQ>,<SINR> <RSRP>** Integer type. Reference signal receiving power. Unit: dBm. **<RSRQ>** Integer type. Reference signal receiving quality. Unit: dB. **<SINR>** Integer type. Signal to interference plus noise 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    44 / 207** 

**LTE Standard Module Series** 



ratio. Unit: dB. 

|**<enable>**<br>|Integer type.|Whether URC indication is enabled.|
|---|---|---|
||0<br>|OFF|
||1<br>|ON|
|**<save_to_nvram>**|Integer type.|Whether to save configuration into NVM.|
||0|Do not save|
||1|Save|
|**<err>**|Error codes,s|ee**_Chapter 12.4_**.|



##### **4.4. AT+QCFG  Extended Configuration Settings** 

###### **AT+QCFG Extended Configuration Settings** 

|Test command<br>**AT+QCFG=?**|Response<br>**…**|
|---|---|
||**+QCFG: "urc/ri/ring",(**list of supported**<typeri>**s**),(**list of<br>supported**<pulse_duration>**s**),(**list<br>of<br>supported<br>**<pulse_count>**s**)**|
||**+QCFG:**<br>**"urc/ri/smsincoming",(**list<br>of<br>supported<br>**<typeri>**s**),(**list of supported**<pulse_duration>**s**),(**list of<br>supported**<pulse_count>**s**)**<br>**+QCFG: "urc/ri/other",(**list of supported**<typeri>**s**),(**list of<br>supported**<pulse_duration>**s**),(**list of supported**<pulse_**<br>**count>**s**)**<br>**+QCFG: "urc/ri/level",(**list of supported**<level>**s)<br>**+QCFG: "cmux/urcport",(**list of supported**<URC_port>**s)<br>**+QCFG: "cmux/baudratework",(**list of supported**<enabl**<br>**e>**s**)**<br>**+QCFG: "urc/delay",(**list of supported**<time>**s**)**<br>**+QCFG: "urc/cache",(**list of supported**<enable>**s**)**<br>**+QCFG: "urc/port",(**list of supported**<URC_port_value>**<br>**s)**<br>**+QCFG: "risignaltype",(**list of supported**<RI_signal_typ**<br>**e>**s**)**<br>**+QCFG: "roamservice",(**list of supported**<roam_mode>**<br>s**),(**list of supported**<effect>**s**)**<br>**+QCFG: "band",(**list of supported**<bandval>**s**),(**list of<br>supported**<ltebandval>**s**)**<br>**+QCFG: "nwoptmz/acq",(**list of supported**<flag>**s**),(**list of<br>supported**<interval>**s**)**<br>**+QCFG: "** **usbnet",**(list of supported**<net>**s)|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    45 / 207** 

**LTE Standard Module Series** 



||**+QCFG: "aprstlevel",(**list of supported**<level>**s)<br>**+QCFG: "modemrstlevel",(**list of supported**<level>**s)<br>**+QCFG: "ntp",(**list of supported**<cnt>**s**),(**list of supported<br>**<interval>**s**)**<br>**+QCFG: "ledmode",(**list of supported**<mode>**s)<br>**+QCFG: "nat",(**list of supported**<nat>**s)<br>**+QCFG: "netmaskset",(**list of supported**<enable>**s**),<net**<br>**mask>**<br>**+QCFG: "ppp/termframe",(**list of supported**<flag>**s)<br>**+QCFG: "rf/tuner_cfg",(**list of supported**<index>**s)**, (**list<br>of supported**<lte bands>**s)<br>**+QCFG:"tcp/windowsize",(**list of supported**<buffer>**s**),(**lis<br>t of supported**<window_size>**s**)**<br>**+QCFG: "TCP/SendMode",(**list of supported**<mode>**s**)**<br>**+QCFG: "fast/poweroff",(**list of supported**<n>**s)<br>**+QCFG: "fota/cid",(**list of supported**<contextID>**s)<br> <br><br><br>|
|---|---|
||**+QCFG:**<br>**"qcautoconnect",(**list<br>of<br>supported<br>**<data_format>**s)<br>**+QCFG: "sms/listmsgmap",(**list of supported**<msgtype>**s)**,**<br>**(**list of supported**<msgmap>**s)<br>**+QCFG: "sms/MeNum",(**list of supported**<value>**s)|
||**OK**|
|Maximum Response Time|300 ms|



###### **4.4.1. AT+QCFG="urc/ri/ring"  Set RI Behavior When RING URC is Presented** 

The ring indicator is active low. **AT+QCFG="urc/ri/ring"** specifies the RI behavior when URC **RING** is presented to indicate an incoming call. 

The sum of **<active_duration>** and **<inactive_duration>** determines the **RING** indication interval of an incoming call. 

|**AT+QCFG="urc/ri/ring"** **Set RI Be**|**havior When RING URC is Presented**|
|---|---|
|Write Command|Response|
|**AT+QCFG="urc/ri/ring"[,<typeri>[,<pul**<br>**se_duration>[,<pulse_count>]]]**|If the optional parameters are omitted, query the current<br>setting:<br>**+QCFG: "urc/ri/ring",<typeri>,<pulse_duration>,<pulse**<br>**_count>**|
||**OK**|
||If anyof the optionalparameters is specified, set the RI|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    46 / 207** 

**LTE Standard Module Series** 



||behavior when**RING**URC is presented:<br>**OK**<br>Or<br>**ERROR**|
|---|---|
||If there is any error related to ME functionality:<br>**+CME ERROR: <err>**|
|Maximum Response Time|300 ms|
|Characteristics|This command takes effect immediately.<br>The configuration is saved automatically.|



###### **Parameter** 

|**<typeri>**|String type. RI behavior when an URC is presented.|
|---|---|
||"off" No change. Ring indicator remains inactive.|
||"pulse"<br>Pulse. Pulse width is determined by**<pulse_duration>**.|
|**<pulse_duration>**|Integer type. Set pulse width. Range: 1–2000. Default value: 120. Unit: ms.|
|<br>|This parameter is only valid when**<typeri>**is "pulse". If this parameter is not<br>needed, it can be set as null.|
|**<pulse_count>**|Integer type. Pulse count. It is valid only when**<typeri>**is "pulse". Range: 1–5.<br>Default value: 1. Interval between two pulses is equal to**<pulse_duration>**.|
|**<err>**<br>|Error codes. For more details, see**_Chapter 12.4_**.|



###### **4.4.2. AT+QCFG="urc/ri/smsincoming"  Set RI Behavior When Incoming SMS URCs** 

###### **are Presented** 

This command specifies the RI (ring indicator) behavior when the following URCs are presented: **+CMTI** , **+CMT** , **+CDS** and **+CBM** . 

|**AT+QCFG="urc/ri/smsincoming"** <br>**Presented**|**Set RI Behavior When Incoming SMS URCs are**|
|---|---|
|Write Command|Response|
|**AT+QCFG="urc/ri/smsincoming"[,<typ**<br>**eri>[,<pulse_duration>[,<pulse_count**<br>**>]]]**|If the optional parameters are omitted, query the current<br>setting:<br>**+QCFG: "urc/ri/smsincoming",<typeri>,<pulse_duratio**<br>**n>,<pulse_count>**|
||**OK**|
||If any of the optional parameters is specified, set the RI<br>behavior when incoming SMS URCs are presented:<br>**OK**<br>Or|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    47 / 207** 

**LTE Standard Module Series** 



||**ERROR**|
|---|---|
||If there is any error related to ME functionality:<br>**+CME ERROR: <err>**|
|Maximum Response Time|300 ms|
|Characteristics|This command takes effect immediately.<br>The configuration is saved automatically.|



###### **Parameter** 

|**<typeri>**|String type. RI behavior when URCs are presented.|
|---|---|
||"off" No change. Ring indicator remains inactive.|
||"pulse" Pulse. Pulse width is determined by**<pulse_duration>**.|
|**<pulse_duration>**|Integer type. Set pulse width. Range: 1–2000. Default value: 120.<br>Unit: ms. This parameter is only valid when**<typeri>**is "pulse".|
|**<pulse_count>**|<br>Integer type. Pulse count. It is valid only when**<typeri>**is  "pulse".|
||Range: 1–5. Default value: 1. Interval between two pulses are equal to<br>**<pulse_duration>**.|
|**<err>**|Error codes. For more details,see**_Chapter 12.4_**.|



###### **4.4.3. AT+QCFG="urc/ri/other"  Set RI Behavior When Other URCs are Presented** 

This command specifies the RI (ring indicator) behavior when other URCs are presented. 

|**AT+QCFG="urc/ri/other"  Set RI B**|**ehavior When Other URCs are Presented**|
|---|---|
|Write Command|Response|
|**AT+QCFG="urc/ri/other"[,<typeri>[,<p**<br>**ulse_duration>[,<pulse_count>]]]**|If the optional parameters are omitted, query the current<br>setting:<br>**+QCFG: "urc/ri/other",<typeri>,<pulse_duration>,**<br>**<pulse_count>**|
||**OK**|
||If the optional parameters are specified, set the RI behavior<br>when other URCs are presented:<br>**OK**<br>Or<br>**ERROR**|
||If there is any error related to ME functionality:<br>**+CME ERROR: <err>**|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    48 / 207** 

**LTE Standard Module Series** 



|Maximum Response Time|300 ms|
|---|---|
|Characteristics|This command takes effect immediately.<br>The configuration is saved automatically.|



###### **Parameter** 

|**<typeri>**|String type. RI behavior when URCs are presented。|
|---|---|
||"off"<br>No change. Ring indicator keeps inactive.|
||"pulse"<br>Pulse. Pulse width is determined by**<pulse_duration>**.|
|**<pulse_duration>**|Integer type. Set the width of pulse. Range: 1–2000. Default value: 120. Unit: ms.<br>It is valid only when**<typeri>**is "pulse".|
|**<pulse_count>**|Integer type. Pulse count. It is valid only when**<typeri>**is "pulse". Range: 1–5.|
||Default value: 1. Interval between two pulses is equal to**<pulse_duration>**.|
|**<err>**|Error codes. For more details,see**_Chapter 12.4_**.|



###### **4.4.4.  AT+QCFG="urc/ri/level"  Set Default Level of MAIN_RI Pin** 

This command sets the default level of MAIN_RI pin. 

|**AT+QCFG="urc/ri/level"  Set D**|**efault Level of MAIN_RI Pin**|
|---|---|
|Write Command|Response|
|**AT+QCFG="urc/ri/level"[,<level>]**|If the optional parameter is omitted, query the current setting:<br>**+QCFG: "urc/ri/level",<level>**|
||**OK**|
||If the optional parameter is specified, set the default level of<br>RI pin<br>**OK**<br>Or<br>**ERROR**|
||If there is any error related to ME functionality:<br>**+CME ERROR: <err>**|
|Maximum Response Time|300 ms|
|Characteristics|This command takes effect immediately.<br>The configuration is saved automatically.|



###### **Parameter** 

**<level>** Integer type. Default level of RI pin. 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    49 / 207** 

**LTE Standard Module Series** 



- 0 low level 

- <u>1</u> high level 

**<err>** Integer type. Error code. For more details, see **_Chapter 12.4_** . 

###### **4.4.5.  AT+QCFG="cmux/urcport"  Set Output Port of CMUX URCs** 

This command sets the output port of CMUX URCs or queries the current setting. 

|**AT+QCFG="cmux/urcport"  Set**|**Output Port of CMUX URCs**|
|---|---|
|Write Command|Response|
|**AT+QCFG="cmux/urcport"[,<URC_p**<br>**ort>]**|If the optional parameter is omitted, query the current setting:<br>**+QCFG: "cmux/urcport",<URC_port>**|
||**OK**|
||If the optional parameter is specified, set the output port of<br>CMUX URCs:<br>**OK**<br>Or<br>**ERROR**|
||If there is any error related to ME functionality:<br>**+CME ERROR: <err>**|
|Maximum Response Time|300 ms|
|Characteristics|The command takes effect immediately.<br>The configuration is saved automatically.|



###### **Parameter** 

|**<URC_port>**|Int|eger type. Output port of CMUX URCs.|
|---|---|---|
||0|All ports|
||1|Virtual port 1|
||2|Virtual port 2|
||3|Virtual port 3|
||4|Virtual port 4|
|**<err>**|Int|eger type. Error code. For more details,see**_Chapter 12.4_**.|



###### **4.4.6.  AT+QCFG="cmux/baudratework"  Set Baud Rate in CMUX Mode** 

This Command sets the baud rate in CMUX mode. 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    50 / 207** 

**LTE Standard Module Series** 



|**AT+QCFG="cmux/baudratework"**|**Set Baud Rate Configuration in CMUX Mode**|
|---|---|
|Write Command|Response|
|**AT+QCFG="cmux/baudratework"[,<e**<br>**nable>]**|If the optional parameter is omitted, query the current setting:<br>**+QCFG: "cmux/baudratework"[,<enable>]**|
||**OK**|
||If the optional parameter is specified, set the baud rate in<br>CMUX mode:<br>**OK**<br>Or<br>**ERROR**|
||If there is any error related to ME functionality:<br>**+CME ERROR: <err>**|
|Maximum Response Time|300 ms|
|Characteristics|This command takes effect immediately.<br>The configuration is saved automatically.|



###### **Parameter** 

**<enable>** Integer type. Whether it is allowed to set the CMUX baud rate. 

   - <u>0</u> Not allowed 1 Allowed 

- **<err>** Integer type. Error code. For more details, see **_Chapter 12.4_** . 

###### **4.4.7. AT+QCFG="urc/delay"  Set Delay Time of URC Indication** 

This command sets the delay time of URC indication starting from the end of RI pulse. 

|**AT+QCFG="urc/delay"  Set De**|**lay Time URC Indication Output**|
|---|---|
|Write Command<br>**AT+QCFG="urc/delay"[,<time>]**|Response<br>If the optional parameter is omitted, query the current setting:<br>**+QCFG: "urc/delay",<time>**<br>**OK**|
||If the optional parameter isspecified, set the delay time of<br>URC indication starting from the end of RI pulse:<br>**OK**<br>Or|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    51 / 207** 

**LTE Standard Module Series** 



||**ERROR**|
|---|---|
||If there is any error related to ME functionality:<br>**+CME ERROR: <err>**|
|Maximum Response Time|300 ms|
|Characteristics|This command takes effect immediately.<br>The configuration is saved automatically.|



###### **Parameter** 

**<time>** Integer type. Delay time of URC indication starting from the end of RI pulse: It is valid only when the RI pulse is greater than or equal to 0. Range: 0–120. Unit: millisecond. Default value: 0, which means no delay. **<err>** Error codes. For more details, see **_Chapter 12.4_** . 

###### **4.4.8. AT+QCFG="urc/cache"  Enable/Disable URC Cache** 

|**AT+QCFG="urc/cache"  Enable**|**/Disable URC Cache**|
|---|---|
|Write Command|Response|
|**AT+QCFG="urc/cache"[,<enable>]**|If the optional parameter is omitted, query the current setting:<br>**+QCFG: "urc/cache",<enable>**|
||**OK**|
||If the optional parameter is specified, enable/disable URC<br>cache:<br>**OK**<br>Or<br>**ERROR**|
||If there is any error related to ME functionality:<br>**+CME ERROR: <err>**|
|Maximum Response Time|300 ms|
|Characteristics|This command takes effect immediately.<br>The configuration is not saved.|



###### **Parameter** 

**<enable>** Integer type. Whether to enable/disable URC cache. <u>0</u> Disable 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    52 / 207** 

**LTE Standard Module Series** 



1 Enable 

**<err>** Error codes. For more details, see **_Chapter 12.4_** . 

###### **Example** 

**<mark>AT+QCFG="urc/cache" +QCFG: "urc/cache",0</mark>** <mark>//Disable URC cache.</mark> **<mark>OK AT+QCFG="urc/cache",1</mark>** <mark>//Enable URC cache.</mark> **<mark>OK AT+QCFG="urc/cache" +QCFG: "urc/cache",1 OK</mark>** 

<mark>//Make a call and send two messages to the module.</mark> **<mark>AT+QCFG="urc/cache",0</mark>** <mark>//Disable URC cache.</mark> **<mark>OK</mark>** 

###### **4.4.9. AT+QCFG="urc/port"  Set URC Output Port** 

|**AT+QCFG="urc/port"Set URC O**|**utput Port**|
|---|---|
|Write Command|Response|
|**AT+QCFG="urc/port"[,<URC_port_va**<br>**lue>]**|if the optional parameter is omitted, query the current setting:<br>**+QCFG: "urc/port",<URC_port_value>**|
||**OK**|
||If the optional parameter is specified, set URC output port:<br>**OK**<br>Or<br>**ERROR**|
|Maximum Response Time|300ms|
|Characteristics|This command takes effect immediately.<br>The configuration is not saved.|



###### **Parameter** 

**<URC_port_value>** String type .URC output port. "usbat" USB AT port 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    53 / 207** 

**LTE Standard Module Series** 



"usbmodem" USB Modem port "uart1" UART port "all" all port 

###### **Example** 

**<mark>AT+QCFG=? +QCFG: "urc/port",("usbat","usbmodem","uart1","all")</mark>** 

**<mark>OK AT+QCFG="urcport"</mark>** <mark>//Query the current URC output port.</mark> **<mark>+QCFG: "urc/port","usbat" OK AT+QCFG="urc/port","usbmodem"</mark>** <mark>//Set the URC output port to USB Modem port.</mark> **<mark>OK AT+QCFG="urc/port" +QCFG: "urc/port","usbmodem"</mark>** 

**<mark>OK</mark>** 

###### **4.4.10. AT+QCFG="risignaltype"  RI Signal Output Carrier** 

This command specifies the RI (ring indicator) signal output carrier. 

**AT+QCFG="risignaltype" RI Signal Output Carrier** 

|Write Command|Response|
|---|---|
|**AT+QCFG="risignaltype",[<RI_signal_**<br>**type>]**|If the optional parameter is omitted, query the current<br>setting:<br>**+QCFG: "risignaltype",<RI_signal_type>**|
||**OK**|
||If the optional parameter is specified, set the RI signal output<br>carrier:<br>**OK**|
||Or|
||**ERROR**|
||If there is any error related to ME functionality:<br>**+CME ERROR: <err>**|
|Maximum Response Time|300 ms|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    54 / 207** 

**LTE Standard Module Series** 



This command takes effect immediately. Characteristics The configuration is saved automatically. 

###### **Parameter** 

|**<RI_signal_type>**|String type.<br>"respective"|RI signal output carrier.<br>Ring indicator is sent over the port through which URC is presented<br>and the port can be queried with**AT+QURCCFG="urcport"**. See<br>**_Chapter 2.24_** for details about the command.|
|---|---|---|
|||If URC is presented on UART port, it is a physical ring line.|
|||If URC is presented on USB modem port, it is a virtual ring line.<br>If URC is presented on USB AT port, no ring line for USB AT port|
|||because it does not support ring lines.|
||"physical"|No matter which port URC is presented on, the RI pin will have a<br>ring jump.|
|**<err>**|Error codes.|For more details,see**_Chapter 12.4_**.|



###### **Example** 

**<mark>AT+QCFG="risignaltype" +QCFG: "risignaltype","respective" OK AT+QCFG="risignaltype","physical" OK AT+QCFG="risignaltype" +QCFG: "risignaltype","physical" OK</mark>** 

###### **4.4.11. AT+QCFG="roamservice"  Enable/Disable Roaming Service** 

This command enables or disables the roaming service. 

**AT+QCFG="roamservice" Enable/Diable Roaming Service** 

|Write Command|Response|
|---|---|
|**AT+QCFG="roamservice"[,<roam_mo**|If the optional parameters are omitted, query the current|
|**de>[,<effect>]]**|setting:|
||**+QCFG: "roamservice",<roam_mode>**|
||**OK**|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    55 / 207** 

**LTE Standard Module Series** 



||If<br>any<br>of<br>the<br>optional<br>parameters<br>are<br>enable/disable the roaming service:<br>**OK**<br>Or<br>**ERROR**|specified,|
|---|---|---|
||If there is any error related to ME functionality:<br>**+CME ERROR: <err>**||
|Maximum Response Time|300 ms||
|Characteristics|**<effect>**determines when will the command take<br>The configuration is saved automatically.|effect.|



###### **Parameter** 

|**<roam_mode>**|Integer type. Whether to enable/disable roaming service.|
|---|---|
||1     Disable roaming service|
||2     Enable roaming service|
||255Auto mode|
|**<effect>**|<br>Integer type. When the command take effect.|
||0     Take effect after UE reboots|
||1Take effect immediately|
|**<err>**|Error codes. For more details, see**_Chapter 12.4_**.|



###### **4.4.12. AT+QCFG="band"  Configure Band** 

This command specifies the preferred frequency bands for UE network searching. 

|**AT+QCFG="band"** **Configure Ban**|**d**|
|---|---|
|Write Command|Response|
|**AT+QCFG="band"[,<bandval>,<lteban**<br>**dval>**|If the optional parameters are omitted, query the current<br>setting:<br>**+QCFG: "band",<bandval>,<ltebandval>**|
||**OK**|
||If the optional parameters are specified, set the band:<br>**OK**<br>Or<br>**ERROR**|
||If there is any error related to ME functionality:<br>**+CME ERROR: <err>**|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    56 / 207** 

**LTE Standard Module Series** 



|Maximum Response Time|300 ms|
|---|---|
|Characteristics|This command takes effect immediately.<br>The configuration is saved automatically.|



###### **Parameter** 

|**<bandval>**|A hexadecimal value that specifies the LTE frequency bands.||
|---|---|---|
||0<br>No change||
|**<ltebandval>**|A hexadecimal value that specifies the LTE frequency band. If it is<br>0x40000000, it means not to change LTE frequency band.|set to 0 or|
||(e.g.: 0x15 = 0x1 (LTE B1) + 0x4 (LTE B3) + 0x10 (LTE B5)||
||0<br>|No change|
||0x1 (CM_BAND_PREF_LTE_EUTRAN_BAND1)<br>|LTE B1|
||0x2 (CM_BAND_PREF_LTE_EUTRAN_BAND2)|LTE B2|
||0x4 (CM_BAND_PREF_LTE_EUTRAN_BAND3)|LTE B3|
||0x8 (CM_BAND_PREF_LTE_EUTRAN_BAND4)|LTE B4|
||0x10 (CM_BAND_PREF_LTE_EUTRAN_BAND5)|LTE B5|
||0x40 (CM_BAND_PREF_LTE_EUTRAN_BAND7)|LTE B7|
||0x80 (CM_BAND_PREF_LTE_EUTRAN_BAND8)|LTE B8|
||0x800 (CM_BAND_PREF_LTE_EUTRAN_BAND12)<br>|LTE B12|
||0x1000 (CM_BAND_PREF_LTE_EUTRAN_BAND13)|LTE B13|
||0x2000 (CM_BAND_PREF_LTE_EUTRAN_BAND14)|LTE B14|
||0x20000 (CM_BAND_PREF_LTE_EUTRAN_BAND18)|LTE B18|
||0x40000 (CM_BAND_PREF_LTE_EUTRAN_BAND19)|LTE B19|
||0x80000 (CM_BAND_PREF_LTE_EUTRAN_BAND20)<br>|LTE B20|
||0x1000000 (CM_BAND_PREF_LTE_EUTRAN_BAND25)<br>|LTE B25|
||0x2000000 (CM_BAND_PREF_LTE_EUTRAN_BAND26)<br>|LTE B26|
||0x8000000 (CM_BAND_PREF_LTE_EUTRAN_BAND28)<br>|LTE B28|
||0x200000000 (CM_BAND_PREF_LTE_EUTRAN_BAND34)|LTE B34|
||0x2000000000 (CM_BAND_PREF_LTE_EUTRAN_BAND38)|LTE B38|
||0x4000000000 (CM_BAND_PREF_LTE_EUTRAN_BAND39)|LTE B39|
||0x8000000000 (CM_BAND_PREF_LTE_EUTRAN_BAND40)|LTE B40|
||0x10000000000 (CM_BAND_PREF_LTE_EUTRAN_BAND41)|LTE B41|
||0x 20000000000000000 (CM_BAND_PREF_LTE_EUTRAN_BAND66)|LTE B66|
||0x400000000000000000 (CM_BAND_PREF_LTE_EUTRAN_BAND71)|LTE B71|
||0x7FFFFFFFFFFFFFFF(CM_BAND_PREF_ANY)           Any frequ|ency band|
|**<err>**|Error codes. For more details, see**_Chapter 12.4_**.||



###### **4.4.13. AT+QCFG="nwoptmz/acq"  Save Searched Cell Database to NV Automatically** 

This command saves the searched sell database to NV automatically. 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    57 / 207** 

**LTE Standard Module Series** 



###### **AT+QCFG="nwoptmz/acq"  Save Searched Cell Database to NV Automatically** 

|Write Command|Response|
|---|---|
|**AT+ QCFG="nwoptmz/acq"[,<flag>,<interval>]**|If the optional parameters are omitted, query the<br>current setting:<br>**+QCFG: "nwoptmz/acq",<flag>,<interval>**|
||**OK**|
||If any of the optional parameter is specified,<br>enable/disable saving of the searched cell<br>database to NV automatically:<br>**OK**<br>Or<br>**ERROR**|
|Characteristics|This command takes effect immediately.<br>The configuration is saved automatically.|



###### **Parameter** 

**<flag>** Integer type. Enable/Disable saving of searched cell database to NV automatically. <u>0</u> Disable 1 Enable **<interval>** Integer type. Interval for checking for database changes. Range: 60–16777200. Default value: 3600. Unit: second. 

###### **NOTE** 

1. If **<flag>** is set to 0, it will disable saving of searched cell database to NV automatically and the database will be cleared. 

2. If this function is enabled, the module prefers to select one cell in database to register after power-on. If all cells in the database are exhausted and the module still fails to register to the network, the module will search the network by scanning the full frequency. 

###### **Example** 

**<mark>AT+QCFG="nwoptmz/acq",1,3600 OK AT+QCFG="nwoptmz/acq" +QCFG: “nwoptmz/acq”,1,3600 OK</mark>** 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    58 / 207** 

**LTE Standard Module Series** 



###### **4.4.14. AT+QCFG="usbnet"  Set Network Card Interface Type** 

This command sets the network card interface type. 

|**AT+QCFG="usbnet"  Set Net**|**work Card Interface Type**|
|---|---|
|Write Command|Response|
|**AT+QCFG="usbnet"[,<net>]**|If the optional parameter is omitted,query the current setting:<br>**+QCFG: "usbnet",<net>**|
||**OK**|
||If theoptionalparameter isspecified, set the network card<br>interface type:<br>**OK**<br>Or<br>**ERROR**|
||If error is related to ME functionality:<br>**+CME ERROR:<err>**|
|Maximum Response Time|300 ms|
|Characteristics|The command takes effect after rebooting;<br>The configuration is saved automatically.|



###### **Parameter** 

**<net>** Integer type. Network card interface type. 1  ECM interface 3  RNDIS interface **<err>** Error codes. For more details, see **_Chapter 12.4_** . 

###### **4.4.15. AT+QCFG="aprstlevel"  Set AP Dump Level** 

This command queries or sets AP dump level. 

|**AT+QCFG="aprstlevel"  Set AP**|**Dump Level**|
|---|---|
|Write Command|Response|
|**AT+QCFG="aprstlevel"[,<level>]**|If the optional parameter is omitted, query the current setting:<br>**+QCFG: "aprstlevel",<level>**|
||**OK**|
||If the optionalparameter is specified, set AP dumplevel:|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    59 / 207** 

**LTE Standard Module Series** 



||**OK**<br>Or<br>**ERROR**|
|---|---|
|Maximum Response Time|300 ms|
|Characteristics|The command takes effect after the module is rebooted.<br>The configuration is saved automatically.|



###### **Parameter** 

- **<level>** Integer type. AP dump level. 

   - 0 Open AP dump. Module reboots and enters dump mode when an AP dump happens. 

   - <u>1</u> Close AP dump. Module only reboots when an AP dump happens. 

###### **4.4.16. AT+QCFG="modemrstlevel"  Set CP Dump Level** 

This command queries or sets CP (i.e., Modem) dump level. 

|**AT+QCFG="modemrstlevel"  Set**|**CP Dump Level**|
|---|---|
|Write Command|Response|
|**AT+QCFG="modemrstlevel"[,<level>]**|If the optional parameter is omitted, query the current setting:<br>**+QCFG: "modemrstlevel",<level>**|
||**OK**|
||If the optional parameter is specified, set CP dump level:<br>**OK**<br>Or<br>**ERROR**|
|Maximum Response Time|300 ms|
|Characteristics|The command takes effect after the module is rebooted.<br>The configuration is saved automatically.|



###### **Parameter** 

- **<level>** Integer type. CP dump level. 

   - 0 Open CP dump. Module reboots and enters dump mode when a CP dump happens. <u>1</u> Close CP dump. Module only reboots when a CP dump happens. 

###### **NOTE** 

To ensure the module enters dump mode when either AP dump or CP dump happens, you must reboot the module after both **AT+QCFG="aprstlevel",0** and **AT+QCFG="modemrstlevel",0** are executed 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    60 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

successfully. 

###### **4.4.17. AT+QCFG="ntp"  Set Maximum NTP Re-transmission Count and Interval** 

This command sets the maximum re-transmission times and the interval of NTP. 

|**AT+QCFG="ntp"  Set Maximum**|**NTP Re-transmission Count and Interval**|
|---|---|
|Write Command|Response|
|**AT+QCFG="ntp"[,<cnt>,<interval>]**|If the optional parameters are omitted, query the current<br>setting:<br>**+QCFG: "ntp",<cnt>,<interval>**|
||**OK**|
||If the optional parameters are specified, set the maximum<br>re-transmission count and the interval for NTP:<br>**OK**<br>Or<br>**ERROR**|
|Maximum Response Time|300 ms|
|Characteristic|The command takes effect immediately.<br>The configurations are saved automatically.|



###### **Parameter** 

**<cnt>** Integer type. NTP re-transmission count. Range: 1–10. Default value: 3. **<interval>** Integer type. NTP re-transmission interval. Range: 5–60. Default value: 15. 

###### **Example** 

**<mark>AT+QCFG="ntp"</mark>** <mark>//Query the current setting.</mark> **<mark>+QCFG: "ntp",3,15 OK AT+QCFG="ntp",5,20</mark>** <mark>//Set NTP re-transmission count and interval.</mark> **<mark>OK AT+QCFG="ntp"</mark>** <mark>//Query the current setting.</mark> **<mark>+QCFG: "ntp",5,20 OK</mark>** 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    61 / 207** 

**LTE Standard Module Series** 



###### **4.4.18. AT+QCFG="ledmode"  Set NET_STATUS Output Mode** 

This command queries or sets the output mode of NET_STATUS pin. 

|**AT+QCFG="ledmode"  Set NE**|**T_STATUS Output Mode**|
|---|---|
|Write Command|Response|
|**AT+QCFG="ledmode"[,<mode>]**|If the optional parameter is omitted, query the current setting:<br>**+QCFG: "ledmode",<mode>**|
||**OK**|
||If the optional parameter is specified, set the output mode of<br>NET_STATUS pin:<br>**OK**<br>Or<br>**ERROR**|
||If there is any error related to ME functionality:<br>**+CME ERROR: <err>**|
|Maximum Response Time|300 ms|
|Characteristic|The command takes effect immediately.<br>The configuration is saved automatically.|



###### **Parameter** 

**<mode>** Integer type. Output mode of NET_STATUS pin. <u>0</u> Flicker mode. Network searching: LED is on for 200 ms (high level); LED is off for 1800 ms (low level) Idle: LED is on for 1800 ms (high level); LED is off for 200 ms (low level) Data transmission: LED is on for 125 ms (high level); LED is off for 125 ms (low level) 1 Output high level when attaching to the network and low level in other cases. 2 Custom setting of output mode of NET_STATUS pin. **<err>** Integer type. Error code. For more details, see **_Chapter 12.4_** . 

###### **Example** 

**<mark>AT+QCFG="ledmode",1</mark>** <mark>//Output high level when attaching to the network and low level in other cases.</mark> **<mark>OK AT+QCFG="ledmode"</mark>** <mark>//Query the current setting.</mark> **<mark>+QCFG: "ledmode",1</mark>** 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    62 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

**<mark>OK</mark>** 

###### **4.4.19. AT+QCFG="nat"  Set NIC Working Mode** 

This command sets the NIC working mode. 

|**AT+QCFG="nat"  Set NIC W**|**orking Mode**|
|---|---|
|Write Command|Response|
|**AT+QCFG="nat"[,<nat>]**|If the optional parameter is omitted, query the current setting:<br>**+QCFG: "nat",<nat>**|
||**OK**|
||If the optional parameter is specified, set the NIC working<br>mode:<br>**OK**<br>Or<br>**ERROR**|
||If there is any error related to ME functionality:<br>**+CME ERROR: <err>**|
|Maximum Response Time|300 ms|
|Characteristics|The command takes effect after rebooting;<br>The configuration is saved automatically.|



###### **Parameter** 

**<nat>** Integer type. NIC working mode. 

- <u>0</u> Routing mode 

- 1 NIC mode 

**<err>** Integer type. Error code. For more details, see **_Chapter 12.4_** . 

###### **Example** 

**<mark>AT+QCFG="nat",1</mark>** <mark>//Set NIC working mode to NIC mode.</mark> **<mark>OK</mark>** 

###### **4.4.20. AT+QCFG="netmaskset"  Enable/Disable Customized Netmask** 

This command enables or disables setting of the netmask. 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    63 / 207** 

**LTE Standard Module Series** 



###### **AT+QCFG="netmaskset"  Enable/Disable Customized Netmask** 

|Write Command|Response|
|---|---|
|**AT+QCFG="netmaskset"[,<enable**<br>**>[,<netmask>]]**|If the optional parameters are omitted, query the current setting:<br>**+QCFG: "netmaskset",<enable>**|
||**OK**|
||If any of the optional parameters is specified, enable or disable<br>customized netmask:<br>**OK**<br>Or|
||**ERROR**|
|Maximum Response Time|300 ms|
|Characteristic|This command takes effect after reconnecting the network card.<br>The configurations is saved automatically.|



###### **Parameter** 

**<enable>** Integer type. Enable/disable customized netmask. 

- <u>0</u> Disable. Netmask is set by the rule inside the module. 

- 1 Enable. Netmask is set by **<netmask>** . 

**<netmask>** String type. Customized netmask. It is valid only when **<enable>** =1. Example: "255.255.255.0". 

###### **Example** 

**<mark>AT+QCFG="netmaskset"</mark>** <mark>//Query the current setting.</mark> **<mark>+QCFG: "netmaskset",0</mark>** <mark>//Customized netmask is disabled.</mark> **<mark>OK AT+QCFG="netmaskset",1,"255.255.255.0"</mark>** <mark>//Enable customized netmask and set netmask.</mark> **<mark>OK AT+QCFG="netmaskset" +QCFG: "netmaskset",1 OK AT+QCFG="netmaskset",0</mark>** <mark>//Disable customized netmask and set netmask.</mark> **<mark>OK</mark>** 

###### **4.4.21. AT+QCFG="ppp/termframe"  Enable/Disable PPP TERM Frame Sending** 

This command enables/disables the PPP TERM frame sending when PPP connection is terminated by 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    64 / 207** 

**LTE Standard Module Series** 



the module. 

|**AT+QCFG="ppp/termframe"  Ena**|**ble/Disable PPP TERM Frame Sending**|
|---|---|
|Write Command|Response|
|**AT+QCFG="ppp/termframe"[,<flag>]**|If the optional parameter is omitted,query the current setting:<br>**+QCFG: "ppp/termframe",<flag>**|
||**OK**|
||If the optional parameter isspecified, enable/disable PPP<br>TERM frame sending:<br>**OK**<br>Or<br>**ERROR**|
||If there is any error related to ME functionality:<br>**+CME ERROR:<err>**|
|Maximum Response Time|300 ms|
|Characteristics|The command takes effect after rebooting.<br>The configuration is saved automatically.|



###### **Parameter** 

**<flag>** Integer type. Enable/disable TERM frame sending when PPP connection is terminated by the module . 

- <u>0   Disable</u> 

1   Enable 

- **<err>** Integer type. Error code. For more details, see **_Chapter 12.4_** . 

###### **NOTE** 

If **AT+QPPPDROP** hangs up the PPP connection with a TERM frame, module sends the TERM frame to MCU regardless of whether **<flag>** is 0 or 1. 

###### **Example** 

**<mark>AT+QCFG="ppp/termframe",1</mark>** <mark>//Enable PPP TERM frame sending:</mark> **<mark>OK AT+QCFG="ppp/termframe"</mark>** <mark>//Query the current setting.</mark> **<mark>+QCFG: "ppp/termframe",1 OK</mark>** 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    65 / 207** 

**LTE Standard Module Series** 



###### **4.4.22. AT+QCFG="rf/tuner_cfg"  Set Mapping Between RF Tuner and RF Bands** 

This command sets the mapping between RF tuner and RF bands. 

|**AT+QCFG="rf/tuner_cfg"  Set Ma**|**pping Between RF Tuner and RF Bands**|
|---|---|
|Write Command|Response|
|**AT+QCFG="rf/tuner_cfg"[,<index>,<lt**<br>**ebands>]**|If the optional parameters are omitted, query the current<br>setting:<br>**+QCFG: "rf/tuner_cfg",<index>,<ltebands>**|
||**OK**|
||If the optional parameters are specified, set the mapping<br>between RF tuner and RF bands:<br>**OK**<br>Or<br>**ERROR**|
|Maximum Response Time|300 ms|
|Characteristics|This command takes effect after rebooting.<br>The configuration is saved automatically.|



###### **Parameter** 

- **<index>** Integer type. Pin level status. Range: 0–3 <u>0</u> FEM6 at low level, FEM5 at low level 1 FEM6 at low level, FEM5 at high level 2 FEM6 at high level, FEM5 at low level 3 FEM6 at high level, FEM5 at high level 

- **<ltebands>** String type. LTE bands, separated by commas. (e.g., "1,3,5", etc.) 

###### **NOTE** 

1. For EG800Q series module, FEM5 corresponds to pin 105 and FEM6 corresponds to pin 104. 

2. For EG91xQ family modules, FEM5 corresponds to pin 76 and FEM6 corresponds to pin 77. 

###### **Example** 

**<mark>AT+QCFG="rf/tuner_cfg"</mark>** <mark>//Query the current setting.</mark> **<mark>+QCFG: "rf/tuner_cfg",0,"1,2,3,4,8,25,39" "rf/tuner_cfg",1,"5,18,19,20,26" "rf/tuner_cfg",2,"12,13,28" "rf/tuner_cfg",3,"7,38,40,41"</mark>** 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    66 / 207** 

**LTE Standard Module Series** 



**<mark>OK</mark>** <mark>//Set the LTE bands, i.e., FEM 6 at high level and FEM 5 at low level.</mark> **<mark>AT+QCFG="rf/tuner_cfg",2,"2,12,13,25,28" OK AT+QCFG="rf/tuner_cfg"</mark>** <mark>//Query the current setting.</mark> **<mark>+QCFG: "rf/tuner_cfg",0,"1,3,4,8,39" "rf/tuner_cfg",1,"5,18,19,20,26" "rf/tuner_cfg",2,"2,12,13,25,28" "rf/tuner_cfg",3,"7,38,40,41" OK</mark>** 

###### **4.4.23. AT+QCFG="tcp/windowsize"  Set TCP Window Available Size** 

This command sets the available size of TCP window when sending/receiving data. 

|**AT+QCFG="tcp/windowsize"  Se**|**t TCP Window Available Size**|
|---|---|
|Write Command|Response|
|**AT+QCFG="tcp/windowsize",<buffer**<br>**>[,<window_size>]**|If the optional parameter is omitted, query the current setting:<br>**+QCFG: "tcp/windowsize",<buffer>,<window_size>**|
||**OK**|
||If the optional parameter is specified, set TCP window<br>available size:<br>**OK**<br>Or<br>**ERROR**|
|Maximum Response Time|300 ms|
|Characteristic|The command takes effect immediately.<br>The configuration is not saved.|



###### **Parameter** 

|**<buffer>**|Integer type. Set whether to receive or sending buffer size.|
|---|---|
||0<br>Receive buffer size|
||1<br>Send buffer size|
|**<window_size>**|Integer type. Available TCP window size. Range: 16–100. Default value: 16.|
|**<err>**|Error codes. For more details,see**_Chapter 12.4_**.|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    67 / 207** 

**LTE Standard Module Series** 



###### **4.4.24. AT+QCFG="TCP/SendMode"  Set TCP Sending Mode** 

This command configures TCP sending mode. 

|**AT+QCFG="TCP/SendMode"  Set T**|**CP Sending Mode**|
|---|---|
|Write Command|Response|
|**AT+QCFG="TCP/SendMode"[,<mode>]**|If optional parameter is omitted, query the current setting:<br>**+QCFG: "TCP/SendMode",<mode>**|
||**OK**|
||If the optional parameter is specified, set TCP sending<br>mode:<br>**OK**<br>Or<br>**ERROR**|
||If error is related to ME functionality:<br>**+CME ERROR: <err>**|
|Maximum Response Time|300 ms|
|Characteristic|The command takes effect immediately.<br>The configuration is not saved.|



###### **Parameter** 

- **<mode>** Integer type. Set when to send **SEND OK** . 

   - <u>0</u> **SEND OK** is sent immediately after TCP socket sends data from serial port. 

   - 1 **SEND OK** is sent after receiving an ACK from the remote TCP socket. 

   - 2 **<SocketID>,SEND OK** is sent after receiving an ACK from the remote TCP socket. 

- **<err>** Error codes. For more details, see **_Chapter 12.4_** . 

###### **Example** 

**<mark>AT+QCFG="TCP/SendMode" +QCFG: "TCP/SendMode",0</mark>** 

**<mark>OK AT+QCFG="TCP/SendMode",1 OK</mark>** 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    68 / 207** 

**LTE Standard Module Series** 



###### **4.4.25. AT+QCFG="fast/poweroff"  Enable/Disable Fast Power-Off** 

This command enables or disables fast power-off. 

|**AT+QCFG="fast/poweroff"**|**Enable/Disable Fast Power-Off**|
|---|---|
|Write Command|Response|
|**AT+QCFG="fast/poweroff"[,<n>]**|<br>If the optional parameter is omitted, query the current setting:<br>**+QCFG: "fast/poweroff",<n>**|
||**OK**|
||If the optional parameter is specified, enable/disable fast<br>power-off:<br>**OK**<br>Or<br>**ERROR**|
|Maximum Response Time|300 ms|
|Characteristics|This command takes effect immediately.<br>The configuration is saved automatically.|



###### **Parameter** 

**<n>** Integer type. Enable/disable fast power-off. <u>0</u> Disable 1 Enable **<err>** Integer type. Error code. For more details, see **_Chapter 12.4_** . 

###### **Example** 

**<mark>AT+QCFG="fast/poweroff"</mark>** <mark>//Query the current setting.</mark> **<mark>+QCFG: "fast/poweroff",0 OK AT+QCFG="fast/poweroff",1</mark>** <mark>//Enable fast power-off.</mark> **<mark>OK AT+QCFG="fast/poweroff" +QCFG: "fast/poweroff",1</mark>** <mark>//Fast power-off is enabled.</mark> **<mark>OK</mark>** 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    69 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
RUECCTEL<br><!-- End of picture text -->

###### **4.4.26. AT+QCFG="fota/cid"  Set PDP Context ID for FOTA Download** 

This command sets the PDP context ID for FOTA download. 

|**AT+QCFG="fota/cid"  Set PDP C**|**ontext ID for FOTA Download**|
|---|---|
|Write Command|Response|
|**AT+QCFG="fota/cid"[,<contextID>]**|If the optional parameter is omitted, query the current<br>setting:<br>**+QCFG: "fota/cid",<contextID>**|
||**OK**|
||If the optional parameter is specified, set PDP context ID for<br>FOTA download:<br>**OK**<br>Or<br>**ERROR**|
||If there is any error related to ME functionality:<br>**+CME ERROR: <err>**|
|Maximum Response Time|300 ms|
|Characteristics|This command takes effect immediately.<br>The configuration is not saved.|



###### **Parameter** 

**<contextID>** Integer type. Context ID. Range: 1–15. Default value: 1. **<err>** Integer type. Error code. For more details, see **_Chapter 12.4_** . 

###### **4.4.27. AT+QCFG="qcautoconnect"  Disable/enable Auto Connect Function in ECM** 

###### **Mode** 

This command disable/enable the auto connect function (data call) when USB mode is ECM. 

|**AT+QCFG="qcautoconnect"**|**Set Auto Connect Function in ECM Mode**|
|---|---|
|Write Command|Response|
|**AT+QCFG="qcautoconnect"[,<dat**|If the optional parameter is omitted, query the current setting:|
|**a_format>]**|**+QCFG: "qcautoconnect",<data_format>**|
||**OK**|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    70 / 207** 

**LTE Standard Module Series** 



||If the optional parameter is specified, set the auto connect|
|---|---|
||function in ECM mode:|
||**OK**|
||Or|
||**ERROR**|
|Maximum Response Time|300 ms|
|Characteristics|/|



###### **Parameter** 

**<data_format>** Integer type. Disable/enable auto connect function in ECM mode. 

- 0 Disable auto connect 

- 1 Enable auto connect 

###### **4.4.28. AT+QCFG="sms/listmsgmap"  List Message Map** 

This command lists the message map with **<msgtype>** in the memory storage **<mem1>** specified by **AT+CPMS** . 

###### **AT+QCFG="sms/listmsgmap"  List Message Map** 

|Write Command|Response|
|---|---|
|**AT+QCFG="sms/listmsgmap",<msgt**|**+QCFG: "sms/listmsgmap",<msgtype>,<msgmap>**|
|**ype>**||
||**OK**|
||Or|
||**ERROR**|
||If there is any error related to ME functionality:|
||**+CME ERROR: <err>**|
|Maximum Response Time|300 ms|
|Characteristics|/|



###### **Parameter** 

|**<msgtype>**|String type. Messag|e type.|
|---|---|---|
||"REC UNREAD"|Received unread messages|
||"REC READ"|Received read messages|
||"STO UNSENT"|Stored unsent messages|
||"STO SENT"|Stored sent messages|
|**<msgmap>**|String type. Bit map|of all messages in the storage specified by**<mem1>**of**AT+CPMS**.|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    71 / 207** 

**LTE Standard Module Series** 



If the bit is 1, it means the message is on the type specified by **<msgtype>** . If the bit is 0, it means the message is not on the type specified by **<msgtype>** ; The position of the bit in bit map specified by **<msgtype>** indicates the message index in the storage specified by **<mem1>** of **AT+CPMS** . **<err>** Integer type. Error code. For more details, see **_Chapter 12.4_** . 

###### **Example** 

**<mark>AT+CPMS? +CPMS: "ME",0,255,"ME",0,255,"ME",0,255</mark>** 

###### **<mark>OK</mark>** 

**<mark>AT+QCFG="sms/listmsgmap","REC UNREAD"</mark>** <mark>//List the received unread message map.</mark> **<mark>+QCFG: "sms/listmsgmap","REC UNREAD","000F5B0000"</mark>** 

**<mark>OK</mark>** 

###### **4.4.29. AT+QCFG="sms/MeNum"  Set the Number of Short Messages Stored by ME** 

This command sets the number of short messages that can be stored by ME. 

|**AT+QCFG="sms/MeNum"  Set th**|**e Number of Short Messages Stored by ME**|
|---|---|
|Write Command|Response|
|**AT+QCFG="sms/MeNum"[,<value>]**|If the optional parameter is omitted, query the current setting:<br>**+QCFG: "sms/MeNum",<value>**|
||**OK**|
||If the optional parameter is specified, set the number of<br>messages stored by ME.<br>**OK**<br>Or<br>**ERROR**|
|Maximum Response Time|300 ms|
|Characteristic|The command takes effect after the module is rebooted.<br>The configuration is saved after power-off.|



###### **Parameter** 

**<value>** Integer type. Number of messages stored by ME. Range: 0–255. Default value: 255. 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    72 / 207** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

###### **Example** 

**<mark>AT+QCFG="sms/MeNum"</mark>** <mark>//Query the setting of</mark> **<mark>sms/MeNum</mark>** <mark>.</mark> **<mark>+QCFG: "sms/MeNum",255 OK</mark>** 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    73 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

# **5 (U)SIM Related Commands** 

##### **5.1. AT+CIMI  Request International Mobile Subscriber Identity (IMSI)** 

This command requests the International Mobile Subscriber Identity (IMSI) which is intended to permit the TE to identify the individual SIM card or active application in the UICC (GSM or USIM) that is attached to MT. 

|**AT+CIMI  Request Internati**|**onal Mobile Subscriber Identity (IMSI)**|
|---|---|
|Test Command<br>**AT+CIMI=?**|Response<br>**OK**|
|Execution Command<br>**AT+CIMI**|Response<br>TA returns**<IMSI>**for identifying the individual (U)SIM<br>attached to ME.<br>**<IMSI>**|
||**OK**|
||If there is any error related to ME functionality:<br>**+CME ERROR: <err>**|
|Maximum Response Time|300 ms|
|Characteristic|/|
|Reference<br>3GPP TS 27.007||



###### **Parameter** 

**<IMSI>** International Mobile Subscriber Identity (string without double quotes) **<err>** Error codes. For more details, see **_Chapter 12.4_** . 

###### **Example** 

**<mark>AT+CIMI 460023210226023</mark>** <mark>//Query IMSI number of (U)SIM which is attached to ME</mark> **<mark>OK</mark>** 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    74 / 207** 

**LTE Standard Module Series** 



##### **5.2. AT+CLCK  Facility Lock** 

This command locks, unlocks or interrogates a MT or a network facility **<fac>** . It can be aborted when network facilities are being set or interrogated. The factory default password of PF, PN, PU, PP and PC lock is "12341234". For Write Command, **<passwd>** is normally needed to do such actions. When querying the status of network service ( **<mode>** =2), the response line for ‘not active’ case ( **<status>** =0) should be returned only if service is not active for any **<class>** . 

|**AT+CLCK  Facility Lock**||
|---|---|
|Test Command|Response|
|**AT+CLCK=?**|**+CLCK: (**list of supported**<fac>**s**)**|
||**OK**|
|Write Command|Response|
|**AT+CLCK=<fac>,<mode>[,<passwd>[**<br>**,<class>]]**|If**<mode>**is not 2 and the command is executed<br>successfully:<br>**OK**|
||If**<mode>**=2 and command is executed successfully:<br>**+CLCK: <status>[,<class>]**<br>**[+CLCK: <status>[,<class>]]**<br>**[…]**|
||**OK**|
|Maximum Response Time|5 s|
|Characteristic|The command takes effect immediately.<br>The configuration is saved automatically.|
|Reference||
|3GPP TS 27.007||



###### **Parameter** 

**<fac>** String type. "SC" (U)SIM (lock SIM/UICC card installed in the currently selected card slot) (SIM/UICC requests password at MT power-up and when this lock command issued). **<mode>** Integer type. Network service status. 0 Unlock 1 Lock 2 Query status **<passwd>** String type. Password. 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    75 / 207** 

**LTE Standard Module Series** 



|**<class>**|Integer|type.|
|---|---|---|
||1<br>|Voice|
||2|Data|
||4<br>|FAX|
||7|All telephony except SMS|
||8|Short message service|
||16|Data circuit synchronization|
||32|Data circuit asynchronization|
|**<status>**|Integer|type. Current status of the specified**<class>**.|
||0<br>|Off|
||1|On|



###### **Example** 

**<mark>AT+CLCK="SC",2</mark>** <mark>//Query the status of (U)SIM card.</mark> 

**<mark>+CLCK: 0</mark>** <mark>//The (U)SIM card is unlocked (OFF).</mark> **<mark>OK AT+CLCK="SC",1,"1234"</mark>** <mark>//Lock (U)SIM card, and the password is 1234.</mark> **<mark>OK AT+CLCK="SC",2</mark>** <mark>//Query the status of (U)SIM card.</mark> **<mark>+CLCK: 1</mark>** <mark>//The (U)SIM card is locked (ON).</mark> **<mark>OK AT+CLCK="SC",0,"1234"</mark>** <mark>//Unlock (U)SIM card. The password is 1234.</mark> **<mark>OK</mark>** 

##### **5.3. AT+CPIN  Enter PIN** 

This command is used to enter a password or queries if the module requires a password before it can be operated. The password may be (U)SIM PIN, (U)SIM PUK, PH-SIM PIN, etc. 

Read Command returns an alphanumeric string indicating whether or not some password is required. 

TA stores the password, such as (U)SIM PIN, (U)SIM PUK, etc., which is necessary for operating. If the PIN is to be entered twice, the TA automatically repeats the PIN. If no PIN request is pending, no action is taken and an error message **+CME ERROR** is returned to TE. 

If the PIN required is (U)SIM PUK or (U)SIM PUK2, the second pin is required. This second pin **<new_pin>** is used to replace the old pin in the (U)SIM. 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    76 / 207** 

**LTE Standard Module Series** 



###### **AT+CPIN  Enter PIN** 

|Test Command|Response|
|---|---|
|**AT+CPIN=?**|**OK**|
|Read Command|Response|
|**AT+CPIN?**|**+CPIN: <code>**|
||**OK**|
|Write Command|Response|
|**AT+CPIN=<pin>[,<new_pin>]**|**OK**|
||If there is any error related to ME functionality:<br>**+CME ERROR: <err>**|
|Maximum Response Time|5 s|
|Characteristics|The command takes effect immediately.<br>The configuration is saved automatically.|
|Reference||
|3GPP TS 27.007||



###### **Parameter** 

|**<code>**|String without double quotes. The password that the module requires.|
|---|---|
||READY<br>MT is not pending for any password|
||SIM PIN<br>MT is waiting for SIM PIN to be given|
||SIM PUK<br>MT is waiting for SIM PUK to be given|
||SIM PIN2<br>MT is waiting for SIM PIN2 to be given|
||SIM PUK2<br>MT is waiting for SIM PUK2 to be given|
|**<pin>**|String type. Password. If the requested password is a PUK, such as (U)SIM PUK1,|
||PH-FSIM PUK or another password, then**<pin>**must be followed by**<new_pin>**.|
|**<new_pin>**|Stringtype. Newpassword required if the requested code is a PUK.|



###### **Example** 

<mark>//Enter PIN.</mark> **<mark>AT+CPIN? +CPIN: SIM PIN</mark>** <mark>//MT is waiting for SIM PIN to be given.</mark> **<mark>OK AT+CPIN=1234</mark>** <mark>//Enter PIN.</mark> **<mark>OK +CPIN: READY AT+CPIN?</mark>** <mark>//PIN has already been entered.</mark> 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    77 / 207** 

**LTE Standard Module Series** 



**<mark>+CPIN: READY OK</mark>** <mark>//Enter PUK and PIN.</mark> **<mark>AT+CPIN? +CPIN: SIM PUK</mark>** <mark>//MT is waiting for SIM PUK to be given.</mark> **<mark>K AT+CPIN="26601934","1234"</mark>** <mark>//Enter PUK and new PIN password.</mark> **<mark>OK CPIN: READY AT+CPIN? +CPIN: READY</mark>** <mark>//PUK has already been entered.</mark> **<mark>OK</mark>** 

##### **5.4. AT+CPWD  Change Password** 

This command sets a new password for the facility lock function defined by **AT+CLCK** . 

This Test Command returns a list of pairs that are present the available facilities and the maximum length of their passwords. 

###### **AT+CPWD  Change Password** 

|Test Command|Response|
|---|---|
|**AT+CPWD=?**|**+CPWD:** **("SC",8)**|
||**OK**|
|Write Command|Response|
|**AT+CPWD=<fac>,<oldpwd>,<newpw**|**OK**|
|**d>**||
|Maximum Response Time|5 s|
|Characteristics|The command takes effect immediately.<br>The configuration is saved automatically.|
|Reference||
|3GPP TS 27.007||



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    78 / 207** 

**LTE Standard Module Series** 



###### **Parameter** 

**<fac>** String type. The facility locks. "SC"    (U)SIM (lock SIM/UICC card) (SIM/UICC asks for password at MT power-up and when this lock command is issued) 

**<pwdlength>** Integer type. Maximum length of the password. 

**<oldpwd>** String type. Password specified for the facility from the user interface or with command. 

**<newpwd>** String type. New password. 

###### **Example** 

**<mark>AT+CPIN? +CPIN: READY OK AT+CPWD="SC","1234","4321"</mark>** <mark>//Change (U)SIM card password to "4321".</mark> **<mark>OK</mark>** <mark>//Restart the module or re-activate the SIM card.</mark> **<mark>AT+CPIN?</mark>** <mark>//Query PIN code is locked.</mark> **<mark>+CPIN: SIM PIN OK AT+CPIN="4321"</mark>** <mark>//PIN must be entered to define a new password "4321".</mark> **<mark>OK +CPIN: READY</mark>** 

##### **5.5. AT+CSIM  Generic (U)SIM Access** 

This command allows a direct control of the (U)SIM installed in the selected card slot by a distant application on the TE. The TE keeps the processing (U)SIM information within the frame specified with GSM/UMTS. 

###### **AT+CSIM  Generic (U)SIM Access** 

Test Command Response **AT+CSIM=? OK** Write Command Response **AT+CSIM=<length>,<command> +CSIM: <length>,<response> OK** Or 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    79 / 207** 

**LTE Standard Module Series** 



||**ERROR**|
|---|---|
||If there is any error related to ME functionality:<br>**+CME ERROR: <err>**|
|Maximum Response Time|300 ms|
|Characteristics|The command takes effect immediately.<br>The configuration is saved automatically.|
|Reference||
|3GPP TS 27.007||



###### **Parameter** 

|**<length>**|Integer type. Length of**<command>**or**<response>**string.|
|---|---|
|**<command>**|Command transferred by the MT to the (U)SIM in the format described in_3GPP TS_|
||_51.011_.|
|**<response>**|Response to the command transferred by the (U)SIM to the MT in the format|
||described in_3GPP TS 51.011_.|
|**<err>**|Error codes. For more details, see**_Chapter 12.4_**.|



##### **5.6. AT+CRSM  Restricted (U)SIM Access** 

This command offers easy and limited access to the (U)SIM database. It transmits the (U)SIM command number **<command>** and its required parameters to the MT. 

###### **AT+CRSM  Restricted (U)SIM Access** 

|Test Command|Response|
|---|---|
|**AT+CRSM=?**|**OK**|
|Write Command|Response|
|**AT+CRSM=<command>[,<fileld>[,<P1**<br>**>,<P2>,<P3>[,<data>][,<pathld>]]]**|**+CRSM: <sw1>,<sw2>[,<response>]**<br>**OK**<br>Or<br>**ERROR**|
||If there is any error related to ME functionality:<br>**+CME ERROR: <err>**|
|Maximum Response Time|300 ms|
|Characteristic|The command takes effect immediately.<br>The configuration is saved automatically.|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    80 / 207** 



**LTE Standard Module Series** 

Reference 3GPP TS 27.007 

###### **Parameter** 

**<command>** Integer type. (U)SIM command number. 176       READ BINARY 178       READ RECORD 192       GET RESPONSE 214       UPDATE BINARY 220       UPDATE RECORD 242       STATUS **<fileId>** Integer type. Identifier of an elementary data file on (U)SIM, if used by **<command>** . **<P1>** , **<P2>** , **<P3>** Integer type. Parameters passed on by the MT to the (U)SIM. These parameters are mandatory for every command, except GET RESPONSE and STATUS. The values are described in _3GPP TS 51.011_ . **<data>** Information to be written to the (U)SIM (hexadecimal character format; refer to **AT+CSCS** ). **<pathId>** String type. Path of an elementary file on a SIM/UICC in hexadecimal format. **<sw1>** , **<sw2>** Integer type. Information from the (U)SIM about the execution of the actual command. These parameters are delivered to the TE in both cases, on successful or failed execution of the command. **<response>** Response of a successful completion of the command previously issued (hexadecimal character format; refer to **AT+CSCS** ). STATUS and GET RESPONSE return data, which gives information about the current elementary data field. The information includes the type of file and its size (refer to _3GPP TS 51.011_ ). After READ BINARY, READ RECORD or RETRIEVE DATA command, the requested data will be returned. **<response>** is not returned after a successful UPDATE BINARY, UPDATE RECORD or SET DATA command. **<err>** Error codes. For more details, see **_Chapter 12.4_** . 

##### **5.7. AT+QCCID  Show ICCID** 

This command returns the ICCID (Integrated Circuit Card Identifier) number of (U)SIM card. 

|**AT+QCCID  Show ICCID**||
|---|---|
|Test Command|Response|
|**AT+QCCID=?**|**OK**|
|Execution Command|Response|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    81 / 207** 

**LTE Standard Module Series** 



|**AT+QCCID**|**+QCCID: <ICCID>**|
|---|---|
||**OK**|
||Or|
||**ERROR**|
|Maximum Response Time|300 ms|
|Characteristics|/|



###### **Parameter** 

**<ICCID>** String without double quotes. ICCID (Integrated Circuit Card Identifier) number of the <u>(U)SIM card.</u> 

###### **Example** 

**<mark>AT+QCCID</mark>** <mark>//Query ICCID of the (U)SIM card.</mark> **<mark>+QCCID: 89860025128306012474 OK</mark>** 

##### **5.8. AT+QPINC  Display PIN Remainder Counter** 

This command queries the number of attempts left to enter the password of (U)SIM PIN/PUK. 

|**AT+ QPINC  Display PIN** **R**|**emainder Counter**|
|---|---|
|Test Command|Response|
|**AT+QPINC=?**|**+QPINC: (**list of supported**<facility>**s**)**|
||**OK**|
|Read Command|Response|
|**AT+QPINC?**|**+QPINC: "SC",<PIN_counter>,<PUK_counter>**<br>**+QPINC: "P2",<PIN_counter>,<PUK_counter>**|
||**OK**|
|Write Command<br>**AT+QPINC=<facility>**|Response<br>**+QPINC: <facility>,<PIN_counter>,<PUK_counter>**|
||**OK**<br>Or<br>**ERROR **|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    82 / 207** 

**LTE Standard Module Series** 



||If there is any error related to ME functionality:|
|---|---|
||**+CME ERROR: <err>**|
|Maximum Response Time|300 ms|
|Characteristics|/|



###### **Parameter** 

**<facility>** String type. "SC"    (U)SIM PIN "P2"    (U)SIM PIN2 **<PIN_counter>** Integer type. Number of attempts left to enter PIN. Maximum value: 3. **<PUK_counter>** Integer type. Number of attempts left to enter PUK. Maximum value: 10. **<err>** Error codes. For more details, see **_Chapter 12.4_** . 

##### **5.9. AT+QINISTAT  Query Initialization Status of (U)SIM Card** 

This command queries the initialization status of (U)SIM card. 

|**AT+QINISTAT  Query Initiali**|**zation Status of (U)SIM Card**|
|---|---|
|Test Command|Response|
|**AT+QINISTAT=?**|**+QINISTAT: (**list of supported**<status>**s**)**|
||**OK**|
|Execution Command|Response|
|**AT+QINISTAT**|**+QINISTAT: <status>**|
||**OK**|
|Maximum Response Time|300 ms|
|Characteristics|/|



###### **Parameter** 

**<status>** Integer type. Initialization status of (U)SIM card. Actual value is the sum of several of the following four states (e.g. 7 = 1 + 2 + 4 means CPIN READY & SMS DONE & PB DONE). Default: 7. 

- 0    Initial state 

   - 1    CPIN READY. Operation like PIN locking/unlocking is allowed 

   - 2    SMS DONE. SMS initialization completed 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    83 / 207** 

**LTE Standard Module Series** 



4    PB DONE. Phonebook initialization completed 

##### **5.10. AT+QSIMDET  (U)SIM Card Detection** 

This command enables (U)SIM card hot-swap function. (U)SIM card is detected by GPIO interrupt. The level of (U)SIM card detection pin should also be set when (U)SIM card is inserted. 

###### **AT+QSIMDET (U)SIM Card Detection** 

|Test Command|Response|
|---|---|
|**AT+QSIMDET=?**|**+QSIMDET: (**list of supported**<enable>**s**),(**list of supported<br>**<insert_level>**s**)**|
||**OK**|
|Read Command<br>**AT+QSIMDET?**|Response<br>**+QSIMDET: <enable>,<insert_level>**|
||**OK**|
|Write Command|Response|
|**AT+QSIMDET=<enable>,<insert_level**<br>**>**|**OK**<br>Or<br>**ERROR**|
|Maximum Response Time|300 ms|
|Characteristics|The command takes effect after module is rebooted.<br>The configuration is saved automatically.|



###### **Parameter** 

**<enable>** Integer type. Enables or disables (U)SIM card detection. <u>0</u> Disable 1   Enable **<insert_level>** Integer type. The level of (U)SIM detection pin when a (U)SIM card is inserted. <u>0</u> Low level 1   High level 

###### **NOTE** 

1. Hot-swap function is invalid if the configured value of **<insert_level>** is inconsistent with that of in hardware design. 

2. Hot-swap function takes effect after the module is restarted. 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    84 / 207** 

**LTE Standard Module Series** 



###### **Example** 

|**AT+QSIMDET=1,0**|//Set (U)SIM card detection pin level as low when (U)SIM card is inserted.|
|---|---|
|**OK**||
|**<Remove (U)SIM card>**|//Remove (U)SIM card|
|**+CPIN: NOT READY**||
|**<Insert (U)SIM card>**|//Insert (U)SIM card.|
|**+CPIN: READY**|//PIN1 of (U)SIM card is unlocked.|



##### **5.11. AT+QSIMSTAT  (U)SIM Card Insertion Status Report** 

This command queries (U)SIM card insertion status or determines whether to report (U)SIM card insertion status. 

###### **AT+QSIMSTAT (U)SIM Card Insertion Status Report** 

|Test Command|Response|
|---|---|
|**AT+QSIMSTAT=?**|**+QSIMSTAT: (**list of supported**<enable>**s**)**|
||**OK**|
|Read Command<br>**AT+QSIMSTAT?**|Response<br>**+QSIMSTAT: <enable>,<inserted_status>**|
||**OK**|
|Write Command<br>**AT+QSIMSTAT=<enable>**|Response<br>**OK**<br>Or<br>**ERROR**|
|Maximum Response Time|300 ms|
|Characteristics|The command takes effect immediately.<br>The configuration is saved automatically.|



###### **Parameter** 

|**<enable>**|Integer type. Enable or disable (U)SIM card insertion status report. If it is enabled,<br>the URC**+QSIMSTAT: <enable>,<inserted_status>**will be reported, when (U)SIM<br>card is removed or inserted.|
|---|---|
||0Disable<br>1   Enable|
|**<inserted_status>**|Integer type. (U)SIM card is inserted or removed. This argument can not be set.|
||0<br>Removed|
||1<br>Inserted|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    85 / 207** 

**LTE Standard Module Series** 



2 Unknown, before (U)SIM initialization 

###### **Example** 

**<mark>AT+QSIMSTAT?</mark>** <mark>//Query (U)SIM card insertion status.</mark> **<mark>+QSIMSTAT: 0,1 OK AT+QSIMDET=1,0 OK AT+QSIMSTAT=1</mark>** <mark>//Enable (U)SIM card insertion status report.</mark> **<mark>OK AT+QSIMSTAT? +QSIMSTAT: 1,1 OK <Remove (U)SIM card> +QSIMSTAT : 1,0</mark>** <mark>//Report on (U)SIM card insertion status: removed.</mark> **<mark>CPIN: NOT READY AT+QSIMSTAT? +QSIMSTAT: 1,0 OK <Insert (U)SIM card> +QSIMSTAT: 1,1</mark>** <mark>//Report on (U)SIM card insertion status: inserted.</mark> **<mark>+CPIN: READY</mark>** 

##### **5.12. AT+CCHO  Open Logical Channel** 

This command opens a logical channel of (U)SIM card. 

|**AT+CCHO  Open Logical Channel**|
|---|
|Test Command<br>Response|
|**AT+CCHO=?**<br>**OK**|
|Write Command<br>Response|
|**AT+CCHO=<dfname>**<br>**<sessionID>**<br>**OK**<br>Or|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    86 / 207** 

**LTE Standard Module Series** 



||**ERROR**|
|---|---|
|Maximum Response Time|300 ms|
|Characteristics|The command takes effect immediately.<br>The configuration is not saved.|



###### **Parameter** 

**<dfname>** String type. All selectable applications in the UICC are referenced by a DF name coded on 1 to 16 bytes. **<sessionID>** Integer type. A session ID to be used to target a specific application on the smart card using logical channel mechanism. 

###### **NOTE** 

The logical channel number is contained in the CLASS byte of an APDU command, and implicitly contained in all APDU commands sent to a UICC. MT manages the logical channel part of the APDU CLASS byte and to ensures that the chosen logical channel is relevant for **<sessionID>** indicated in the AT command. See _3GPP TS 31.101 [65]_ for further information on logical channels in APDU command protocol. 

###### **Example** 

**<mark>AT+CCHO=?</mark>** <mark>//Test command.</mark> **<mark>OK AT+CCHO="A0000000871002FF86FFFF89FFFFFFFF"</mark>** <mark>//</mark> **<mark><dfname></mark>** <mark>is made up of AID strings.</mark> **<mark>+CCHO: 1</mark>** <mark>//The session ID is 1.</mark> **<mark>OK</mark>** 

##### **5.13. AT+CGLA  UICC Logical Channel Access** 

This command accesses a UICC logical channel. 

|**AT+CGLA  UICC Logical Chan**|**nel Access**|
|---|---|
|Test Command|Response|
|**AT+CGLA=?**|**OK**|
|Write Command|Response|
|**AT+CGLA=<sessionID>,<length>,**<br>**<command>**|**+CGLA: <length>,<response>**|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    87 / 207** 

**LTE Standard Module Series** 



**OK** Or **ERROR** Maximum Response Time 300 ms The command takes effect immediately. Characteristics The configuration is not saved. 

###### **Parameter** 

**<sessionID>** Integer type. Identifier of the session for sending the APDU commands to the UICC. The commands to the UICC when targeting applications on the smart card using a logical channel other than the default channel (channel "0"). **<length>** Integer type. Length of the characters that are sent to TE in **<command>** or **<response>** (Twice the actual length of the command or response). **<command>** Command passed on by the MT to the UICC in the format described in 3GPP TS 31.101 [65] (hexadecimal character format; refer to **AT+CSCS** ) **<response>** Response to the command passed on by the UICC to the MT in the format as described in 3GPP TS 31.101 [65] (hexadecimal character format; refer to **AT+CSCS** <u>).</u> 

###### **Example** 

**<mark>AT+CGLA=?</mark>** <mark>//Test command.</mark> **<mark>OK AT+CGLA=1,14,"00A40804022F00"</mark>** <mark>//The command is 00A40804022F00.</mark> **<mark>+CGLA: 4,"6121"</mark>** <mark>//The length is 4, the response is 6121.</mark> **<mark>OK</mark>** 

##### **5.14. AT+CCHC  Close Logical Channel** 

This command closes a logical channel of (U)SIM card with the given **<sessionID>** . 

|**AT+CCHC  Close Logical Channel**|
|---|
|Test Command<br>Response|
|**AT+CCHC=?**<br>**OK**|
|Write Command<br>Response|
|**AT+CCHC=<sessionID>**<br>**OK**<br>Or<br>**ERROR**|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    88 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

|Maximum Response Time|300 ms|
|---|---|
|Characteristics|The command takes effect immediately.<br>The configuration is not saved.|



###### **Parameter** 

**<sessionID>** Inter type. A session ID to be used to target a specific application on the smart card using logical channels mechanism. 

###### **Example** 

|**AT+CCHC=?**|//Test command.|
|---|---|
|**OK**||
|**AT+CCHC=1**|//Close logical channel: 1.|
|**OK**||



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    89 / 207** 

**LTE Standard Module Series** 



# **6 Network Service Commands** 

##### **6.1. AT+COPS  Operator Selection** 

This command returns the current operators and their status, and allows setting automatic or manual network selection. 

The Test Command returns a set of five parameters, each representing an operator present in the network. Any of the formats may be unavailable and should be an empty field. The list of operators will be ordered as follows: home network, networks referenced in (U)SIM and other networks. 

The Read Command returns the current mode and the currently selected operator. If no operator is selected, **<format>** , **<oper>** and **<Act>** are omitted. 

This Write Command forces an attempt to select and register the GSM/UMTS network operator. If the selected operator is not available, no other operator will be selected (except **<mode>** =4). The format of selected operator name will apply to other Read Commands ( **AT+COPS?** ). 

|**AT+COPS  Operator Selection**||
|---|---|
|Test Command|Response|
|**AT+COPS=?**|**+COPS: [**list of supported**(<stat>,**long alphanumeric**<**<br>**oper>,**short alphanumeric**<oper>,**numeric**<oper>     [,<**<br>**Act>]**)s**][,,(**list of supported**<mode>**s**),(**list of supported**<f**<br>**ormat>**s**)]**|
||**OK**|
||If there is any error related to ME functionality:<br>**+CME ERROR: <err>**|
|Read Command<br>**AT+COPS?**|Response<br>**+COPS: <mode>[,<format>[,<oper>][,<Act>]]** <br>**OK**|
||If there is any error related to ME functionality:<br>**+CME ERROR: <err>**|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    90 / 207** 

**LTE Standard Module Series** 



|Write Command|Response|
|---|---|
|**AT+COPS=<mode>[,<format>[,<oper**|**OK**|
|**>[,<Act>]]]**|If there is any error related to ME functionality:<br>**+CME ERROR: <err>**|
|Maximum Response Time|180 s, determined by network.|
|Characteristics|The command takes effect immediately.|
|Reference||
|3GPP TS 27.007||



###### **Parameter** 

|**<stat>**|Intege|r type.|
|---|---|---|
||0|Unknown|
||1|Operator available|
||2|Current operator|
||3|Operator forbidden|
|**<oper>**|Opera|tor in format as per**<format>**.|
|**<mode>**|Intege|r type. Determines whether**<oper>**is present or not.|
||0|Automatic mode.**<oper>**field is omitted|
||1|Manual operator selection.**<oper>**field is present and <**Act**> optionally|
||2|Manually deregister from network|
||3|Set only**<format>**(for**AT+COPS?**Read Command), and do not attempt|
|||registration/deregistration (**<oper>**and**<Act>**fields are ignored). This value is<br>invalid in Read Command response.|
||4|Manual/automatic selection.**<oper>**field is present. If manual selection fails,|
|||automatic mode (**<mode>**=0) is entered|
|**<format>**|Intege|r type. Indicates the format of**<oper>**.|
||0|Long format alphanumeric**<oper>**up to 16 characters|
||1|Short format alphanumeric**<oper>**|
||2|Numeric**<oper>**. GSM location area identification number|
|**<Act>**|Intege<br>Comm<br>Write|r type. Access technology selected. Values 3, 4, 5 and 6 occur only in the Read<br>and response while MS is in data service state and is not intended for**AT+COPS**<br>Command.|
||0|GSM|
||2|UTRAN|
||3|GSM W/EGPRS|
||4|UTRAN W/HSDPA|
||5|UTRAN W/HSUPA|
||6|UTRAN W/HSDPA and HSUPA|
||7|E-UTRAN|
||100|CDMA|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    91 / 207** 

**LTE Standard Module Series** 



**<err>** Error codes. For more details, see **_Chapter 12.4_** . 

###### **Example** 

**<mark>AT+COPS=?</mark>** <mark>//List all current network operators.</mark> **<mark>+COPS: (1,"CHN-UNICOM","UNICOM","46001",2),(1,"CHN-UNICOM","UNICOM","46001",0),(2,"CH N-UNICOM","UNICOM","46001",7),(1,"46011","46011","46011",7),(3,"CHINA MOBILE","CMCC","46 000",0),,(0,1,2,3,4),(0,1,2)</mark>** 

**<mark>OK AT+COPS?</mark>** <mark>//Query the currently selected network operator.</mark> **<mark>+COPS: 0,0,"CHN-UNICOM",7 OK</mark>** 

##### **6.2. AT+CREG  Network Registration Status** 

This Read Command returns the status of result code presentation and an integer **<stat>** that shows whether the network has currently indicated the registration of the ME. Location information elements **<LAC>** and **<ci>** are returned only when **<n>** =2 and ME is registered on the network. 

This Write Command controls the presentation of an unsolicited result code **+CREG: <stat>** when **<n>** =1 and there is a change in the ME network registration status. 

|**AT+CREG  Network Registr**|**ation Status**|
|---|---|
|Test Command|Response|
|**AT+CREG=?**|**+CREG: (**list of supported**<n>**s**)**|
||**OK**|
|Read Command<br>**AT+CREG?**|Response<br>**+CREG: <n>,<stat>[,<LAC>,<ci>[,<Act>]]**|
||**OK**|
||If there is any error related to ME functionality:<br>**+CME ERROR: <err>**|
|Write Command<br>**AT+CREG[=<n>]**|Response<br>**OK**|
|Maximum Response Time|300 ms|
|Characteristics|The command takes effect immediately.<br>The configuration is not saved.|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    92 / 207** 

**LTE Standard Module Series** 



Reference 3GPP TS 27.007 

###### **Parameter** 

|**<n>**|Integ|er|type. Whether to enable related registration network.|
|---|---|---|---|
||0||Disable network registration URC|
||1||Enable network registration URC**+CREG: <stat>**|
||2||Enable network registration URC with location information:<br> **+CREG: <stat>[,<LAC>,<ci>[,<Act>]]**|
|**<stat>**|Integ|er|type. Registration network status.|
||0||Not registered. ME is not currently searching a new operator to register to|
||1||Registered, home network|
||2||Not registered, but ME is currently searching a new operator to register to|
||3||Registration denied|
||4||Unknown|
||5||Registered, roaming|
|**<LAC>**|String|t|ype. Two-byte location area code in hexadecimal format.|
|**<ci>**|String|t|ype. 16-bit (GSM) or 28-bit (UMTS/LTE) cell ID in hexadecimal format.|
|**<Act>**|<br>Integ|er|type. Access technology selected.|
||0||GSM|
||2||UTRAN|
||3||GSM W/EGPRS|
||4||UTRAN W/HSDPA|
||5||UTRAN W/HSUPA|
||6||UTRAN W/HSDPA and HSUPA|
||7||E-UTRAN|
|**<err>**|Error|co|des. For more details, see**_Chapter 12.4_**.|



###### **Example** 

**<mark>AT+CREG=1 OK +CREG: 1</mark>** <mark>//URC reports that ME has registered on network.</mark> **<mark>AT+CREG=2</mark>** <mark>//Activate extended URC mode.</mark> **<mark>OK +CREG: 1,"D509","80D413D",7</mark>** <mark>//URC reports that operator has found location area code and cell ID.</mark> 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    93 / 207** 

**LTE Standard Module Series** 



##### **6.3. AT+CSQ  Signal Quality Report** 

The command returns the received signal strength **<rssi>** and the channel bit error rate **<ber>** . 

The Test Command returns values supported by MT. 

The Execution Command returns received signal strength indication **<rssi>** and channel bit error rate **<ber>** from MT. 

###### **AT+CSQ  Signal Quality Report** 

|Test Command|Response|
|---|---|
|**AT+CSQ=?**|**+CSQ: (**list of supported**<rssi>**s**),(**list of supported**<ber>**s**)**|
||**OK**|
|Execution Command|Response|
|**AT+CSQ**|**+CSQ: <rssi>,<ber>**|
||**OK**|
||If there is error related to MT functionality:<br>**+CME ERROR: <err****_>_**|
|Maximum Response Time|300 ms|
|Characteristics|/|
|Reference||
|3GPP TS 27.007||



###### **Parameter** 

|**<rssi>**|Integer type.|Received signal strength indication.|
|---|---|---|
||0|-113 dBm or less|
||1|-111 dBm|
||2–30|-109 dBm to -53 dBm|
||31|-51 dBm or greater|
||99|Not known or not detectable|
||100|-116 dBm or less|
||101|-115 dBm|
||102...190|-114 dBm to -26 dBm|
||191|-25 dBm or greater|
||199|Not known or not detectable|
||100–199|Extended to be used in TD-SCDMA indicating received signal code|
|||power(RSCP)|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    94 / 207** 

**LTE Standard Module Series** 



**<ber>** Integer type. Channel bit error rate (in percent). 0–7 As RxQual values in the table in _3GPP TS 45.008 subclause 8.2.4_ 99 Not known or not detectable 

###### **Example** 

**<mark>AT+CSQ=? +CSQ: (0-31,99),(0-7,99) OK AT+CSQ +CSQ: 28,99</mark>** <mark>//Received signal strength indication is 28 dBm and channel bit error rate is 99 dBm.</mark> **<mark>OK</mark>** 

##### **6.4. AT+CPOL  Preferred Operator List** 

This command edits and queries the list of preferred operators. 

|**AT+CPOL  Preferred Operator Lis**|**t**|
|---|---|
|Test Command|Response|
|**AT+CPOL=?**|**+CPOL: (**list of supported**<index>**s**),(**list of supported<br>**<format>**s**)**<br>**OK**|
|Read Command<br>**AT+CPOL?**|Response<br>Query the list of preferred operators:<br>**+CPOL: <index>,<format>,<oper>[,<GSM>,<GSM_comp**<br>**act>,<UTRAN>,<E-UTRAN>]**<br>**[+CPOL: <index>,<format>,<oper>[,<GSM>,<GSM_comp**<br>**act>,<UTRAN>,<E-UTRAN>**<br>**…]**<br>**OK**|
|Write Command<br>**AT+CPOL=<index>[,<format>[,<oper**<br>**>[<GSM>,<GSM_compact>,<UTRAN>**<br>**,<E-UTRAN>]]]**|Response<br>Edit the list of preferred operators:<br>**OK**<br>Or<br>**ERROR**|
||If**<index>**is given but**<oper>**is omitted, the entry is<br>deleted.|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    95 / 207** 

**LTE Standard Module Series** 



|Maximum Response Time|300 ms|
|---|---|
|Characteristics|The command takes effect immediately.|
|Reference||
|3GPP TS 27.007||



###### **Parameter** 

|**<index>**|Integer type. The  operator order number on the (U)SIM preferred operator list.|
|---|---|
|**<format>**<br> <br> <br>|Integer type. Format of**<oper>**.<br>0<br>Long format alphanumeric**<oper>**<br>1<br>Short format alphanumeric**<oper>**<br> 2<br>Numeric**<oper>**|
|**<oper>**|String type. Operation Name.**<format>**indicates if the format is alphanumeric or<br>numeric (see**AT+COPS**).|
|**<GSM>**|Integer type. GSM access technology.<br>0<br>Access technology is not selected<br>1<br>Access technology is selected|
|**<GSM_compact>**|Integer type. GSM compact access technology.<br>0<br>Access technology is not selected<br>1<br>Access technology is selected|
|**<UTRAN>**|Integer type. UTRAN access technology.<br>0<br>Access technology is not selected<br>1<br>Access technology is selected|
|**<E-UTRAN>**|Integer type. E-UTRAN access technology.<br>0<br>Access technology is not selected<br>1<br>Access technologyis selected|



###### **NOTE** 

The access technology selection parameters **<GSM>** , **<GSM_compact>** , **<UTRAN>** and **<E-UTRAN>** are required for (U)SIM cards or UICC’s containing PLMN selector with access technology. 

##### **6.5. AT+COPN  Read Operator Names** 

This command returns the list of supported operators from MT. Each operator code **<numericn>** that has an alphanumeric equivalent **<alphan>** in the MT memory is returned. 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    96 / 207** 

**LTE Standard Module Series** 



###### **AT+COPN  Read Operator Names** 

|Test Command|Response|
|---|---|
|**AT+COPN=?**|**OK**|
|Execution Command|Response|
|**AT+COPN**|**+COPN: <numeric1>,<alpha1>**<br>**[+COPN: <numeric2>,<alpha2>**<br>**…]**|
||**OK**|
||If there is any error related to ME functionality:<br>**+CME ERROR: <err>**|
|Maximum Response Time|Depends on the number of operator names.|
|Characteristics|/|
|Reference||
|3GPP TS 27.007||



###### **Parameter** 

|**<numericn>**|String type. Operator in numeric format (see**AT+COPS**).|
|---|---|
|**<alphan>**|String type. Operator in long alphanumeric format (see**AT+COPS**).|
|**<err>**|Error codes. For more details, see**_Chapter 12.4_**.|



##### **6.6. AT+CTZU  Automatic Time Zone Update** 

This command enables/disables automatic time zone update via NITZ. 

###### **AT+CTZU  Automatic Time Zone Update** 

|Test Command|Response|
|---|---|
|**AT+CTZU=?**|**+CTZU: (**list of supported**<enable>**s**)**|
||**OK**|
|Write Command|Response|
|**AT+CTZU=<enable>**|**OK**|
||Or<br>**ERROR**|
|Read Command|Response|
|**AT+CTZU?**|**+CTZU: <enable>**|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    97 / 207** 

**LTE Standard Module Series** 



||**OK**|
|---|---|
|Maximum Response Time|300 ms|
|Characteristics|The command takes effect immediately.<br>The configuration is saved automatically.|
|Reference||
|3GPP TS 27.007||



###### **Parameter** 

- **<enable>** Integer type. Mode of automatic time zone update. 

- <u>0</u> Disable automatic time zone update via NITZ. 

   - 1 Enable automatic time zone update via NITZ 

   - 3 Enable automatic time zone update via NITZ and update LOCAL time to RTC 

###### **Example** 

**<mark>AT+CTZU?</mark>** <mark>//Read command.</mark> **<mark>+CTZU: 0 OK AT+CTZU=?</mark>** <mark>//Test command.</mark> **<mark>+CTZU: (0,1,3) OK AT+CTZU=1</mark>** <mark>//Enable automatic time zone update.</mark> **<mark>OK AT+CTZU? +CTZU: 1 OK</mark>** 

##### **6.7. AT+CTZR  Time Zone Reporting** 

This command controls the time zone chang event reporting. If reporting is enabled, the MT returns the unsolicited result code **+CTZV: <tz>** or **+CTZE: <tz>,<dst>,<time>** whenever the time zone is changed. 

|**AT+CTZR  Time Zone Reporting**||
|---|---|
|Test Command|Response|
|**AT+CTZR=?**|**+CTZR: (**list of supported**<reporting>**s**)**|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    98 / 207** 

**LTE Standard Module Series** 



||**OK**|
|---|---|
|Write Command|Response|
|**AT+CTZR=<reporting>**|**OK**<br>Or<br>**ERROR**|
|Read Command|Response|
|**AT+CTZR?**|**+CTZR: <reporting>**|
||**OK**|
|Maximum Response Time|300 ms|
|Characteristics|The command takes effect after module is rebooted.<br>The configuration is saved automatically.|
|Reference||
|3GPP TS 27.007||



###### **Parameter** 

- **<reporting>** Integer type. Mode of time zone reporting. <u>0</u> Disable time zone change event reporting 1 Enable time zone change event reporting by URC **+CTZV: <tz>** 2 Enable extended time zone reporting by URC **+CTZE: <tz>,<dst>,<time>** 

- **<tz>** String type. Sum of the local time zone (difference between the local time and GMT is expressed in quarters of an hour) plus daylight saving time. The format is "±zz", expressed as a fixed width, two-digit integer with the range -48 to +56. To maintain a fixed width, numbers in the range -9 to +9 are expressed with a leading zero, e.g. "-09", "+00" and "+09". 

- **<dst>** Integer type. Indicates whether **<tz>** includes daylight savings adjustment. 0 **<tz>** includes no adjustment for daylight saving time 1 **<tz>** includes +1 hour (equals 4 quarters in **<tz>** ) adjustment for daylight saving time 

- 2 **<tz>** includes +2 hours (equals 8 quarters in **<tz>** ) adjustment for daylight saving time 

- **<time>** String type. Local time. The format is "YYYY/MM/DD,hh:mm:ss", expressed as integers representing year (YYYY), month (MM), date (DD), hour (hh), minute (mm) and second (ss). This parameter can be provided by the network when delivering time zone information and will be present in the unsolicited result code of extended time zone reporting if provided by the network. 

###### **Example** 

**<mark>AT+CTZR=2 OK</mark>** 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    99 / 207** 

**LTE Standard Module Series** 



**<mark>AT+CTZR? +CTZR: 2</mark>** 

**<mark>OK</mark>** 

**<mark>+CTZE: "+32",0,"2017/11/04,06:51:13"</mark>** <mark>//Extended time zone and local time reporting by URC.</mark> 

##### **6.8. AT+QLTS  Obtain the Latest Time Synchronized Through Network** 

This command obtains the latest time synchronized through network. 

The Execution Command returns the latest time synchronized through the network. 

|**AT+QLTS  Obtain the Lates**|**t Time Synchronized Through Network**|
|---|---|
|Test Command|Response|
|**AT+QLTS=?**|**+QLTS:** **(**list of supported**<mode>**s**)**|
||**OK**|
|Execution Command|Response|
|**AT+QLTS**|**+QLTS: <time>,<dst>**|
||**OK**|
|Write Command|Response|
|**AT+QLTS=<mode>**|**+QLTS: <time>,<dst>**|
||**OK**<br>Or<br>**ERROR**|
||If there is error related to ME functionality:<br>**+CME ERROR: <err>**|
|Maximum Response Time|300 ms|
|Characteristic|The command takes effect immediately.|



###### **Parameter** 

- **<mode>** Integer type. Network time obtaining mode. 

   - 0 Query the latest time that has been synchronized through network 

   - 1 Query the current GMT time calculated from the latest time that has been 

      - synchronized through network 

   - 2 Query the current LOCAL time calculated from the latest time that has been 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    100 / 207** 

**LTE Standard Module Series** 



synchronized through network 

**<time>** String type. Format is "YYYY/MM/dd,hh:mm:ss±zz", where characters indicate year (two last digits), month, day, hour, minutes, seconds and time zone (indicates the difference, expressed in quarters of an hour, between the local time and GMT; Range: -48 to +48). E.g. 6th of May 2004, 22:10:00 GMT+2 hours equals to "04/05/06,22:10:00+08" 

**<dst>** Integer type. Daylight saving time. Range: 0–2. Default: 0. 

**<err>** Error codes. For more details, see **_Chapter 12.4_** . 

###### **NOTE** 

If the time has not been synchronized through the network, the command will return a null time string as **+QLTS: ""** . 

###### **Example** 

**<mark>AT+QLTS=?</mark>** <mark>//Query the supported network time mode.</mark> **<mark>+QLTS: (0-2)</mark>** 

**<mark>OK AT+QLTS</mark>** <mark>//Query the latest time synchronized through network.</mark> **<mark>+QLTS: "2017/10/13,03:40:48+32,0"</mark>** 

**<mark>OK</mark>** 

**<mark>AT+QLTS=0</mark>** <mark>//Query the latest time synchronized through network. It performs the same function as Execution Command</mark> **<mark>AT+QLTS</mark>** <mark>.</mark> **<mark>+QLTS: "2017/10/13,03:40:48+32,0"</mark>** 

**<mark>OK AT+QLTS=1</mark>** <mark>//Query the current GMT time calculated from the latest time that has been synchronized through network.</mark> **<mark>+QLTS: "2017/10/13,03:41:22+32,0" OK AT+QLTS=2</mark>** <mark>//Query the current LOCAL time calculated from the latest time that has been synchronized through network</mark> **<mark>+QLTS: "2017/01/13,11:41:23+32,0"</mark>** 

**<mark>OK</mark>** 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    101 / 207** 

**LTE Standard Module Series** 



##### **6.9. AT+QNWINFO  Query Network Information** 

This command queries network information such as the selected access technology, operator and band. 

|**AT+QNWINFO  Query Network I**|**nformation**|
|---|---|
|Test Command|Response|
|**AT+QNWINFO=?**|**OK**|
|Execution Command|Response|
|**AT+QNWINFO**|**+QNWINFO: <Act>,<oper>,<band>,<channel>**|
||**OK**|
|Maximum Response Time|300 ms|
|Characteristics|/|



###### **Parameter** 

**<Act>** String type. Selected access technology. "NONE" "TDD LTE" "FDD LTE" **<oper>** String type. Operator in numeric format. **<band>** String type. Selected band. "LTE BAND 1" "LTE BAND 2" "LTE BAND 3" "LTE BAND 4" "LTE BAND 5" "LTE BAND 7" "LTE BAND 8" "LTE BAND 12" "LTE BAND 13" "LTE BAND 14" "LTE BAND 18" "LTE BAND 19" "LTE BAND 20" "LTE BAND 25" "LTE BAND 26" "LTE BAND 28" "LTE BAND 34" "LTE BAND 38" "LTE BAND 39" "LTE BAND 40" 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    102 / 207** 

**LTE Standard Module Series** 



"LTE BAND 41" 

"LTE BAND 66" "LTE BAND 71" 

**<channel>** Integer type. Channel ID. 

###### **NOTE** 

See the product specification of all applicable modules for specific frequency bands supported by each module. 

###### **Example** 

**<mark>AT+QNWINFO=? OK AT+QNWINFO +QNWINFO: "FDD LTE",46011,"LTE BAND 3",1650 OK</mark>** 

##### **6.10. AT+QSPN  Display Name of Registered Network** 

|**AT+QSPN  Display the Nam**|**e of Registered Network**|
|---|---|
|Test Command|Response|
|**AT+QSPN=?**|**OK**|
|Execution Command|Response|
|**AT+QSPN**|**+QSPN: <FNN>,<SNN>,<SPN>,<alphabet>,<RPLMN>**|
||**OK**|
|Maximum Response Time|300 ms|
|Characteristics|/|



###### **Parameter** 

|**<FNN>**|String type. Full network name.|
|---|---|
|**<SNN>**|String type. Abbreviated network name.|
|**<SPN>**|String type. Service provider name.|
|**<alphabet>**|Integer type. Alphabet of full network names and abbreviated network name.|
||0<br>GSM 7-bit default alphabet|
||1<br>UCS2|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    103 / 207** 

**LTE Standard Module Series** 



**<RPLMN>** String type. Registered PLMN. 

###### **NOTE** 

1. If **<alphabet>** is 0, **<FNN>** and **<SNN>** will be shown in GSM 7-bit default alphabet string. 

2. If **<alphabet>** is 1, **<FNN>** and **<SNN>** will be shown in UCS2 hexadecimal string. 

###### **Example** 

**<mark>AT+QSPN</mark>** <mark>//Query the EONS information of RPLMN.</mark> **<mark>+QSPN: "CHN-UNICOM","UNICOM","",0,"46001"</mark>** 

**<mark>OK</mark>** 

##### **6.11. AT+QENG  Switch on/off Engineering Mode** 

Engineering mode is designed to report the information of serving cells, neighbour cells and packet switch parameters. The command switches on/off the mode. 

|**AT+QENG  Switching on/off Engi**|**neering Mode**|
|---|---|
|Test Command<br>**AT+QENG=?**|Response<br>**+QENG:**(list of supported**<cell_type>**s)|
||**OK**|
|**AT+QENG="servingcell"**|Response|
|Query the information of serving cells|**+QENG: "servingcell",<state>,"LTE",<is_tdd>,<mcc>,**<br>**<mnc>,<cellID>,<earfcn>,<pcid>,<freq_band_ind>,<ul_b**<br>**andwidth>,<dl_bandwidth>,<tac>,<rsrp>,<rsrq>,<rssi>,**<br>**<sinr>,<s_rxlev>**|
||**OK**|
|**AT+QENG="neighbourcell"**|Response|
|Query the information of neighbour cells|**[+QENG: "neighbourcell intra","LTE",<earfcn>,<pcid>,**<br>**<rsrq>,<rsrp>,-,-,-,-,-,-**<br>**[…]]**|
||Or|
||**[+QENG: "neighbourcell inter","LTE",<earfcn>,<pcid>,**<br>**<rsrq>,<rsrp>,-,-,-,-,-,-**|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    104 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

||**[…]]**|
|---|---|
||**OK**|
|Maximum Response Time|300 ms|
|Characteristics|/|



###### **Parameter** 

**<cell_type>** String format. The information of different cells. "servingcell"      Information of 4G serving cells "neighbourcell"    Information of 4G neighbour cells **<state>** String format. UE state. "SEARCH"       UE is searching but could not (yet) find a suitable 4G cell. "LIMSRV"       UE is camping on a cell but has not registered on the network. "NOCONN"       UE is camping on a cell and has registered on the network, and it is in idle mode. "CONNECT"     UE is camping on a cell and has registered on the network, and a call is in progress. **<is_tdd>** String format. Communication mode . "TDD"           Time division duplex mode "FDD"           Frequency division duplex mode **<mcc>** Integer type. Mobile country code (first part of PLMN code). "-" Invalid **<mnc>** Integer type. Mobile network code (second part of PLMN code). "-" Invalid **<cellID>** Hexadecimal format. Cell ID. Range: 0-0xFFFFFFF. "-" Invalid **<earfcn>** Integer type. E-UTRA-ARFCN of the scanned cell. **<pcid>** Physical cell ID. **<freq_band_ind>** Integer type. E-UTRA band (see _3GPP 36.101_ ). **<ul_bandwidth>** Integer type. UL bandwidth. 0              1.4 MHz 1              3 MHz 2              5 MHz 3              10 MHz 4              15 MHz 5              20 MHz **<dl_bandwidth>** Integer type. DL bandwidth. 0              1.4 MHz 1              3 MHz 2              5 MHz 3              10 MHz 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    105 / 207** 

**LTE Standard Module Series** 



4              15 MHz 5              20 MHz 

**<tac>** Tracking area code (see _3GPP 23.003_ **_Chapter 19.4.2.3_** ). 

**<rsrp>** Reference signal received power (see _3GPP 36.214_ **_Chapter 5.1.1_** ). 

**<rsrq>** Reference signal received quality (see _3GPP 36.214_ **_Chapter 5.1.2_** ). **<rssi>** Integer type. Received signal strength indication. **<sinr>** Integer type. Logarithmic value of SINR. Range: -20 to +30. Unit: dB. **<s_rxlev>** Suitable receive level of Inter-frequency cell. 

**<arfcn>** Integer type. Determines the ARFCN of the scanned cell. Range: 0-1023. 

**NOTE** 

If “-” or - is returned, it indicates the parameter is invalid under current condition. 

###### **Example** 

**<mark>AT+QENG="servingcell" +QENG: "servingcell","SEARCH" OK AT+QENG="servingcell" +QENG: "servingcell","NOCONN","LTE","FDD",460,01,B57DE33,63,1850,3,5,5,B504,-85,-10,-54,17,38 OK</mark>** 

**<mark>AT +QENG="neighbourcell"</mark>** <mark>//Get neighbour cell information in LTE mode.</mark> **<mark>+QENG: "neighbourcell inter","LTE",575,-,-,-,-,-,-,-,-,+QENG: "neighbourcell inter","LTE",300,-,-,-,-,-,-,-,-,+QENG: "neighbourcell inter","LTE",500,-,-,-,-,-,-,-,-,+QENG: "neighbourcell inter","LTE",525,-,-,-,-,-,-,-,-,+QENG: "neighbourcell inter","LTE",1650,-,-,-,-,-,-,-,-,+QENG: "neighbourcell inter","LTE",1850,-,-,-,-,-,-,-,-,+QENG: "neighbourcell inter","LTE",2452,-,-,-,-,-,-,-,-,+QENG: "neighbourcell inter","LTE",3745,-,-,-,-,-,-,-,-,-</mark>** 

**<mark>OK</mark>** 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    106 / 207** 

**LTE Standard Module Series** 



##### **6.12. AT+CIND  Command of Control Indicator** 

|**AT+CIND  Command of Co**|**ntrol Indicator**|
|---|---|
|Test Command|Response|
|**AT+CIND=?**|**+CIND:(<descr>,(**list of supported**<ind>**s**))[,(<descr>,(**list of<br>supported**<ind>**s**))[,...]]**<br>**OK**|
|Read Command<br>**AT+CIND?**|Response<br>**+CIND: <ind>[,<ind>[,...]]**|
||**OK**<br>If error is related to ME functionality:<br>**+CME ERROR: <err>**|
|Maximum Response Time|300 ms|
|Characteristics|/|



###### **Parameter** 

**<descr>** String type. Indicator description. See the following notes for details. **<ind>** Integer types. Indicator. Related to the value of **<descr>** . see the following notes for details. **<err>** Error codes. For more details, see **_Chapter 12.4_** . 

|**NOTE**|
|---|



<mark>The values of</mark> **<mark><descr></mark>** <mark>and</mark> **<mark><ind></mark>** <mark>are described as follows:</mark> 

|**<descr>**|**<ind>**|
|---|---|
|**"call"**|Call in progress indication.<br>0<br>No call in progress<br>1<br>Call inprogress|
||Roaming indicator.|
|**"roam"**|0<br>Registered to home network or not registered<br>1<br>Registered to other network|
||Signal quality indication.|
|**"signal"**|0–5: Signal is divided into five levels. The larger the<br>value, the better the signal.|
||Service availability indicator.|
|**"service"**|0<br>Not registered on the network<br>1<br>Registered to home network|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    107 / 207** 

**LTE Standard Module Series** 



PS domain registration indicator. 

- **"GPRS coverage"** 0 Not registered on PS domain 

   - 1 Registered on PS domain 

   - Message received indicator. 

- **"message"** 0 Turn off 

   - 1 Turn on 

###### **Example** 

**<mark>AT+CIND=? +CIND: ("call",(0,1)),("roam",(0,1)),("signal",(0-5)),("service",(0,1)),("GPRS coverage",(0,1)), ("message",(0,1))</mark>** 

**<mark>OK AT+CIND? +CIND: 0,0,3,1,1,0</mark>** 

**<mark>OK</mark>** 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    108 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
RUECCTEL<br><!-- End of picture text -->

# **7 Dial Related Commands** 

##### **7.1. ATD  PPP Dial** 

This command sets up a PPP dial. Supplementary services can also be controlled with this command. 

|**ATD  PPP Dial**||
|---|---|
|Execution Command|Response|
|**ATD<n>**|If a connection cannot be established:<br>**NO CARRIER**|
||If connection is successful:<br>**CONNECT**|
|Maximum Response Time|5 s, determined by network (**AT+COLP=0**).|
|Characteristics|/|
|Reference||
|V.25ter||



###### **Parameter** 

**<n>** String of dialing digits and optionally V.25ter modifiers. Dialing digits: 0–9, * , #, +, A, B, C Following V.25ter modifiers are ignored: ,(comma), T, P, !, W, @ 

###### **NOTE** 

This command can only be used in PPP. 

###### **Example** 

**<mark>ATD*99#</mark>** <mark>//Start PPP dialing.</mark> **<mark>OK</mark>** 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    109 / 207** 

**LTE Standard Module Series** 



##### **7.2. +++ Switch from Data Mode to Command Modeg** 

This command is only available when TA is in data mode. The " **+++** " character sequence causes the TA to cancel the data flow over the AT interface and switch to command mode. This allows inputting AT commands while maintaining the data connection with the remote server. 

|**+++  Switch from Data Mode to C**|**ommand Mode**|
|---|---|
|Execution Command|Response|
|**+++**|**OK**|
|Maximum Response Time|300 ms|
|Characteristics|/|
|Reference||
|V.25ter||



###### **NOTE** 

1. To prevent **+++** escape sequence from being misinterpreted as data, the following sequence should be followed: 

   - ⚫ Do not input any character for at least 1s before and after inputting **+++** . 

   - ⚫ Input **+++** within 1s, and wait until OK is returned. 

   - ⚫ Switch to command mode successfully; otherwise return to Step 1. 

2. To return back to data mode from AT command mode, enter **ATO** . 

3. Another way to change to command mode is through DTR level change, and please refer to **AT&D** command for details. 

##### **7.3. ATO  Switch from Command Mode to Data Mode** 

This command resumes the connection and switches back to data mode from command mode. 

|**ATO  Switch from Comman**|**d Mode to Data Mode**|
|---|---|
|Execution Command|Response|
|**ATO[n]**|If connection is not successfully resumed:<br>**NO CARRIER**|
||If connection is successfully resumed, TA returns to data<br>mode from command mode:<br>**CONNECT <text>**|
|Maximum Response Time|300 ms|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    110 / 207** 

**LTE Standard Module Series** 



|Characteristics<br>/|
|---|
|Reference|
|V.25ter|



###### **Parameter** 

**<n>** Integer type. 0 Switch from command mode to data mode 

###### **NOTE** 

When TA returns to data mode from command mode successfully, **CONNECT <text>** is returned. Note that **<text>** outputs only when **<value>** is greater than 0 in **ATX<value>** parameter setting. 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    111 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

# **8 Short Message Service Commands** 

##### **8.1. AT+CSMS  Select Message Service** 

This command selects messaging service **<service>** and returns the types of messages supported by the ME. 

|**AT+CSMS  Select Message**|**Service**|
|---|---|
|Test Command|Response|
|**AT+CSMS=?**|**+CSMS:** **(**list of supported**<service>**s**)**|
||**OK**|
|Read Command<br>**AT+CSMS?**|Response<br>**+CSMS:** **<service>,<mt>,<mo>,<bm>**|
||**OK**|
|Write Command|Response|
|**AT+CSMS=<service>**|**+CSMS:** **<mt>,<mo>,<bm>**|
||**OK**|
||If there is any error related to ME functionality:<br>**+CMS ERROR: <err>**|
|Maximum Response Time|300 ms|
|Characteristics|The command takes effect immediately.<br>The configurations is saved automatically.|
|Reference||
|3GPP TS 27.005||



###### **Parameter** 

**<service>** Integer type. Message service type. <u>0</u> _3GPP TS 23.040_ and _3GPP TS 23.041_ (the syntax of SMS AT commands is compatible with _3GPP TS 27.005 Phase 2 version 4.7.0_ ; Phase 2+ features that do not require new command syntax may be supported, e.g. correct routing of messages with new Phase 2+ data coding schemes). 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    112 / 207** 

**LTE Standard Module Series** 



||1|_3GPP TS 23.040_and_3GPP TS 23.04_1 (the syntax of SMS AT commands is<br>compatible with_3GPP TS 27.005 Phase 2+ version_; the requirement of<br>**<service>**setting 1 is mentioned under corresponding command<br>descriptions).|
|---|---|---|
|**<mt>**|Integer type.|Mobile terminated messages.|
||0|Type not supported|
||1|Type supported|
|**<mo>**|Integer type.|Mobile originated messages.|
||0|Type not supported|
||1|Type supported|
|**<bm>**|Integer type.|Broadcast type messages.|
||0|Type not supported|
||1|Type supported|
|**<err>**|Error codes.|For more details, see**_Chapter 12.5_**.|



###### **Example** 

|**AT+CSMS=?**<br>|//Test command.|
|---|---|
|**+CSMS: (0,1)**||
|**OK**||
|**AT+CSMS=1**|//Set message service type to 1.|
|**+CSMS: 1,1,1**||
|**OK**||
|**AT+CSMS?**|//Read command.|
|**+CSMS: 1,1,1,1**||
|**OK**||



##### **8.2. AT+CMGF  Message Format** 

This command specifies the input and output format of the short messages. **<mode>** indicates the format of messages used with Test, Read, Write and Execution Commands and unsolicited result codes resulting from received messages. 

The format of messages can be either PDU mode (entire TP data units used) or text mode (headers and body of the messages given as separate parameters). Text mode uses the value of **<chset>** specified by **AT+CSCS** command to inform the character set to be used in the message body in the TA-TE interface. 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    113 / 207** 

**LTE Standard Module Series** 



###### **AT+CMGF  Message Format** 

|Test Command|Response|
|---|---|
|**AT+CMGF=?**|**+CMGF:** **(**list of supported**<mode>**s**)**|
||**OK**|
|Read Command<br>**AT+CMGF?**|Response<br>**+CMGF:** **<mode>**|
||**OK**|
|Write Command|Response|
|**AT+CMGF[=<mode>]**|TA sets parameter to denote I/O format of messages to use.<br>**OK**|
|Maximum Response Time|300 ms|
|Characteristics|The command takes effect immediately.<br>The configuration is saved automatically.|
|Reference||
|3GPP TS 27.005||



###### **Parameter** 

|**<mode>**|Integer|type.|
|---|---|---|
||0|PDU mode|
||1|Text mode|



##### **8.3. AT+CSCA  Service Center Address** 

This Write Command updates the SMSC address when mobile originated SMS are transmitted. In text mode, the setting is used by Write Command. In PDU mode, setting is used by the same command, but only when the length of the SMSC address is coded into the **<pdu>** parameter which equals to zero. 

###### **AT+CSCA  Service Center Address** 

|Test Command|Response|
|---|---|
|**AT+CSCA=?**|**OK**|
|Read Command|Response|
|**AT+CSCA?**|**+CSCA: <sca>,<tosca>**|
||**OK**|
|Write Command|Response|
|**AT+CSCA=<sca>[,<tosca>]**|**OK**|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    114 / 207** 

**LTE Standard Module Series** 



||If there is any error related to ME functionality:<br>**+CME ERROR: <err>**|
|---|---|
|Maximum Response Time|300 ms|
|Characteristics|The command takes effect immediately.<br>The configuration is saved automatically.|
|Reference||
|3GPP TS 27.005||



###### **Parameter** 

**<sca>** Service center address. 3GPP TS 24.011 RP SC address Address-Value field in string format; BCD numbers (or GSM 7 bit default alphabet characters) are converted to characters of the currently selected TE character set (see **AT+CSCS** in _3GPP TS 27.007_ ). The type of address is given by **<tosca>** . **<tosca>** Type of service center address. 3GPP TS 24.011 RP SC address Type-of-Address octet in integer format (see **<toda>** ). **<err>** Error codes. For more details, see **_Chapter 12.4_** . 

###### **Example** 

**<mark>AT+CSCA="+8613800210500",145</mark>** <mark>//Set SMSC address.</mark> **<mark>OK AT+CSCA?</mark>** <mark>//Query SMSC address.</mark> **<mark>+CSCA: "+8613800210500",145 OK</mark>** 

##### **8.4. AT+CPMS  Preferred Message Storage** 

This command selects the memory storages **<mem1>** , **<mem2>** and **<mem3>** to be used for reading, writing, etc. 

###### **AT+CPMS  Preferred Message Storage** 

Test Command Response **AT+CPMS=? +CPMS: (** list of supported **<mem1>** s **),(** list of supported **<mem2>** s **),(** list of supported **<mem3>** s **) OK** <mark>Read Command</mark> Response 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    115 / 207** 

**LTE Standard Module Series** 



|**AT+CPMS?**|**+CPMS:**<br>**<mem1>,<used1>,<total1>,<mem2>,<used2>,<total2>,<m**<br>**em3>,<used3>,<total3>**|
|---|---|
||**OK**|
|Write Command|Response|
|**AT+CPMS=<mem1>[,<mem2>[,<mem**<br>**3>]]**|**+CPMS:**<br>**<used1>,<total1>,<used2>,<total2>,<used3>,<total3>**|
||**OK**|
||If there is any error related to ME functionality:<br>**+CMS ERROR: <err>**|
|Maximum Response Time|300 ms|
|Characteristics|The command takes effect immediately.<br>The configurations are saved automatically.|
|Reference||
|3GPP TS 27.005||



###### **Parameter** 

**<mem1>** String type. Memory from which messages are read and deleted. "SM" (U)SIM message storage "ME" Mobile equipment message storage **<mem2>** String type.  Memory to which writing and sending operations are made. "SM" (U)SIM message storage "ME" Mobile equipment message storage **<mem3>** String type. Memory to which received SMs are preferred to be stored (unless forwarded directly to TE ( **AT+CNMI** ). "SM" (U)SIM message storage "ME" Mobile equipment message storage **<usedx>** Integer type. Number of messages currently in **<memx>** . **<totalx>** Integer type. Total number of message locations in **<memx>** . **<err>** Error codes. For more details, see **_Chapter 12.5_** . 

###### **Example** 

**<mark>AT+CPMS?</mark>** <mark>//Query the current SMS message storage.</mark> **<mark>+CPMS: "ME",0,255,"ME",0,255,"ME",0,255</mark>** 

**<mark>OK AT+CPMS="SM","SM","SM"</mark>** <mark>//Set SMS message storage as "SM".</mark> **<mark>+CPMS: 0,50,0,50,0,50</mark>** 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    116 / 207** 

**LTE Standard Module Series** 



**<mark>OK AT+CPMS?</mark>** <mark>//Query the current SMS message storage.</mark> **<mark>+CPMS: "SM",0,50,"SM",0,50,"SM",0,50</mark>** 

**<mark>OK</mark>** 

##### **8.5. AT+CMGD  Delete Message** 

This command deletes short messages from the preferred message storage **<mem1>** location **<index>** . If **<delflag>** is present and not set to 0, the ME ignores **<index>** and follow the rules of **<delflag>** shown below. 

###### **AT+CMGD  Delete Message** 

|Test Command|Response|
|---|---|
|**AT+CMGD=?**|**+CMGD: (**list of supported**<index>**s**),(**list of supported<br>**<delflag>**s**)**|
||**OK**|
|Write Command|Response|
|**AT+CMGD=<index>[,<delflag>]**|TA deletes message from preferred message storage<br>**<mem1>**location**<index>**.<br>**OK**|
||If there is any error related to ME functionality:<br>**+CMS ERROR: <err>**|
||300 ms.|
|Maximum Response Time|Note: Operation of**<delflag>**depends on the storage of<br>deleted messages.|
|Characteristics|The command takes effect immediately.<br>The configuration is not saved.|
|Reference||
|3GPP TS 27.005||



###### **Parameter** 

|**<index>**|Integer|type. Location numbers supported by the associated memory.|
|---|---|---|
|**<delflag>**|Integer|type.|
||0|Delete the message specified in**<index>**|
||1|Delete all read messages from**<mem1>**storage|
||2|Delete all read messages from**<mem1>**storage and sent mobile originated|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    117 / 207** 

**LTE Standard Module Series** 



messages 3 Delete all read messages from **<mem1>** storage, sent and unsent mobile originated messages 

4 Delete all messages from **<mem1>** storage **<mem1>** String type. Memory from which messages are read and deleted. "SM" (U)SIM message storage "ME" Mobile equipment message storage **<err>** Error codes. For more details, see **_Chapter 12.5_** . 

###### **Example** 

**<mark>AT+CMGD=1</mark>** <mark>//Delete the message specified in</mark> **<mark><index>=1</mark>** <mark>.</mark> **<mark>OK AT+CMGD=1,4</mark>** <mark>//Delete all messages from</mark> **<mark><mem1></mark>** <mark>storage.</mark> **<mark>OK</mark>** 

##### **8.6. AT+CMGL  List Message** 

The Read Command returns messages with status value **<stat>** from preferred message storage **<mem1>** to the TE. If the status of the message is "REC UNREAD", the status in the storage changes to "REC READ". When executing **AT+CMGL** without status value **<stat>** , it will report the list of SMS with "REC UNREAD" status. 

###### **AT+CMGL  List Message** 

<mark>Test Command</mark> Response **AT+CMGL=? +CMGL: (** list of supported **<stat>** s **) OK** <mark>Write Command</mark> Response **AT+CMGL=[<stat>]** If in text mode ( **AT+CMGF=1** ) and the command is executed successfully: For SMS-SUBMITs and/or SMS-DELIVERs: **+CMGL: <index>,<stat>,<oa/da>,[<alpha>],[<scts>] [,<tooa/toda>,<length>]<CR><LF><data>[<CR><LF> +CMGL: <index>,<stat>,<da/oa>,[<alpha>],[<scts>], [<tooa/toda>,<length>]<CR><LF><data>[...]]** For SMS-STATUS-REPORTs: **+CMGL: <index>,<stat>,<fo>,<mr>,[<ra>],[<tora>], <scts>,<dt>,<st>[<CR><LF>** 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    118 / 207** 

**LTE Standard Module Series** 



||**+CMGL:** **<index>,<stat>,<fo>,<mr>,[<ra>],[<tora>],**<br>**<scts>,<dt>,<st>[...]]**|
|---|---|
||For SMS-COMMANDs:<br>**+CMGL:** **<index>,<stat>,<fo>,<ct>[<CR><LF>**<br>**+CMGL: <index>,<stat>,<fo>,<ct>[...]]**|
||For CBM storage:<br>**+CMGL: <index>,<stat>,<sn>,<mid>,<page>,<pages>**<br>**<CR><LF><data>[<CR><LF>**<br>**+CMGL:** **<index>,<stat>,<sn>,<mid>,<page>,<pages>**<br>**<CR><LF><data>[...]]**|
||**OK**|
||If in PDU mode (**AT+CMGF=0**) and the command is executed<br>successfully:<br>**+CMGL: <index>,<stat>,[<alpha>],<length><CR><LF>**<br>**<pdu><CR><LF>**<br>**+CMGL: <index>,<stat>,[alpha],<length><CR><LF>**<br>**<pdu>[...]]**|
||**OK**|
||If there is any error related to ME functionality:<br>**+CMS ERROR: <err>**|
|Execution Command<br>**AT+CMGL**|Response<br>List all messages with "REC UNREAD" status from message<br>storage**<mem1>**, and then the status in the storage changes<br>to "REC READ".|
|Maximum Response Time|300 ms.|
|Characteristics|The command takes effect immediately.<br>The configurations are not saved.|
|Reference<br>3GPP TS 27.005||



###### **Parameter** 

**<stat>** String type. In text mode: "REC UNREAD"      Received unread messages "REC READ"         Received read messages "STO UNSENT"     Stored unsent messages "STO SENT"         Stored sent messages 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    119 / 207** 

**LTE Standard Module Series** 



|<br>|"ALL"<br>All messages<br>Integer type. In PDU mode:|
|---|---|
||0<br>Received unread messages|
|<br>|1<br>Received read messages<br>2<br>Stored unsent messages|
|<br>|3<br>Stored sent messages<br>4<br>All messages|
|**<index>**|Integer type. Location numbers supported by the associated memory.|
|**<da>**|Destination Address. 3GPP TS 23.040 TP-Destination-Address Address-Value field in<br>string format. BCD numbers (or GSM 7 bit default alphabet characters) are<br>converted to characters of the currently selected TE character set (see**AT+CSCS**<br>in_3GPP TS 27.007_). The type of address is given by**<toda>**.|
|**<oa>**|Originating address. 3GPP TS 23.040 TP-Originating-Address Address-Value field in<br>string format. BCD numbers (or GSM 7 bit default alphabet characters) are converted<br>to characters of the currently selected TE character set (see**AT+CSCS**in_3GPP TS_<br>_27.007_). The type of address is given by**<tooa>**.|
|**<alpha>**|String type alphanumeric representation of**<da>**or**<oa>**corresponding to the entry<br>found in MT phonebook. Implementation of this feature is manufacturer specified. The<br>used character set should be the one selected with**AT+CSCS**command (see<br>definition of this command in_3GPP TS 27.007_).|
|**<scts>**|Service center time stamp. 3GPP TS 23.040 TP-Service-Centre-Time-Stamp in<br>time-string format (see**<dt>**).|
|**<toda>**|Type of recipient address. 3GPP TS 24.011 TP-Destination-Address Type-of-Address<br>octet in integer format.|
|**<tooa>**|Type<br>of<br>originating<br>address.<br>3GPP<br>TS<br>24.011<br>TP-Originating-Address<br>Type-of-Address  octet in integer format (defalt see**<toda>**).|
|**<length>**|Integer type. Message length. Indicate the length of the message body**<data>**(or<br>**<cdata>**) in characters in the text mode (**AT+CMGF=1**), or the length of the actual TP<br>data unit in octets in PDU mode (**AT+CMGF=0**) (i.e. the RP layer SMSC address<br>octets are not counted in the length).|
|**<data>**|In case of SMS: 3GPP TS 23.040 TP-User-Data in text mode responses; format:<br>-  If**<dcs>**(see_Chapter 9.7_), indicates that 3GPP TS 23.038 GSM 7 bit default<br>alphabet is used and**<fo>**(see_Chapter 9.7_) indicates that 3GPP TS 23.040<br>TP-User-Data-Header-Indication is not set.<br>-  If TE character set other than "HEX" (refer to**AT+CSCS**in_3GPP TS 27.007_):<br>ME/TA converts GSM alphabet into current TE character set according to rules of<br>**_Annex A_**in_3GPP TS 27.007_.<br>-  If TE character set is "HEX": ME/TA converts each 7-bit character of GSM 7 bit<br>default alphabet into two IRA character long hexadecimal number (e.g. character<br>(GSM 7 bit default alphabet 23) is presented as 17 (IRA 49 and 55)).|



- If **<dcs>** , indicates that 8-bit or UCS2 data coding scheme is used, or **<fo>** indicates that 3GPP TS 23.040 TP-User-Data-Header-Indication is set: ME/TA converts each 8-bit octet into two IRA character long hexadecimal number (e.g. octet with integer value 42 is presented to TE as two characters 2A (IRA 50 and 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    120 / 207** 

**LTE Standard Module Series** 



###### 65)). 

In case of CBS: 3GPP TS 23.041 CBM Content of Message in text mode responses; format: 

- If **<dcs>** , indicates that 3GPP TS 23.038 GSM 7 bit default alphabet is used: 

   - If TE character set other than "HEX" (see **AT+CSCS** in _3GPP TS27.007_ ): ME/TA converts GSM alphabet into current TE character set according to rules of **_Annex A_** in _3GPP TS 27.007_ . 

-  If TE character set is "HEX": ME/TA converts each 7-bit character of the GSM 7 bit default alphabet into two IRA character long hexadecimal number. -  If **<dcs>** , indicates that 8-bit or UCS2 data coding scheme is used: ME/TA converts each 8-bit octet into two IRA character long hexadecimal number. **<pdu>** In case of SMS: 3GPP TS 24.011 SC address followed by 3GPP TS 23.040 TPDU in hexadecimal format: ME/TA converts each octet of TP data unit into two IRA character long hexadecimal number (e.g. octet with integer value 42 is presented to TE as two characters 2A (IRA 50 and 65)) 3GPP TS 27.007. **<fo>** Depending on the command or result code: first octet of 3GPP TS 23.040 [3] SMS-DELIVER, SMS-SUBMIT (17 by default), SMS-STATUS-REPORT, or SMS-COMMAND (2 by default) in integer format. **<mr>** 3GPP TS 23.040 [3] TP-Message-Reference in integer format. **<ra>** 3GPP TS 23.040 [3] TP-Recipient-Address Address-Value field in string format; BCD numbers (or GSM 7 bit default alphabet characters) are converted to characters of the currently selected TE character set (see +CSCS in 3GPP TS 27.007 [9]); type of address given by **<tora>** . **<tora>** 3GPP TS 24.011 [6] TP-Recipient-Address Type-of-Address octet in integer  format (default see **<toda>** ). **<scts>** 3GPP TS 23.040 [3] TP-Service-Centre-Time-Stamp in time-string format (see **<dt>** ) **<dt>** 3GPP TS 23.040 [3] TP-Discharge-Time in time-string format: "yy/MM/dd,hh:mm:ss+zz", where characters indicate year (two last digits), month, day, hour, minutes, seconds and time zone. E.g. 6th of May 1994,  22:10:00 GMT+2 hours equals "94/05/06,22:10:00+08". **<st>** 3GPP TS 23.040 [3] TP-Status in integer format. **<ct>** 3GPP TS 23.040 [3] TP-Command-Type in integer format (0 by default). **<sn>** 3GPP TS 23.041 [4] CBM Serial Number in integer format. **<mid>** 3GPP TS 23.041 [4] CBM Message Identifier in integer format. **<page>** 3GPP TS 23.041 [4] CBM Page Parameter bits 4-7 in integer format. **<pages>** 3GPP TS 23.041 [4] CBM Page Parameter bits 0-3 in integer format. **<mem1>** Memory from which messages are read and deleted "SM" (U)SIM message storage "ME" Mobile equipment message storage **<err>** Error codes. For more details, see **_Chapter 12.5_** . 

###### **Example** 

**<mark>AT+CMGF=1</mark>** <mark>//Set SMS message format as text mode.</mark> 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    121 / 207** 



**LTE Standard Module Series** 

**<mark>OK AT+CMGL="ALL"</mark>** <mark>//List all messages from message storage.</mark> **<mark>+CMGL: 1,"STO UNSENT","",,</mark> <This is a test from Quectel>** **<mark>+CMGL: 2,"STO UNSENT","",,</mark> <This is a test from Quectel>** **<mark>OK</mark>** 

**NOTE** 

Operation of **<stat>** depends on the storage of listed messages. 

##### **8.7. AT+CMGR  Read Message** 

This Write Command returns SMS messages with location value **<index>** from message storage **<mem1>** to the TE. If status of the message is "REC UNREAD", status in the storage changes to "REC READ". 

|**AT+CMGR  Read Message**||
|---|---|
|Test Command|Response|
|**AT+CMGR=?**|**OK**|
|Write Command|Response|
|**AT+CMGR=<index>**|In Non-CDMA mode:<br>If in text mode (**AT+CMGF=1**) and the command is executed<br>successfully:<br>For SMS-DELIVER:<br>**+CMGR: <stat>,<oa>,[<alpha>],<scts>[,<tooa>,<fo>,**<br>**<pid>,<dcs>,<sca>,<tosca>,<length>]<CR><LF><data>**|
||**OK**|
||For SMS-SUBMIT:<br>**+CMGR: <stat>,<da>,[<alpha>][,<toda>,<fo>,<pid>,**<br>**<dcs>,[<vp>],<sca>,<tosca>,<length>]<CR><LF><data>**|
||**OK**|
||For SMS-STATUS-REPORTs:<br>**+CMGR: <stat>,<fo>,<mr>,[<ra>],[<tora>],<scts>,<dt>,**|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    122 / 207** 

**LTE Standard Module Series** 



||**<st>**|
|---|---|
||**OK**|
||For SMS-COMMANDs:<br>**+CMGR: <stat>,<fo>,<ct>[,<pid>,[<mn>],[<da>],[<toda>],**<br>**<length><CR><LF><cdata>]**|
||**OK**<br>For CBM storage:<br>**+CMGR: <stat>,<sn>,<mid>,<dcs>,<page>,<pages>**<br>**<CR><LF><data>**|
||**OK**|
||If in PDU mode (**AT+CMGF=0**) and command is executed<br>successfully:<br>**+CMGR: <stat>,[<alpha>],<length><CR><LF><pdu>**|
||**OK**|
||In CDMA Text mode:<br>**+CMGR: <stat>,<oa/da>,<scts>,<alpha>,<tooa/toda>,**<br>**<lang>,<fmt>,<length>,<prt>,<prv>,<type><CR><LF>**<br>**<data>**<br>**OK**<br>If there is any error related to ME functionality:<br>**+CMS ERROR: <err>**|
|Maximum Response Time|Depends on the length of message content.|
|Characteristics|The command takes effect immediately.<br>The configurations are not saved.|
|Reference<br>3GPP TS 27.005||



###### **Parameter** 

|**<index>**|Integer type. Value|in the range of location numbers supported by the associated|
|---|---|---|
||memory.||
|**<stat>**|String type. In text m|ode.<br>|
||"REC UNREAD"|Received unread messages|
||"REC READ"<br>|Received read messages|
||"STO UNSENT"<br>|Stored unsent messages|
||"STO SENT"<br>|Stored sent messages|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    123 / 207** 

**LTE Standard Module Series** 



||"ALL"<br>All messages<br>Integer type. In PDU mode.|
|---|---|
||0<br>Received unread messages<br>1<br>Received read messages<br>2<br>Stored unsent messages|
||3<br>Stored sent messages|
||4<br>All messages|
|**<alpha>**|String type alphanumeric representation of**<da>**or**<oa>**corresponding to the entry<br>found in MT phonebook. Implementation of this feature is manufacturer specified. The<br>used character set should be the one selected with**AT+CSCS**command (see definition<br>of this command in_3GPP TS 27.007_).|
|**<da>**|Destination address. 3GPP TS 23.040 TP-Destination-Address Address-Value field in<br>string format. BCD numbers (or GSM 7 bit default alphabet characters) are converted to<br>characters of the currently selected TE character set (refer to**AT+CSCS**in_3GPP TS_<br>_27.007_). The type of address is given by**<toda>**.|
|**<oa>**|Originating address. 3GPP TS 23.040 TP-Originating-Address Address-Value field in<br>string format. BCD numbers (or GSM 7 bit default alphabet characters) are converted to<br>characters of the currently selected TE character set (refer to**AT+CSCS**in_3GPP TS_<br>|
||_27.007_). The type of address is given by**<tooa>**.|
|**<scts>**|<br>Service center time stamp. 3GPP TS 23.040 TP-Service-Centre-Time-Stamp in<br>time-string format (refer to**<dt>**).|
|**<fo>**|First octet. Depending on the command or result code: First octet of 3GPP TS 23.040<br>SMS-DELIVER,<br>SMS-SUBMIT<br>(17<br>by<br>default),<br>SMS-STATUS-REPORT,<br>or<br>SMS-COMMAND in integer format. If a valid value has been entered once, the<br>parameter can be omitted.|
|**<pid>**|Protocol identifier. 3GPP TS 23.040 TP-Protocol-Identifier in integer format (0 by<br>default).|
|**<dcs>**|Data coding scheme. Depending on the command or result code: 3GPP TS 23.038<br>SMS Data Coding Scheme (0 by default), or Cell Broadcast Data Coding Scheme in<br>integer format.|
|**<vp>**|Validity period. Depending on SMS-SUBMIT**<fo>**setting: 3GPP TS 23.040<br>TP-Validity-Period either in integer format or in time-string format (refer to**<dt>**).|
|**<mn>**|3GPP TS 23.040 TP-Message-Number in integer format.|
|**<mr>**|3GPP TS 23.040 TP-Message-Reference in integer format.|
|**<ra>**|3GPP TS 23.040 TP-Recipient-Address Address-Value field in string format. BCD<br>numbers (or GSM default alphabet characters) are converted to characters of the<br>currently selected TE character set (refer to**AT+CSCS**command). The type of address<br>is given by**<tora>**.|
|**<tora>**|3GPP TS 24.011 TP-Destination-Address Type-of-Address octet in integer format<br>(default refer**<toda>**).|
|**<toda>** <br>**<tooa>**|3GPP TS 24.011 TP-Destination-Address Type-of-Address octet in integer format.<br>3GPP TS 24.011 TP-Originating-Address Type-of-Address octet in integer format<br>(default see**<toda>**).|
|**<sca>**|Service center address. 3GPP TS 24.011 RP SC address Address-Value field in string|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    124 / 207** 

**LTE Standard Module Series** 



format. BCD numbers (or GSM 7 bit default alphabet characters) are converted to characters of the currently selected TE character set (refer to **AT+CSCS** command in _3GPP TS 27.007_ ). The type of address is given by **<tosca>** . 

**<tosca>** 3GPP TS 24.011 RP SC address Type-of-Address octet in integer format (default see **<toda>** ). 

**<length>** Integer type. Message length. In the text mode ( **AT+CMGF=1** ) the length of the message body **<data>** (or **<cdata>** ) in characters, or in PDU mode ( **AT+CMGF=0** ) the length of the actual TP data unit in octets (i.e. the RP layer SMSC address octets are not counted in the length). 

- **<data>** User-Data in text mode responses. See **_Chapter 12.8_** for details. 

**<pdu>** In case of SMS: 3GPP TS 24.011 SC address followed by 3GPP TS 23.040 TPDU in hexadecimal format: ME/TA converts each octet of TP data unit into two IRA character long hexadecimal number (e.g. octet with integer value 42 is presented to TE as two characters 2A (IRA 50 and 65)). 

- **<prt>** Priority. 

0   Normal 

      - 1   Interactive 

      - 2   Urgent 3   Emergency 

- **<fmt>** Format. 

   - 0   GSM 7-bit 

   - 1   ASCII 

6   UNICODE **<prv>** Privacy. 

0   Normal 1   Restricted 2   Confidential 3   Secret 

**<lang>** Language. 

0   Unspecified 1   English 2   French 3   Spanish 4   Japanese 5   Korean 

6   Chinese 7   Hebrew 

**<type>** 0   Normal 

1   CPT 2   Voice Mail 3   SMS Report **<mem1>** String type. Memory storage from which messages are to be read and deleted. "SM" (U)SIM message storage 

- "ME" Mobile equipment message storage 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    125 / 207** 

**LTE Standard Module Series** 



**<err>** Error codes. For more details, see **_Chapter 12.5_** . 

###### **Example** 

|**+CMTI: "SM",3**|//Indicates that new message has been received and saved<br>to**<index>**=3 of "SM".|
|---|---|
|**AT+CSDH=1**||
|**OK**||
|**AT+CMGR=3**|//Read message.|
|**+CMGR:**<br>**"REC**<br>**UNREAD","+861**|**5021012496",,"13/12/13,15:06:37+32",145,4,0,0,"+861380021050**|
|**0",145,27**||
|**<This is a test from Quectel>**||
|**OK**||



##### **8.8. AT+CMGS  Send Message** 

This command sends a short message from TE to network (SMS-SUBMIT). After invoking the Write Command, wait for the prompt **>** and then start to write the message. After that, enter **<CTRL-Z>** to indicate the ending of PDU and begin to send the message. Sending can be cancelled by entering **<ESC>** character. Abortion is acknowledged with **OK** , though the message will not be sent. The message reference **<mr>** is returned to the TE on successful message delivery. The value can be used to identify message upon unsolicited delivery status report result code. 

###### **AT+CMGS  Send Message** 

|Test Command|Response|
|---|---|
|**AT+CMGS=?**|**OK**|
|Write Command|Response|
|1) If text mode (**AT+CMGF=1**):|TA sends message from TE to the network (SMS-SUBMIT).|
|**AT+CMGS=<da>[,<toda>]**<CR>|Message reference value**<mr>**is returned to the TE on|
|**text is entered**|successful message delivery. Optionally (when**AT+CSMS**|
|**<Ctrl+Z/ESC>**|**<service>**value is 1 and network supports)**<scts>**is|
|ESC quits without sending|returned. Values can be used to identify message upon<br>unsolicited delivery status report result code.|
|2) If PDU mode (**AT+CMGF=0**):<br>**AT+CMGS=<length>**<CR>|If in text mode (**AT+CMGF=1**) and sent successfully:<br>**+CMGS:** **<mr>**|
|**PDU is given** **<Ctrl+Z/ESC>**|**OK**|
||If in PDU mode (**AT+CMGF=0**) and sent successfully:<br>**+CMGS:** **<mr>**|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    126 / 207** 

**LTE Standard Module Series** 



||**OK**|
|---|---|
||If there is any error related to ME functionality:<br>**+CMS ERROR: <err>**|
|Maximum Response Time|120 s, determined by network.|
|Characteristics|The command takes effect immediately.<br>The configurations are not saved.|
|Reference||
|3GPP TS 27.005||



###### **Parameter** 

- **<da>** Destination address. 3GPP TS 23.040 TP-Destination-Address Address-Value field in string format. BCD numbers (or GSM 7-bit default alphabet characters) are converted to characters of the currently selected TE character set (refer to **AT+CSCS** in _3GPP TS 27.007_ ). The type of address is given by **<toda>** . 

- **<toda>** Type of destination address. 3GPP TS 24.011 TP-Destination-Address Type-of-Address octet in integer format. 

- **<length>** Integer type. Message length. In the text mode ( **AT+CMGF=1** ) the length of the message body **<data>** (or **<cdata>** ) in characters, or in PDU mode ( **AT+CMGF=0** ), the length of the actual TP data unit in octets (i.e. the RP layer SMSC address octets are not counted in the length). 

   - Maximum length in text mode: 160 bytes. Maximum length in PDU mode: 140 bytes. 

**<mr>** 3GPP TS 23.040 TP-Message-Reference in integer format. 

**<err>** Error codes. For more details, see **_Chapter 12.5_** . 

###### **NOTE** 

1. For concatenated messages, the maximum length will be reduced by the length of the user data header (UDH). _3GPP TS 23.040_ defines two kinds of UDH length: 6 bytes and 7 bytes, so the two kinds of **<uid>** are 8-bit (6 bytes) and 16-bit (7 bytes). **AT+QCMGS** uses 8-bit **<uid>** . 

   - ⚫ In case of GSM 7-bit default alphabet data coding scheme, the maximum length of each segment of a concatenated message is (140 octets - 6 ) × 8/7=153 characters. 

   - ⚫ In case of 16 bit UCS2 data coding scheme, the maximum length of each segment is (140-6)/2=67 characters. 

   - ⚫ In case of 8-bit data coding scheme, the maximum length of each segment is 140-6=134 characters. 

2. **<mr>** Message-Reference field gives an integer representation of a reference number of the SMS-SUBMIT or SMS-COMMAND submitted to the SC by the MS, and it is used to confirm whether the SMS-DELIVER has been received from SC duplicate or not. 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    127 / 207** 

**LTE Standard Module Series** 



   - **<uid>** The field of UDH. It is a message identification of the concatenated SMS, which is different from **<mr>** . Each segment in a concatenated message should have the same **<uid>** , but **<mr>** must be incremented for each segment of a  concatenated message. 

3. **AT+QCMGS** does not support sending messages in PDU mode ( **AT+CMGF=0** ). 

###### **Example** 

**<mark>AT+CMGF=1</mark>** <mark>//Set SMS message format as text mode.</mark> **<mark>OK AT+CSCS="GSM"</mark>** <mark>//Set character set used by the TE as GSM.</mark> **<mark>OK AT+CMGS="15021012496"</mark> > <This is a test from Quectel>** //Enter text. Use **<CTRL+Z>** to send messages, or **<mark><ESC></mark>** <mark>to quit without sending.</mark> **<mark>+CMGS: 247 OK</mark>** 

##### **8.9. AT+CMMS  More Messages to Send** 

This command controls the continuity of the SMS relay protocol link. If the feature is enabled (and supported by the currently used network) multiple messages can be sent faster as the link is kept open. 

|**AT+CMMS  More Messages**|**to Send**|
|---|---|
|Test Command|Response|
|**AT+CMMS=?**|**+CMMS: (**list of supported**<n>**s**)**|
||**OK**|
|Read Command|Response|
|**AT+CMMS?**|**+CMMS: <n>**|
||**OK**|
|Write Command<br>**AT+CMMS=<n>**|Response<br>**OK**<br>Or<br>**ERROR**|
||If there is any error related to ME functionality:<br>**+CMS ERROR: <err>**|
|Maximum Response Time|120 s, determined by network.|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    128 / 207** 

**LTE Standard Module Series** 



The command takes effect immediately. Characteristics The configuration is saved automatically. Reference 3GPP TS 27.005 

###### **Parameter** 

|**<n>**Integ|er type.|
|---|---|
|0|Feature disabled|
|1|Keep enabled until the time between the response of the latest message send<br>command (**AT+CMGS**,**AT+CMSS**, etc.) and the next send command exceeds|
||1-5 s (the exact value is up to ME implementation), and then ME closes link and<br>TA switches**<n>**back to 0 automatically|
|2|Feature enabled (if the time between the response of the latest message send<br>command and the next send command exceeds 1-5 s (the exact value is|
||up to ME implementation), ME closes the link but TA does not switch**<n>**back|
||to 0 automatically)|
|**<err>**Error|codes. For more details, see**_Chapter 12.5_**.|



###### **NOTE** 

Once Read Command is executed, a delay of 5-10 s is required before issuing the Write Command. Otherwise **+CMS ERROR: 500** may be returned. 

##### **8.10. AT+CMGW  Write Message to Memory** 

This Write and Execution Commands store short messages from TE to memory storage **<mem2>** , and then the memory location **<index>** of the stored message is returned. Message status is set to "stored unsent" by default, but **<stat>** allows other status values to be given, as well. 

The syntax of input text is the same as the one specified in **AT+CMGS** Write Command. 

|**AT+CMGW  Write Message to Me**|**mory**|
|---|---|
|Test Command|Response|
|**AT+CMGW=?**|**OK**|
|Write Command|Response|
|1) If text mode (**AT+CMGF=1**):|If writing is successful:|
|**AT+CMGW=<oa/da>[,<tooa/toda>[,<s**<br>**tat>]]**<CR><br>**text is entered**<br>**<Ctrl+Z/ESC>**|**+CMGW:** **<index>**<br>**OK**|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    129 / 207** 

**LTE Standard Module Series** 



|**<ESC>**quits without sending|If there is any error related to ME functionality:<br>**+CMS ERROR: <err>**|
|---|---|
|2) If PDU mode (**AT+CMGF=0**):<br>**AT+CMGW=<length>[,<stat>]**<CR>||
|**PDU isgiven** **<Ctrl+Z/ESC>**||
|Maximum Response Time|300 ms|
|Characteristics|The command takes effect immediately.<br>The configurations are not saved.|
|Reference||
|3GPP TS 27.005||



###### **Parameter** 

- **<da>** Destination address. 3GPP TS 23.040 TP-Destination-Address Address-Value field in string format. BCD numbers (or GSM 7 bit default alphabet characters) are converted to characters of the currently selected TE character set (see **AT+CSCS** in _3GPP TS 27.007_ ). The type of address is given by **<toda>** . 

**<oa>** Originating address. 3GPP TS 23.040 TP-Originating-Address Address-Value field in string format. BCD numbers (or GSM 7 bit default alphabet characters) are converted to characters of the currently selected TE character set (see **AT+CSCS** in _3GPP TS 27.007_ ). The type of address given by **<tooa>** . **<tooa>** 3GPP TS 24.011 TP-Originating-Address Type-of-Address octet in integer format (default see **<toda>** ). **<stat>** PDU mode     Text mode Explanation 0 "REC UNREAD"   Received unread messages 1 "REC READ" Received read messages 2 "STO UNSENT" Stored unsent messages 3 "STO SENT" Stored sent messages 4 "ALL" All messages **<toda>** 3GPP TS 24.011 TP-Destination-Address Type-of-Address octet in integer format. **<length>** Integer type. Message length. Length of the message body **<data>** (or **<cdata>** ) in characters in the text mode ( **AT+CMGF=1** ), or the length of the actual TP data unit in octets (i.e. the RP layer SMSC address octets are not counted in the length) in PDU mode ( **AT+CMGF=0** ),. 

- **<pdu>** In case of SMS: 3GPP TS 24.011 SC address followed by 3GPP TS 23.04TPDU in hexadecimal format: ME/TA converts each octet of TP data unit into two IRA character long hexadecimal number (e.g. octet with integer value 42 is presented to TE as two characters 2A (IRA 50 and 65)). 

- **<index>** Integer type. Index of message in selected storage **<mem2>** . **<err>** Error codes. For more details, see **_Chapter 12.5_** . 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    130 / 207** 

**LTE Standard Module Series** 



###### **Example** 

**<mark>AT+CMGF=1</mark>** <mark>//Set SMS message format as text mode.</mark> **<mark>OK AT+CSCS="GSM"</mark>** <mark>//Set character set used by the TE as GSM.</mark> **<mark>OK AT+CMGW="15021012496"</mark> > <This is a test from Quectel>** //Enter in text. Use **<CTRL+Z>** to write message or **<mark><ESC></mark>** <mark>to quit without sending.</mark> **+CMGW: 4** **<mark>OK AT+CMGF=0</mark>** <mark>//Set SMS message format as PDU mode.</mark> **<mark>OK AT+CMGW=18 > 0051FF00000008000A0500030002016D4B8BD5 +CMGW: 5 OK</mark>** 

##### **8.11. AT+CMSS  Send Message from Storage** 

This Write Command sends a message with location value **<index>** from message storage **<mem2>** to the network (SMS-SUBMIT). If new destination address **<da>** is given, it should be used instead of the one stored with the message. Reference value **<mr>** is returned to the TE on successful message delivery. Values can be used to identify message upon unsolicited delivery status report of result code. 

###### **AT+CMSS  Send Message from Storage** 

|Test Command|Response|
|---|---|
|**AT+CMSS=?**|**OK**|
|Write Command|Response|
|**AT+CMSS=<index>[,<da>[,<toda>]]**|If in text mode (**AT+CMGF=1**) and sent successfully:<br>**+CMSS:** **<mr>[,<scts>]**|
||**OK**|
||If in PDU mode (**AT+CMGF=0**) and sent successfully:<br>**+CMSS:** **<mr> [,<ackpdu>]**|
||**OK**|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    131 / 207** 

**LTE Standard Module Series** 



||If there is any error related to ME functionality:<br>**+CMS ERROR: <err>**|
|---|---|
|Maximum Response Time|120 s, determined by network.|
|Characteristics|The command takes effect immediately.<br>The configurations are not saved.|
|Reference||
|3GPP TS 27.005||



###### **Parameter** 

- **<index>** Integer type. Value in the range of location numbers supported by the associated memory. 

- **<da>** Destination Address. 3GPP TS 23.040 TP-Destination-Address Address-Value field in string format. BCD numbers (or GSM 7 bit default alphabet characters) are converted to characters of the currently selected TE character set (see **AT+CSCS** in _3GPP TS 27.007_ ). The type of address is given by **<toda>** . 

- **<toda>** Type of destination address. 3GPP TS 24.011 TP-Destination-Address Type-of-Address octet in integer format. 

- **<mr>** 3GPP TS 23.040 TP-Message-Reference in integer format. 

- **<scts>** Service center time stamp. 3GPP TS 23.040 TP-Service-Centre-Time-Stamp in time-string format (see **<dt>** ). 

- **<ackpdu>** The format is the same as for **<pdu>** in case of SMS, but without 3GPP TS 24.011 SC address field and the parameter is bound by double quote characters like a normal string type parameter. 

- **<err>** Error codes. For more details, see **_Chapter 12.5_** . 

###### **Example** 

**<mark>AT+CMGF=1</mark>** <mark>//Set SMS message format as text mode.</mark> **<mark>OK AT+CSCS="GSM"</mark>** <mark>//Set character set used by the TE as GSM.</mark> **<mark>OK AT+CMGW="15021012496" > Hello</mark>** <mark>//Enter in text. Use</mark> **<mark><CTRL+Z></mark>** <mark>to send message or</mark> **<mark><ESC></mark>** <mark>to quit without sending.</mark> **<mark>+CMGW: 4 OK AT+CMSS=4</mark>** <mark>//Send the message of index 4 from memory storage.</mark> **<mark>+CMSS: 54 OK</mark>** 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    132 / 207** 

**LTE Standard Module Series** 



##### **8.12. AT+CNMA  New Message Acknowledgement to UE/TE** 

The Write and Execution Commands confirm successful receipt of a new message (SMS-DELIVER or SMS-STATUS-REPORT) routed directly to the TE. If the UE does not receive acknowledgement within required time (network timeout), it sends an **RP-ERROR** message to the network. The UE will automatically disable routing to the TE by setting both **<mt>** and **<ds>** values of **AT+CNMI** to 0. 

###### **AT+CNMA  New Message Acknowledgement to UE/TE** 

|Test Command|Response|
|---|---|
|**AT+CNMA=?**|**+CNMA: (**list of supported**<n>**s**)**|
||**OK**|
|Execution Command<br>**AT+CNMA**|Response<br>**OK**<br>Or<br>**ERROR**|
||If there is any error related to ME functionality:<br>**+CMS ERROR: <err>**|
|Write Command<br>**AT+CNMA=<n>**|Response<br>**OK**<br>Or<br>**ERROR**|
||If there is any error related to ME functionality:<br>**+CMS ERROR: <err>**|
|Maximum Response Time|300 ms|
|Characteristics|/|
|Reference||
|3GPP TS 27.005||



###### **Parameter** 

- **<n>** Integer type. Parameter required only for PDU mode. 

   - 0     Command operates similarly as in text mode 

   - 1     Send positive (RP-ACK) acknowledgement to the network. Accepted only in PDU mode 

- 2     Send negative (RP-ERROR) acknowledgement to the network. Accepted only in PDU mode 

**<err>** Error code. For more details, see **_Chapter 12.5_** . 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    133 / 207** 

**LTE Standard Module Series** 



###### **NOTE** 

The Execution and Write Commands will only be used when **AT+CSMS** parameter **<service>** equals 1 (phase 2+) and an appropriate URC has been issued by the module, i.e.: **+CMT** for **<mt>** =2 incoming message classes 0,1,3 and none; **+CMT** for **<mt>** =3 incoming message classes 0 and 3; **+CDS** for **<ds>** =1. 

###### **Example** 

**<mark>AT+CSMS=1 +CSMS: 1,1,1 OK AT+CNMI=1,2,0,0,0 OK +CMT: "+8615021012496",,"13/03/18,17:07:21+32",145,4,0,0,"+8613800551500",145,28 This is a test from Quectel.</mark>** <mark>//Short message is outputted directly when an SMS is incoming.</mark> **<mark>AT+CNMA</mark>** <mark>//Send ACK to the network.</mark> **<mark>OK AT+CNMA</mark>** 

**<mark>+CMS ERROR</mark>** <mark>：</mark> **<mark>340</mark>** <mark>// Returned error for the second time. It needs ACK only once.</mark> 

##### **8.13. AT+CNMI  SMS Event Reporting Configuration** 

This Write Command selects the procedure on how the received new messages from the network are indicated to the TE when TE is active, e.g. DTR is at low level (ON). If TE is inactive (e.g. DTR is at high level (OFF)), message should be received as specified in _3GPP TS 23.038_ . 

|**AT+CNMI  New Message Indications to TE**|||
|---|---|---|
|Test Command<br>Response|||
|**AT+CNMI=?**<br>**+CNMI: (**list of supported**<mode>**s**),(**list<br>**<mt>**s**),(**list<br>of<br>supported<br>**<bm>**s**),(**list<br>**<ds>**s**),(**list of supported**<bfr>**s**)**|of<br>of|supported<br>supported|
|**OK**|||
|Read Command<br>Response|||
|**AT+CNMI?**<br>**+CNMI: <mode>,<mt>,<bm>,<ds>,<bfr>**|||



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    134 / 207** 

**LTE Standard Module Series** 



||**OK**|
|---|---|
|Write Command|Response|
|**AT+CNMI[=<mode>[,<mt>[,<bm>[,<ds**|**OK**|
|**>[,<bfr>]]]]]**|Or<br>**ERROR**|
||If there is any error related to ME functionality:<br>**+CMS ERROR: <err>**|
|Maximum Response Time|300 ms|
|Characteristics|The command takes effect immediately.<br>The configurations are not saved automatically.|
|Reference||
|3GPP TS 27.005||



###### **Parameter** 

- **<mode>** Integer type. 0 Buffer unsolicited result codes in the TA. If TA result code buffer is full, indications can be buffered in some other place or the oldest indications may be discarded and replaced with the new received indications. 

- 1      Discard indication and reject new received message unsolicited result codes when TA-TE link is reserved (e.g. in on-line data mode). Otherwise forward them directly to the TE. 

- <u>2</u> Buffer unsolicited result codes in the TA when TA-TE link is reserved (e.g. in data mode) and flush them to the TE after reservation. Otherwise forward them directly to the TE. 

- **<mt>** Integer type. The rules for storing received SMS depend on its data coding scheme (refer to _3GPPTS 23.038_ ) and preferred memory storage ( **AT+CPMS** ) setting, and the value is: 

- 0 No SMS-DELIVER indications are routed to the TE. <u>1</u> If SMS-DELIVER is stored into ME/TA, indication of the memory location is routed to the TE by using unsolicited result code: **+CMTI: <mem>,<index>** 

- 2 SMS-DELIVERs (except class 2) are routed directly to the TE using unsolicited result code: **+CMT: [<alpha>],<length><CR><LF><pdu>** (PDU mode enabled) or **+CMT: <oa>,[<alpha>],<scts>[,** **_<tooa>,<fo>,<pid>,<dcs>,<sca>,<tosca>, <length>_ ]<CR><LF><data>** (text mode enabled; about the parameters in italics, see **AT+CSDH** ) or **^HCMT: <oa>,<scts>,<lang>,<fmt>,<length>, <prt>,<prv>,<type>,<stat><CR><LF><data>** (text mode for CDMA SMS). Class 2 messages result in indication as defined in **<mt>** =1. 

- 3 Class 3 SMS-DELIVERs are routed directly to TE by using unsolicited result codes defined in **<mt>** =2. Messages of other classes result in indication as defined in **<mt>** =1. 

- **<bm>** Integer type. The rules for storing received CBMs depend on its data coding scheme (see 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    135 / 207** 

**LTE Standard Module Series** 



- _3GPP TS 23.038_ ) and the setting of Select CBM Types ( **AT+CSCB** ), and the value is: <u>0</u> No CBM indications are routed to the TE. 

- 2 New CBMs are routed directly to the TE using unsolicited result code: **+CBM: <length><CR><LF><pdu>** (PDU mode); or **+CBM: <sn>,<mid>,<dcs>,<page>,<pages><CR><LF><data>** (text mode) 

- **<ds>** Integer type. <u>0</u> No SMS-STATUS-REPORTs are routed to the TE. 

- 1       SMS-STATUS-REPORTs are routed to the TE using unsolicited result code: **+CDS: <length><CR><LF><pdu>** (PDU mode) **+CDS: <fo>,<mr>,[<ra>],[<tora>],<scts>,<dt>,<st>** (text mode) 

- 2      If SMS-STATUS-REPORT is stored into ME/TA, indication of the memory location is routed to the TE using unsolicited result code: **+CDSI: <mem>,<index>** 

- **<bfr>** Integer type. <u>0</u> TA buffer of unsolicited result codes defined within this command is flushed to the TE when **<mode>** 1 or 2 is entered ( **OK** response should be given before flushing the codes). 

- 1 TA buffer of unsolicited result codes defined within this command is cleared when **<mode>** 1 or 2 is entered. 

- **<err>** Error codes. For more details, see **_Chapter 12.5_** . 

|**NOTE**||
|---|---|
|Unsolicited result code:||
|**+CMTI:** **<mem>,<index>**<br>|Indicates that new message has been received|
|**+CMT: [<alpha>],<length><CR><LF><pdu>**|Short message is outputted directly|
|**+CBM: <length><CR><LF><pdu>**|Cell broadcast message is outputted directly|



###### **Example** 

|**AT+CMGF=1**//Set SMS message format as text mode.|
|---|
|**OK**|
|**AT+CSCS="GSM"**//Set character set used by the TE as GSM.|
|**OK**|
|**AT+CNMI=1,2,0,1,0**//Set SMS-DELIVERs are routed directly to the TE.|
|**OK**|
|**+CMT: "+8615021012496",,"13/03/18,17:07:21+32",145,4,0,0,"+8613800551500",145,28**|
|**This is a test from Quectel.**//Short message is outputted when an SMS is incoming.|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    136 / 207** 

**LTE Standard Module Series** 



##### **8.14. AT+CSCB  Select Cell Broadcast Message Types** 

The Write Command selects which types of CBMs are to be received by the ME. 

|**AT+CSCB  Select Cell Broadcast**|**Message Types**|
|---|---|
|Test Command|Response|
|**AT+CSCB=?**|**+CSCB:** **(**list of supported**<mode>**s**)**|
||**OK**|
|Read Command|Response|
|**AT+CSCB?**|**+CSCB:** **<mode>,<mids>,<dcss>**|
||**OK**|
|Write Command<br>**AT+CSCB=<mode>[,mids>[,<dcss>]]**|Response<br>**OK**|
||If there is any error related to ME functionality:<br>**+CMS ERROR: <err>**|
|Maximum Response Time|300 ms|
|Characteristics|The command takes effect immediately.<br>The configurations are saved automatically.|
|Reference<br>3GPP TS 27.005||



###### **NOTE** 

The configuration is stored to NVM. 

###### **Parameter** 

|**<mode>**|Integer type.|
|---|---|
||0<br>Message types specified in**<mids>**and**<dcss>**are accepted|
||1<br>Message types specified in**<mids>**and**<dcss>**are not accepted|
|**<mids>**|String type. All different possible combinations of CBM message identifiers (refer to**<mid>**)|
||(default is empty string), e.g. "0,1,5,320-478,922".|
|**<dcss>**|String type. All different possible combinations of CBM data coding schemes (refer to|
||**<dcs>**) (default is empty string), e.g. "0-3,5".|
|**<err>**|Error codes. For more details, see**_Chapter 12.5_**.|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    137 / 207** 

**LTE Standard Module Series** 



##### **8.15. AT+CSDH  Show SMS Text Mode Parameters** 

This Write Command controls whether detailed header information is shown in text mode result codes. 

|**AT+CSDH  Show SMS Text**|**Mode Parameters**|
|---|---|
|Test Command|Response|
|**AT+CSDH=?**|**+CSDH:** **(**list of supported**<show>**s**)**|
||**OK**|
|Read Command|Response|
|**AT+CSDH?**|**+CSDH: <show>**|
||**OK**|
|Write Command<br>**AT+CSDH[=<show>]**|Response<br>**OK**<br>Or<br>**ERROR**|
|Maximum Response Time|300 ms|
|Characteristics|/|
|Reference||
|3GPP TS 27.005||



###### **Parameter** 

**<show>** Integer type. 

- <u>0</u> Do not show header values defined in commands **+CSCA** , **+CSMP** ( **<sca>** , **<tosca>** , 

   - **<fo>** , **<vp>** , **<pid>** , **<dcs>** ) or **<length>** , **<toda>** or **<tooa>** in **+CMT** , **+CMGL** , 

   - **+CMGR** result codes for SMS-DELIVERs and SMS-SUBMITs in text mode. 

   - For SMS-COMMANDs in +CMGR result code, do not show <pid>,<mn>,<da>, <toda>,<length> or<cdata>. 

- 1 Show the values in result codes 

###### **Example** 

**<mark>AT+CSDH=0 OK AT+CMGR=2 +CMGR: "STO UNSENT" ,"",</mark> <This is a test from Quectel>** **<mark>OK</mark>** 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    138 / 207** 

**LTE Standard Module Series** 



**<mark>AT+CSDH=1 OK AT+CMGR=2 +CMGR: "STO UNSENT","",,128,17,0,0,143,"+8613800551500",145,18</mark> <This is a test from Quectel>** **<mark>OK</mark>** 

##### **8.16. AT+CSMP  Set SMS Text Mode Parameters** 

This command sets values for additional parameters needed when a short message is sent to the network or placed in a storage in text mode. 

|**AT+CSMP  Set SMS Text Mode P**|**arameters**|
|---|---|
|Test Command<br>**AT+CSMP=?**|Response<br>**OK**|
|Read Command<br>**AT+CSMP?**|Response<br>**+CSMP: <fo>,<vp>,<pid>,<dcs>**|
||**OK**|
|Write Command|Response|
|**AT+CSMP=<fo>[,<vp>[,<pid>[,<dcs>]**<br>**]]**|TA selects values for additional parameters needed when SM<br>is sent to the network or placed in a storage when text mode<br>is selected (**AT+CMGF=1**). It is possible to set the validity<br>period starting from when the SM is received by the SMSC<br>(**<vp>**ranges from 0 to 255) or define the absolute time of<br>validity period termination (**<vp>**is a string).<br>**OK**|
|Maximum Response Time|300 ms|
|Characteristics|/|
|Reference<br>3GPP TS 27.005||



###### **Parameter** 

**<fo>** Depending on the command or result code: first octet of 3GPP TS 23.040 SMS-DELIVER, SMS-SUBMIT (17 by default), SMS-STATUS-REPORT, SMS-COMMAND in integer format. If a valid value has been entered once, parameter can be omitted. **<vp>** Validity <u>period. Depending on SMS-SUBMIT</u> **<fo>** setting: 3GPP TS 23.040 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    139 / 207** 

**LTE Standard Module Series** 



TP-Validity-Period either in integer format or in time-string format (refer to **<dt>** ). 

**<pid>** Protocol identifier. 3GPP TS 23.040 TP-Protocol-Identifier in integer format (0 by default). 

- **<dcs>** Data coding scheme. Depending on the command or result code: 3GPP TS 23.038 SMS Data Coding Scheme (0 by default), or Cell Broadcast Data Coding Scheme in integer format. 

##### **8.17. AT+QCMGS  Send Concatenated Messages** 

This command sends concatenated massages. Different from **AT+CMGS** , when sending a concatenated message via this command, each segment of the concatenated message must be identified by the additional parameters: **<uid>** , **<msg_seg>** and **<msg_total>** . When sending all segments of the message one by one, **AT+QCMGS** must be executed multiple times (equal to **<msg_total>** ) for each segment. This command is only used in text mode ( **AT+CMGF** =1). 

###### **AT+QCMGS Send Concatenated Messages** 

|Test Command|Response|
|---|---|
|**AT+QCMGS=?**|**OK**|
|Write Command|Response|
|If text mode (**+CMGF=1**):|If in text mode (**AT+CMGF**=1) and sent successfully:|
|**AT+QCMGS=<da>[,<toda>],<uid>,<m**|**+QCMGS:** **<mr>**|
|**sg_seg>,<msg_total>**<CR>||
|**text is entered**<br>**<Ctrl+Z/ESC>**|**OK**<br>Or<br>**ERROR**|
||If there is any error related to ME functionality:<br>**+CMS ERROR: <err>**|
|Maximum Response Time|120 s, determined by network.|
|Characteristics|This command takes effect immediately.<br>The configuration is not saved.|



###### **Parameter** 

|**<uid>**|Integer type. Message identification in the user data header (UDH). Range:|
|---|---|
||0–255. This parameter is defined and inputted by the user. All segments of a<br>same concatenated message must have the same**<uid>**. Different concatenated<br>messages should have different**<uid>**.|
|**<msg_seg>**|Integer type. Sequence number of a concatenated message. Range: 0–7.<br> **<msg_seg>**=0 means: ignore the value and regard it as a non-concatenated|
||message.|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    140 / 207** 

**LTE Standard Module Series** 



**<msg_total>** Integer type. The total number of the segments of one concatenated message. Range: 0–7. **<msg_total>** =0 or 1 means: ignore the value and regard it as a non-concatenated message. **<da>** Refer to **AT+CMGS** . **<toda>** Refer to **AT+CMGS** . **<mr>** Refer to **AT+CMGS** . **<err>** Error codes. For more details, see **_Chapter 12.5_** . 

###### **NOTE** 

1. For concatenated messages, the maximum length will be reduced by the length of the user data header (UDH). _3GPP TS 23.040_ defines two kinds of UDH length: 6 bytes and 7 bytes, so the two kinds of **<uid>** are 8-bit (6 bytes) and 16-bit (7 bytes). **AT+QCMGS** uses 8-bit **<uid>** . 

   - ⚫ In case of GSM 7-bit default alphabet data coding scheme, the maximum length of each segment of a concatenated message is (140 octets - 6 ) × 8/7=153 characters. 

   - ⚫ In case of 16-bit UCS2 data coding scheme, the maximum length of each segment is (140-6)/2=67 characters. 

   - ⚫ In case of 8-bit data coding scheme, the maximum length of each segment is 140-6=134 characters. 

2. **<mr>** Message-Reference field gives an integer representation of a reference number of the SMS-SUBMIT or SMS-COMMAND submitted to the SC by the MS, and it is used to confirm whether the SMS-DELIVER has been received from SC duplicate or not. **<uid>** The field of UDH. It is message identification of the concatenated SMS, which is 

different from **<mr>** . Each segment in a concatenated message should have the same **<uid>** , but **<mr>** must be incremented for each segment of a concatenated message. 

3. **AT+QCMGS** does not support sending messages in PDU mode ( **AT+CMGF=0** ). 

###### **Example** 

**<mark>AT+CMGF=1</mark>** <mark>//Set SMS message format as text mode.</mark> **<mark>OK AT+CSCS="GSM"</mark>** <mark>//Set character set used by the TE as GSM.</mark> **<mark>OK</mark>** 

**<mark>AT+QCMGS="15056913384",120,1,2</mark>** <mark>//Input 120 for</mark> **<mark><uid></mark>** <mark>, and send the first segment of the</mark> **<mark>>ABCD<Ctrl-Z></mark>** <mark>concatenated SMS.</mark> **<mark>+QCMGS: 190</mark>** 

**<mark>OK AT+QCMGS="15056913384",120,2,2</mark>** <mark>//Send the second segment of the concatenated SMS.</mark> **<mark>>EFGH<Ctrl-Z> +QCMGS: 191 OK</mark>** 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    141 / 207** 

**LTE Standard Module Series** 



##### **8.18. AT+QCMGR Read Concatenated Messages** 

The function of this command is similar to **AT+CMGR** , except that the message to be read is a segment of concatenated messages, and parameters **<uid>** , **<msg_seg>** and **<msg_total>** would be shown in the result. Several segments should be concatenated to a whole concatenated message according to these three parameters. Similar to **AT+QCMGS** , **AT+QCMGR** is only used in text mode ( **AT+CMGF=1** ). 

###### **AT+QCMGR  Read Concatenated Messages** 

|Test Command|Response|
|---|---|
|**AT+QCMGR=?**|**OK**|
|Write Command<br>**AT+QCMGR=<index>**|Response<br>in text mode (**AT+CMGF=1**) and command is executed<br>successfully:|
||For SMS-DELIVER:<br>**+QCMGR:**<br>**<stat>,<oa>,[<alpha>],<scts>[,<tooa>,<fo>,<pid>,<dcs>,**<br>**<sca>,<tosca>,<length>][,<uid>,<msg_seg>,<msg_total>**<br>**]<CR><LF><data>**|
||**OK**|
||For SMS-SUBMIT:<br>**+QCMGR:**<br>**<stat>,<da>,[<alpha>][,<toda>,<fo>,<pid>,<dcs>,[<vp>],**<br>**<sca>,<tosca>,<length>][,<uid>,<msg_seg>,<msg_total>**<br>**]<CR><LF><data>**|
||**OK**|
||For SMS-STATUS-REPORTs:<br>**+QCMGR:**<br>**<stat>,<fo>,<mr>,[<ra>],[<tora>],<scts>,<dt>,<st>**|
||**OK**|
||For SMS-COMMANDs:<br>**+QCMGR:**<br>**<stat>,<fo>,<ct>[,<pid>,[<mn>],[<da>],[<toda>],<length>**<br>**<CR><LF><cdata>]**<br>**OK**|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    142 / 207** 

**LTE Standard Module Series** 



||Else, If there is any error related to ME functionality:|
|---|---|
||**+CMS ERROR: <err>**|
|Maximum Response Time|Depends on the length of message content.|
|Characteristics|/|



###### **Parameter** 

**<uid>** Integer type. Message identification in the user data header (UDH). Range: 0–65535 (see NOTES). All segments of a same concatenated message have same **<uid>** . Different concatenated messages should have different **<uid>** . **<msg_seg>** Integer type. Sequence number of a concatenated message. Range: 1–7. **<msg_total>** Integer type. The total number of the segments of one concatenated message. Range: 2–7. Other parameters please refer to **AT+CMGR <err>** Error codes. For more details, see **_Chapter 12.5_** . 

###### **NOTE** 

1. The **<uid>** in **AT+QCMGR** is different from the **<uid>** in **AT+QCMGS** . It is possible that UE receives concatenated messages with 8-bit or 16-bit **<uid>** . So its maximal value is 255 with 8-bit and 65535 with 16-bit. 

2. If the message to be read is not a concatenated message, **<uid>** , **<msg_seg>** and **<msg_total>** would not be shown in the result. 

###### **Example** 

**<mark>+CMTI: "SM",3</mark>** <mark>//The first message of a concatenated message comes.</mark> 

**<mark>+CMTI: "SM",4</mark>** <mark>//The second message of a concatenated message comes.</mark> **<mark>AT+QCMGR=3</mark>** <mark>//Read the first segment of the concatenated message.</mark> **<mark>+QCMGR: "REC UNREAD","+8615056913384",,"13/07/30,14:44:37+32",120,1,2 ABCD</mark>** 

###### **<mark>OK</mark>** 

**<mark>AT+QCMGR=4</mark>** <mark>//Read the second segment of the concatenated message.</mark> **<mark>+QCMGR: "REC UNREAD","+8615056913384",,"13/07/30,14:44:37+32",120,2,2 EFGH</mark>** 

**<mark>OK</mark>** 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    143 / 207** 

**LTE Standard Module Series** 



# **9 Packet Domain Commands** 

##### **9.1. AT+CGATT  Attachment or Detachment of PS** 

This Write Command attaches the MT to, or detaches the MT from the Packet Domain service. After the command has been completed, the MT remains in V.25ter command state. If the MT is already in the requested state, the command will be ignored and the **OK** response will be returned. If the requested state cannot be achieved, an **ERROR** or **+CME ERROR** response is returned. 

###### **AT+CGATT  Attachment or Detachment of PS** 

|Test Command|Response|
|---|---|
|**AT+CGATT=?**|**+CGATT: (**list of supported**<state>**s**)**|
||**OK**|
|Read Command|Response|
|**AT+CGATT?**|**+CGATT: <state>**|
||**OK**|
|Write Command<br>**AT+CGATT=<state>**|Response<br>**OK**|
||If there is any error related to ME functionality:<br>**+CME ERROR: <err>**|
|Maximum Response Time|140 s, determined by network.|
|Characteristics|Whether the command takes effect determined by network.<br>The configuration is not saved.|
|Reference||
|3GPP TS 27.007||



###### **Parameter** 

**<state>** Integer type. Indicates the state of PS attachment. 

||0<br>Detached<br>1<br>Attached|
|---|---|
||Other values are reserved and will result in an**ERROR**response to the Write Command.|
|**<err>**|Error codes. For more details, see**_Chapter 12.4_**.|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    144 / 207** 

**LTE Standard Module Series** 



###### **Example** 

|**AT+CGATT=1**|//Attach to PS service.|
|---|---|
|**OK**||
|**AT+CGATT=0**|//Detach from PS service.|
|**OK**||
|**AT+CGATT?**|//Query the current PS service state.|
|**+CGATT: 0**||
|**OK**||



##### **9.2. AT+CGDCONT  Define PDP Context** 

This command specifies PDP context parameters for a specific context **<cid>** . A special form of the Write Command ( **AT+CGDCONT=<cid>** ) causes the values for context **<cid>** to become undefined. It is not allowed to change the definition of an already activated context. 

This Read Command returns the current settings for each defined PDP context. 

###### **AT+CGDCONT  Define PDP Context** 

|Test Command|Response|
|---|---|
|**AT+CGDCONT=?**|**+CGDCONT:** **(**list of supported**<cid>**s**),<PDP_type>,  <A**<br>**PN>,<PDP_addr>,(**list of supported**<data_comp>**s**),(**list o<br>f supported**<head_comp>**s**),(**list of supported**<IPv4_a**<br>**ddr_alloc>**s**),(**list of supported**<request_type>**s**)**|
||**OK**|
|Read Command<br>**AT+CGDCONT?**|Response<br>**+CGDCONT: <cid>,<PDP_type>,<APN>,<PDP_addr>,**<br>**<data_comp>,<head_comp>,<IPv4_addr_alloc>,**<br>**<request_type>**<br>**[…]**|
||**OK**|
|Write Command<br>**AT+CGDCONT=<cid>[,<PDP_type>[,<**<br>**APN>[,<PDP_addr>[,<data_comp>[,<**<br>**head_comp>[,<IPv4_addr_alloc>[,<re**<br>**quest_type>]]]]]]]**|Response<br>**OK**<br>Or<br>**ERROR**|
|Maximum Response Time|300 ms|
|Characteristics|The command takes effect immediately.|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    145 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

The configurations are saved automatically. 

Reference 3GPP TS 27.007 

###### **Parameter** 

**<cid>** Integer type. PDP context identifier. A numeric parameter which specifies a particular PDP context definition. The parameter is local to the TE-MT interface and is used in other PDP context-related commands. The range of permitted values (minimum value = 1) is returned by the test form of the command. **<PDP_type>** String type. Packet data protocol type, a string parameter which specifies the type of packet data protocol. "IP" Internet Protocol (IETF STD 5) "PPP" "IPV6" "IPV4V6" **<APN>** String type. Access point name, a string parameter that is a logical name used to select the GGSN or the external packet data network. If the value is null or omitted, then the subscription value will be requested. **<PDP_addr>** String type. Identifies the MT in the address space applicable to the PDP. If the value is null or omitted, then a value may be provided by the TE during the PDP startup procedure or, failing that, a dynamic address will be requested. The allocated address may be read with **AT+CGPADDR** . **<data_comp>** Integer type. Controls PDP data compression (applicable for SNDCP only) (refer to _3GPP TS 44.065_ ). <u>0</u> Off (Default if value is omitted) 1 On (Manufacturer preferred compression) 2      V.42bis **<head_comp>** Integer type. Controls PDP header compression (refer to _3GPP TS 44.065_ and _3GPP TS 25.323_ ). <u>0</u> Off (Default if value is omitted) 1      On 2      RFC1144 3      RFC2507 4      RFC3095 **<IPv4_addr_alloc>** Integer type. Controls how the MT/TA requests to get the IPv4 address information. 0 IPv4 address allocation through NAS signaling 1 IPv4 address allocated through DHCP **<request_type>** Integer type. Indicate the type of PDP context activation request for the PDP context. 0 PDP context is for new PDP context establishment or for handover from 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    146 / 207** 

**LTE Standard Module Series** 



a non-3GPP access network (how the MT decides whether the PDP context is for new PDP context establishment or for handover is 

implementation specific). 

1 PDP context is for emergency bearer services 

##### **9.3. AT+CGACT  Activate or Deactivate PDP Context** 

This Write Command activates or deactivates the specified PDP context(s). After the command has completed, the MT remains in V.250 command state. If any PDP context is already in the requested state, the state for that context remains unchanged. If the MT is not PS attached when the activation form of the command is executed, the MT first performs a PS attach and then attempts to activate the specified contexts. If no **<cid>** s specify the activation/deactivation form of the command, it will activate or deactivate all defined contexts. 

###### **AT+CGACT  Activate or Deactivate PDP Context** 

|Test Command|Response|
|---|---|
|**AT+CGACT=?**|**+CGACT: (**list of supported**<state>**s**)**|
||**OK**|
|Read Command<br>**AT+CGACT?**|Response<br>**+CGACT: <cid>,<state>**<br>**[+CGACT: <cid>,<state>**<br>**…]**|
||**OK**|
|Write Command<br>**AT+CGACT=<state>,<cid>**|Response<br>**OK**<br>Or<br>**NO CARRIER**|
||If there is any error related to ME functionality:<br>**+CME ERROR: <err>**|
|Maximum Response Time|150 s, determined by network.|
|Characteristics|Whether the command takes effect is determined by network.<br>The configurations are not saved.|
|Reference||
|3GPP TS 27.007||



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    147 / 207** 

**LTE Standard Module Series** 



###### **Parameter** 

|**<state>**|Integer type. Indicates the state of PDP context activation.|
|---|---|
||0<br>Deactivated|
||1<br>Activated|
||Other values are reserved and will result in an**ERROR**response to the Write Command|
|**<cid>**|Integer type. Specifies a particular PDP context definition (see**AT+CGDCONT**).|
|**<err>**|Error codes. For more details, see**_Chapter 12.4_**.|



###### **Example** 

|**AT+CGDCONT=1,"IP","UNINET"**|//Define PDP context.|
|---|---|
|**OK**||
|**AT+CGACT=1,1**|//Activated PDP.|
|**OK**||
|**AT+CGACT=0,1**|//Deactivated the PDP.|
|**OK**||



##### **9.4. AT+CGDATA  Enter Data State** 

This Write Command causes the MT to perform whatever actions that are necessary to establish communication between the TE and the network using one or more packet domain PDP types. This may include performing a PS attach and one or more PDP context activations. Any command following the **AT+CGDATA** in the AT command line will not be processed by the MT. 

If the **<L2P>** value is unacceptable to the MT, the MT shall return an **ERROR** or **+CME ERROR** response. Otherwise, the MT issues the intermediate result code **CONNECT** and enters V.250 online data state. After data transfer is completed, and the layer 2 protocol termination procedure has been completed successfully, the command state is reentered and the MT returns the final result code **OK** . 

###### **AT+CGDATA  Enter Data State** 

|Test Command|Response|
|---|---|
|**AT+CGDATA=?**|**+CGDATA: (**list of supported**<L2P>**s**)**|
||**OK**|
|Write Command|Response|
|**AT+CGDATA=<L2P>[,<cid>[,<cid>[,…**|**CONNECT**|
|**]]]**|Or<br>**ERROR**|
||If there is anyerror related to ME functionality:|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    148 / 207** 

**LTE Standard Module Series** 



||**+CME ERROR: <err>**|
|---|---|
|Maximum Response Time|300 ms|
|Characteristics|Whether the command takes effect is determined by network.<br>The configurations are not saved.|
|Reference||
|3GPP TS 27.007||



###### **Parameter** 

**<L2P>** String type. Indicates the layer 2 protocol to be used between the TE and MT: PPP Point to Point protocol for a PDP such as IP Other values  Not supported and will result in an **ERROR** response to the Execution Command **<cid>** Integer type. Specifies a particular PDP context definition (see **AT+CGDCONT** ). **<err>** Error codes. For more details, see **_Chapter 12.4_** . 

##### **9.5. AT+CGPADDR  Show PDP Address** 

This Write Command returns a list of PDP addresses for the specified context identifiers. If no **<cid>** is specified, the addresses for all defined contexts are returned. 

###### **AT+CGPADDR  Show PDP Address** 

|Test Command|Response|
|---|---|
|**AT+CGPADDR=?**|**+CGPADDR:** **(**list of defined**<cid>**s**)**|
||**OK**|
|Write Command|Response|
|**AT+CGPADDR[=<cid>[,<cid>[,…]]]**|**+CGPADDR: <cid>,<PDP_addr>**<br>**[+CGPADDR: <cid>,<PDP_addr>**<br>**…]**|
||**OK**<br>Or<br>**ERROR**|
|Maximum Response Time|300 ms|
|Characteristics|Whether the command takes effect is determined by network.<br>The configurations are not saved.|
|Reference||
|3GPP TS 27.007||



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    149 / 207** 

**LTE Standard Module Series** 



###### **Parameter** 

|**<cid>**|Integer type. Specifies a particular PDP context definition (see**AT+CGDCONT**).|
|---|---|
|**<PDP_addr>**|String type. Identifies the MT in the address space applicable to the PDP. The address|
|<br>|may be static or dynamic. For a static address, it will be the one set by**AT+CGDCONT**<br>when the context was defined. For a dynamic address it will be the one assigned|
||during the last PDP context activation that used the context definition referred to by|
||**<cid>**.**<PDP_address>**is omitted if none is available.|



###### **Example** 

|**AT+CGDCONT=1,"IP","UNINET"**|//Define PDP context.|
|---|---|
|**OK**||
|**AT+CGACT=1,1**|//Activated PDP.|
|**OK**||
|**AT+CGPADDR=1**|//Show PDP address.|
|**+CGPADDR: 1,"10.76.51.180"**||
|**OK**||



##### **9.6. AT+CGREG  Network Registration Status** 

This command queries the network registration status and controls the presentation of an unsolicited result code **+CGREG: <stat>** when **<n>** =1 and there is a change in the MT’s GPRS network registration status in GERAN/UTRAN, or unsolicited result code **+CGREG: <stat>[,[<lac>],[<ci>],[<AcT>]]** when **<n>** =2 and there is a change of the network cell in GERAN/UTRAN. 

###### **AT+CGREG  Network Registration Status** 

|Test Command|Response|
|---|---|
|**AT+CGREG=?**|**+CGREG:** **(**Range of supported**<n>**s**)**|
||**OK**|
|Read Command<br>**AT+CGREG?**|Response<br>**+CGREG: <n>,<stat>[,<lac>,<ci>[,<AcT>]]**|
||**OK**|
|Write Command<br>**AT+CGREG[=<n>]**|Response<br>**OK**<br>Or<br>**ERROR**|
|Maximum Response Time|300 ms|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    150 / 207** 

**LTE Standard Module Series** 



|Characteristics|The command takes effect immediately.<br>The configuration is saved automatically.|
|---|---|
|Reference||
|3GPP TS 27.007||



###### **Parameter** 

- **<n>** Integer type. Control the presentation of the specified URC. <u>0</u> Disable network registration unsolicited result code 

   - 1 Enable network registration unsolicited result code **+CGREG: <stat>** 

- 2 Enable network registration and location information unsolicited result code **+CGREG: <stat>[,<lac>,<ci>[,<AcT>]]** 

- **<stat>** Integer type. Network registration status. 0 Not registered. MT is not currently searching an operator to register to. The UE is in GMM state GMM-NULL or GMM-DEREGISTERED-INITIATED. The GPRS service is disabled, but the UE is allowed to attach for GPRS if requested by the user. 

   - 1 Registered, home network. The UE is in GMM state GMM-REGISTERED or GMM-ROUTING-AREA-UPDATING-INITIATED INITIATED on the home PLMN. 

- 2 Not registered, but MT is currently trying to attach or searching an operator to register to. UE is in GMM state GMM-DEREGISTERED or GMM-REGISTERED-INITIATED. The GPRS service is enabled, but an allowable PLMN is currently not available. The UE will start a GPRS attach as soon as an allowable PLMN is available. 

- 3 Registration denied. The UE is in GMM state GMM-NULL. The GPRS service is disabled, and the UE is not allowed to attach for GPRS if requested by the user. 

- 4 Unknown 5 Registered, roaming 

- **<lac>** String type. Two-byte location area code in hexadecimal format (e.g. "00C3" equals 195 in decimal). 

- **<ci>** String type. 16 bit (GSM) or 28 bit (UMTS/LTE) cell ID in hexadecimal format. **<AcT>** Integer type. Access technology selected. 0 GSM 2 UTRAN 

   - 3 GSM W/EGPRS 

   - 4 UTRAN W/HSDPA 

- 5 UTRAN W/HSUPA 6 UTRAN W/HSDPA and HSUPA 

- 7 E-UTRAN 

###### **Example** 

**<mark>AT+CGREG=2 OK AT+CGATT=0 OK</mark>** 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    151 / 207** 

**LTE Standard Module Series** 



**<mark>+CGREG: 2 AT+CGATT=1 OK</mark>** 

**<mark>+CGREG: 1,"D504","80428B5",7</mark>** 

##### **9.7. AT+CGEREP  Packet Domain Event Reporting** 

This Write Command enables or disables sending of unsolicited result codes **+CGEV: XXX** from MT to TE in the case of certain events occurring in the Packet Domain MT or the network. **<mode>** controls the processing of unsolicited result codes specified within this command. **<bfr>** controls the effect on buffered codes when **<mode>** =1 or 2. 

###### **AT+CGEREP Packet Domain Event Reporting** 

|Test Command|Response|
|---|---|
|**AT+CGEREP=?**|**+CGEREP: (**list of supported**<mode>**s**),(**list of supported<br>**<bfr>**s**)**|
||**OK**|
|Read Command<br>**AT+CGEREP?**|Response<br>**+CGEREP: <mode>,<bfr>**|
||**OK**|
|Write Command<br>**AT+CGEREP=mode[,<bfr>]**|Response<br>**OK**<br>Or<br>**ERROR**|
|Execution Command<br>**AT+CGEREP**|Response<br>**OK**|
|Maximum Response Time|300 ms|
|Characteristics|The command takes effect immediately.<br>The configurations are saved automatically.|
|Reference||
|3GPP TS 27.007||



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    152 / 207** 

**LTE Standard Module Series** 



###### **Parameter** 

|**<mode>**|Integer type. Controls the processing of unsolicited result codes specified within this<br>command.<br> <br>|
|---|---|
||0<br>Buffer unsolicited result codes in the MT; if MT result code buffer is full, the oldest<br>ones can be discarded. No code is forwarded to the TE.|
||1<br>Discard unsolicited result codes when MT-TE link is reserved (e.g. in on-line data<br>mode), otherwise forward them directly to the TE.|
||2<br>Buffer unsolicited result codes in the MT when MT-TE link is reserved (e.g. in<br>on-line data mode) and flush them to the TE when MT-TE link becomes available.<br>Otherwise forward them directly to the TE.|
|**<bfr>**|Integer type. Controls the effect on buffered codes.<br>0<br>MT buffer of unsolicited result codes defined within this command is cleared when<br>**<mode>**1 or 2 is specified.|
||1<br>MT buffer of unsolicited result codes defined within this command is flushed to the<br>TE when**<mode>**1 or 2 is specified (**OK**response shall be given before flushing<br>the codes).|



###### **NOTE** 

The unsolicited result codes and the corresponding events are defined as follows: 

1. **+CGEV: REJECT <PDP_type>, <PDP_addr>** : A network request for PDP context activation occurred when the MT was unable to report it to the TE with a **+CRING** unsolicited result code and was automatically rejected. 

Note: This event is not applicable for EPS. 

2. **+CGEV: NW REACT <PDP_type>, <PDP_addr>,[<cid>]** : The network has requested a context reactivation. The **<cid>** used to reactivate the context is provided if known to the MT. Note: This event is not applicable for EPS. 

3. **+CGEV: NW DEACT <PDP_type>, <PDP_addr>,[<cid>]** : The network has forced a context deactivation. The **<cid>** used to activate the context is provided if known to the MT. 

4. **+CGEV: ME DEACT <PDP_type>, <PDP_addr>,[<cid>]** : The mobile equipment has forced a context deactivation. The **<cid>** used to activate the context is provided if known to the MT. 

5. **+CGEV: NW DETACH** : The network has forced a Packet Domain detach. This implies that all active contexts have been deactivated. These are not reported separately. 

6. **+CGEV: ME DETACH** : The mobile equipment has forced a Packet Domain detach. This implies that all active contexts have been deactivated. These are not reported separately. 

7. **+CGEV: NW CLASS <class>** : The network has forced a change of MS class. The highest available class is reported (see **AT+CGCLASS** ). 

8. **+CGEV: ME CLASS <class>** : The mobile equipment has forced a change of MS class. The highest available class is reported (see **AT+CGCLASS** ). 

9. **+CGEV: PDN ACT <cid>** : Activated a context. The context represents a PDN connection in LTE or a Primary PDP context in GSM/UMTS. 

10. **+CGEV: PDN DEACT <cid>** : Deactivated a context. The context represents a PDN connection in LTE or a Primary PDP context in GSM/UMTS. 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    153 / 207** 

**LTE Standard Module Series** 



###### **Example** 

**<mark>AT+CGEREP=? +CGEREP: (0-2),(0,1) OK AT+CGEREP? +CGEREP: 0,0 OK</mark>** 

##### **9.8. AT+CEREG  EPS Network Registration Status** 

This command queries the network registration status and controls the presentation of an unsolicited result code **+CEREG: <stat>** when **<n>** =1 and there is a change in the MT’s EPS network registration status in E-UTRAN, or unsolicited result code **+CEREG: <stat>[,[<tac>],[<ci>],[<AcT>]]** when **<n>** =2 and there is a change of the network cell in E-UTRAN. 

###### **AT+CEREG  EPS Network Registration Status** 

|Test Command|Response|
|---|---|
|**AT+CEREG=?**|**+CEREG:** **(**list of supported**<n>**s**)**|
||**OK**|
|Read Command|Response|
|**AT+CEREG?**|**+CEREG: <n>,<stat>[,<tac>,<ci>[,<AcT>]]**|
||**OK**|
|Write Command<br>**AT+CEREG[=<n>]**|Response<br>**OK**<br>Or<br>**ERROR**|
|Maximum Response Time|300 ms|
|Characteristics||
|Reference||
|3GPP TS 27.007||



###### **Parameter** 

**<n>** Integer type. Controls the presentation of an unsolicited result code **+CEREG: <stat>** . <u>0</u> Disable network registration unsolicited result code 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    154 / 207** 

**LTE Standard Module Series** 



- 1 Enable network registration unsolicited result code **+CEREG: <stat>** 

- 2 Enable network registration and location information unsolicited result code **+CEREG: <stat>[,<tac>,<ci>[,<AcT>]]** 

**<stat>** Integer type. 

   - 0 Not registered. MT is not currently searching an operator to register to 

   - 1 Registered, home network 

   - 2 Not registered, but MT is currently trying to attach or searching an operator to register to 

   - 3 Registration denied 

   - 4 Unknown 

   - 5 Registered, roaming 

- **<tac>** String type. Two-byte tracking area code in hexadecimal format. 

- **<ci>** String type. 28-bit E-UTRAN cell ID in hexadecimal format. 

**<AcT>** Integer type. Access technology selected. 

- 0 GSM 

- 2 UTRAN 

- 3 GSM W/EGPRS 

- 4 UTRAN W/HSDPA 

- 5 UTRAN W/HSUPA 

- 6 UTRAN W/HSDPA and HSUPA 

- 7 E-UTRAN 

##### **9.9. AT+QGDCNT  Packet Data Counter** 

This command allows the application to check how much bytes are sent to or received by the module. 

|**AT+QGDCNT  Packet Dat**|**a Counter**|
|---|---|
|Test Command|Response|
|**AT+QGDCNT=?**|**+QGDCNT: (**list of supported**<op>**s**)**|
||**OK**|
|Read Command|Response|
|**AT+QGDCNT?**|**+QGDCNT: <bytes_sent>,<bytes_recv>**|
||**OK**|
|Write Command<br>**AT+QGDCNT=<op>**|Response<br>**OK**<br>Or<br>**ERROR**|
||If there is any error related to ME functionality:<br>**+CME ERROR: <err>**|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    155 / 207** 

**LTE Standard Module Series** 



|Maximum Response Time|300 ms|
|---|---|
|Characteristics|Whether the command takes effect is determined by network.<br>The configuration is not saved.|



###### **Parameter** 

|**<op>**|Integer type. The operation about data counter.|
|---|---|
||0<br>Reset the data counter|
||1<br>Save the results of data counter to NVM<br>If results need to be automatically saved, refer to**AT+QAUGDCNT**.|
|**<bytes_sent>**|Integer type. The amount of sent bytes.|
|**<bytes_recv>**|Integer type. The amount of received bytes.|
|**<err>**|Error codes. For more details,see**_Chapter 12.4_**.|



###### **NOTE** 

When the MT is powered on, **<bytes_sent>** and **<bytes_recv>** are loaded from results of data counter in NVM. The default result in NVM is 0. 

###### **Example** 

**<mark>AT+QGDCNT=?</mark>** <mark>//Test command.</mark> **<mark>+QGDCNT: (0,1) OK AT+QGDCNT?</mark>** <mark>//Query the current bytes sent and received.</mark> **<mark>+QGDCNT: 3832,4618 OK AT+QGDCNT=1</mark>** <mark>//Save the results to NVM.</mark> **<mark>OK AT+QGDCNT=0</mark>** <mark>//Reset the data counter.</mark> **<mark>OK</mark>** 

##### **9.10. AT+QAUGDCNT  Auto Save Packet Data Counter** 

This command allows **AT+QGDCNT** to save results to NVM automatically. 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    156 / 207** 

**LTE Standard Module Series** 



|**AT+QAUGDCNT  Auto Save**|**Packet Data Counter**|
|---|---|
|Test Command|Response|
|**AT+QAUGDCNT=?**|**+QAUGDCNT: (**list of supported**<value>**s**)**|
||**OK**|
|Read Command|Response|
|**AT+QAUGDCNT?**|**+QAUGDCNT: <value>**|
||**OK**|
|Write Command<br>**AT+QAUGDCNT=<value>**|Response<br>**OK**<br>Or<br>**ERROR**|
||If there is any error related to ME functionality:<br>**+CME ERROR: <err>**|
|Maximum Response Time|300 ms|
|Characteristics|Whether the command takes effect is determined by network.<br>The configuration is not saved.|



###### **Parameter** 

**<value>** Integer type. This parameter is the time-interval for **AT+QGDCNT** to save results to NVM automatically. Range: 0, 30–65535. Default: 0. Unit: second. If it is set to 0, auto-save feature is disabled. **<err>** Error code. For more details, see **_Chapter 12.4_** . 

###### **Example** 

**<mark>AT+QAUGDCNT=?</mark>** <mark>//Test command.</mark> **<mark>+QAUGDCNT: (0,30-65535) OK AT+QGDCNT=35</mark>** <mark>//Set</mark> **<mark><value></mark>** <mark>to 35.</mark> **<mark>OK AT+QAUGDCNT?</mark>** <mark>//Query the interval of auto-save.</mark> **<mark>+QAUGDCNT: 35 OK</mark>** 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    157 / 207** 

**LTE Standard Module Series** 



##### **9.11. AT+QNETDEVSTATUS  Query USBNet Device Status** 

This command can query USBNet device status. 

|**AT+QNETDEVSTATUS**|**Query USBNet Device Status**|
|---|---|
|Test Command|Response|
|**AT+QNETDEVSTATUS=?**|**+QNETDEVSTATUS: (**list of supported**<on_off>**s**)**|
||**OK**|
|Read Command|Response|
|**AT+QNETDEVSTATUS?**|If an USBNet call exists,**<state>**,**<IP_type>**and**<instance>**will be<br>included.<br>**+QNETDEVSTATUS: <on_off>[,<state>[,<IP_type>[,<instance>]]]**|
||**OK**|
|Write Command|Response|
|**AT+QNETDEVSTATUS=<on_**<br>**off>**|**OK**<br>Or<br>**ERROR**|
|Maximum Response Time|300 ms|
|Characteristics|/|



###### **Parameter** 

|**<on_off>**|Int|eger type.|
|---|---|---|
||0|Disable USBNet device status URC|
||1|Enable USBNet device status URC|
|**<state>**|Int|eger type.|
||0|Enable USBNet device status URC|
||1<br>2|A USBNet call is ready and MCU can get IP addresses by DHCP or QMI<br>A USBNet call is connected|
|**<IP_type>**|Int|eger type.|
||4|IPv4 call|
||6|IPv6 call|
||Ot|her value is invalid.|
|**<instance>**|Int|eger type. USBNet call instance. It is always 1.|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    158 / 207** 

**LTE Standard Module Series** 



###### **Example** 

|**AT+QNETDEVSTATUS=?**<br>**+QNETDEVSTATUS: (0,1)**|//Test command.|
|---|---|
|**OK**||
|**AT+QNETDEVSTATUS?**<br>**+QNETDEVSTATUS: 0**|//Query command.|
|**OK**||
|**AT+QNETDEVSTATUS=1**|//Enable USBNet device status URC.|
|**OK**||
|**AT+QNETDEVSTATUS?**<br>**+QNETDEVSTATUS: 1**|//Query command.|
|**OK**||
|**AT$QCRMCALL=1,1,1,2,1**<br>**$QCRMCALL: 1,4**|//Start an IPv4 USBNet call.|
|**OK**||
|**+QNETDEVSTATUS: 1,1,4,1**|//USBNet call is ready URC.|
|**+QNETDEVSTATUS: 1,2,4,1**|//MCU get IP addresses from module.|
|**AT+QNETDEVSTATUS?**<br>**+QNETDEVSTATUS: 1,2,4,1**|//Query the current configuration.|
|**OK**||
|**AT$QCRMCALL=0,1,1,2,1**<br>**OK**|//Stop an IPv4 USBNet call.|
|**+QNETDEVSTATUS: 1,0,4,1**<br>**AT+QNETDEVSTATUS?**<br>**+QNETDEVSTATUS: 1**<br>**OK**|//Module report USBNet call disconnect URC.<br>//Query the current configuration.|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    159 / 207** 

**LTE Standard Module Series** 



##### **9.12. AT+CGCONTRDP  PDP Context Read Dynamic Parameters** 

|**AT+CGCONTRDP  PDP Co**|**ntext Read Dynamic Parameters**|
|---|---|
|Test Command<br>**AT+CGCONTRDP=?**|Response<br>**+CGCONTRDP: (**list of supported**<cid>**s**)**|
||**OK**|
|Write Command<br>**AT+CGCONTRDP[=<cid>]**|Response<br>**+CGCONTRDP: <cid>,<bearer_id>,<APN>[,<local_addr and**<br>**subnet_mask>[,<gw_addr>[,<DNS_prim_addr>[,<DNS_sec_a**<br>**ddr>[,<P-CSCF_prim_addr>[,<P-CSCF_sec_addr>[,<IM_CN_S**<br>**ignalling_Flag>[,<LIPA_indication>[,<IPv4_MTU>[,<WLAN_Of**<br>**fload>[,<Local_Addr_Ind>[,<Non-IP_MTU>[,<Serving_PLMN_**<br>**rate_control_value>]]]]]]]]]]]]]]**|
||**[+CGCONTRDP: <cid>,<bearer_id>,<APN>[,<local_addr and**<br>**subnet_mask>[,<gw_addr>[,<DNS_prim_addr>[,<DNS_sec_a**<br>**ddr>[,<P-CSCF_prim_addr>[,<P-CSCF_sec_addr>[,<IM_CN_S**<br>**ignalling_Flag>[,<LIPA_indication>[,<IPv4_MTU>[,<WLAN_Of**<br>**fload>[,<Local_Addr_Ind>[,<Non-IP_MTU>[,<Serving_PLMN_**<br>**rate_control_value>]]]]]]]]]]]]]**|
||**[...]**|
||**OK**<br>Or<br>**ERROR**|
|Maximum Response Time|300 ms|
|Characteristics|/|



###### **Parameter** 

|**<cid>**|Integer type. Specifies a particular non secondary PDP context|
|---|---|
||definition. The parameter is local to the TE-MT interface and is used in<br>other PDP context-related command.|
|**<bearer_id>**|Integer type. Identifies the bearer, i.e. the EPS bearer in EPS and the<br>NSAPI in UMTS/GPRS.|
||1<br>A RmNet call is ready and MCU can get IP addresses by DHCP or|
||QMI|
||2<br>A RmNet call is connected|
|**<APN>**|Stringtype. A logical name that was used to select the GGSN or the|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    160 / 207** 

**LTE Standard Module Series** 



||external packet data network.|
|---|---|
|**<local_addr and**<br>**subnet_mask>**|String type. Shows the IP address and subnet mask of the MT. The string<br>is given as dot-separated numeric (0–255) parameters on the form:<br>"a1.a2.a3.a4.m1.m2.m3.m4" for IPv4 or<br>"a1.a2.a3.a4.a5.a6.a7.a8.a9.a10.a11.a12.a13.a14.a15.a16.m1.m2.m3.<br>m4.m5.m6.m7.m8.m9.m10.m11.m12.m13.m14.m15.m16" for IPv6.|
|**<gw_addr>**|String type. Shows the Gateway Address of the MT. The string is given<br>as dot-separated numeric (0–255) parameters.|
|**<DNS_prim_addr>**|String type. Shows the IP address of the primary DNS server.|
|**<DNS_sec_addr>**|String type. Shows the IP address of the secondary DNS server.|
|**<P_CSCF_prim_addr>**|String type. Shows the IP address of the primary P-CSCF server.|
|**<P_CSCF_sec_addr>**|String type. Shows the IP address of the secondary P-CSCF server.|
|**<IM_CN_Signalling_Flag>**|Integer type. Shows whether the PDP context is for IM CN<br>subsystem-related signalling only or not.<br>0<br>PDP context is not for IM CN subsystem-related signalling only<br>1<br>PDP context is for IM CN subsystem-related signalling only|
|**<LIPA_indication>**|Integer type. Indicates that the PDP context provides connectivity using<br>a LIPA PDN connection. This parameter cannot be set by the TE.<br>0<br>Indication not received that the PDP context provides connectivity<br>using a LIPA PDN connection<br>1<br>Indication received that the PDP context provides connectivity using<br>a LIPA PDN connection|
|**<IPv4_MTU>**|Integer type. Shows the IPv4 MTU size in octets.|
|**<WLAN_Offload>**|Integer type. Indicates whether traffic can be offloaded using the<br>specified PDN connection via a WLAN or not. This refers to bits 1 and 2<br>of the WLAN offload acceptability IE as specified in_3GPP TS 24.008 [8]_<br>_subclause 10.5.6.20_.<br>0<br>Offloading the traffic of the PDN connection via a WLAN when in S1<br>mode or when in Iu mode is not acceptable.|
||1<br>Offloading the traffic of the PDN connection via a WLAN when in S1<br>mode is acceptable, but not acceptable in Iu mode.<br>2<br>Offloading the traffic of the PDN connection via a WLAN when in Iu<br>mode is acceptable, but not acceptable in S1 mode.<br>3<br>Offloading the traffic of the PDN connection via a WLAN when in S1<br>mode or when in Iu mode is acceptable.|
|**<Local_Addr_Ind>**|Integer type. Indicates whether or not the MS and the network support<br>local IP address in TFTs (see_3GPP TS 24.301 [83]_and_3GPP TS 24.008_<br>_[8] subclause 10.5.6.3_)<br>0<br>Indicates that the MS or the network or both do not support local IP<br>address in TFTs|
||1<br>Indicates that the MS and the network support local IP address in<br>TFTs|
|**<Non-IP_MTU>**|Integer type. Shows the Non-IP MTU size in octets.|
|**<Serving_PLMN_rate_cont**|Integer type. Indicates the maximum number of uplink messages the UE|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    161 / 207** 

**LTE Standard Module Series** 



**rol_value>** 

is allowed to send in a 6-minute interval. This refers to octet 3 to 4 of the Serving PLMN rate control IE as specified in _3GPP TS 24.301 [8] subclause 9.9.4.28_ . 

##### **9.13. AT+QNETDEVCTL  Make/Disconnect PDP Call** 

This command makes or disconnects a PDP call. 

|**AT+QNETDEVCTL  Make/Disconnect**|**PDP Call**|
|---|---|
|Test Command|Response|
|**AT+QNETDEVCTL=?**|**+QNETDEVCTL: (**list of supported**<op>**s**),(**list of su<br>pported**<contextID>),(**list of supported**<URC_enabl**<br>**e>**s**),(**list of supported**<state>**s**)**|
||**OK**|
|Read Command<br>**AT+QNETDEVCTL?**|Response<br>**+QNETDEVCTL: <op>,<contextID>,<URC_enable>,<**<br>**state>**|
||**OK**|
|Write Command|Response|
|Make a call through a specified PDP context<br>**AT+QNETDEVCTL=<op>,<contextID>[,<U**<br>**RC_enable>]**|**OK**<br>Or<br>**ERROR**|
|Maximum Response Time|300 ms|
|Characteristics|The command takes effect immediately.<br>The configurations are saved automatically.|



###### **Parameter** 

|**<op>**|Integer type. Make or disconnect a call.|
|---|---|
||0<br>Disconnect a call<br>1<br>Make a call|
||3<br>Make a call and automatically remake a call after disconnecting from the network,<br>and automatically make a call after startup|
|**<contextID>**|Integer type. PDP context ID. Range: 1–15.|
|**<URC_enable>**|Integer type. Whether to enable URC that reports the call status.<br>0<br>Disable|
||1<br>Enable|
|**<state>**|Integer type. Call status.|
||0<br>Failed|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    162 / 207** 

**LTE Standard Module Series** 

1 Successful 



###### **NOTE** 

You can configure **<op>** as 0/1 to disable automatically making a call after startup as needed. 

###### **Example** 

**<mark>AT+QNETDEVCTL=3,1,1</mark>** <mark>//Configure</mark> **<mark><contextID></mark>** <mark>=1 to make a call.</mark> **<mark>OK</mark>** 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    163 / 207** 

**LTE Standard Module Series** 



# **10 Hardware Related Commands** 

##### **10.1. AT+QPOWD Power off** 

This command shuts down the module. The UE returns **OK** immediately when the command is executed. Then the UE deactivates the network. After it is completed, the UE outputs **POWERED DOWN** and enters into the shutdown state. The maximum time for unregistering network is 60 s. The UE is not allowed to turn off the power before the module’s STATUS pin is set low or the URC **POWERED DOWN** is outputted to avoid data loss. 

|**AT+QPOWD  Power off**||
|---|---|
|Test Command<br>**AT+QPOWD=?**|Response<br>**+QPOWD: (**list of supported**<n>**s**)**|
||**OK**|
|Execution Command|Response|
|**AT+QPOWD[=<n>]**|**OK**|
||**POWERED DOWN**|
|Maximum Response Time|300 ms|
|Characteristics|/|



###### **Parameter** 

- **<n>** Integer type. Power down the module. 

   - 0 Immediately power down 

   - <u>1</u> Normal power down 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    164 / 207** 

**LTE Standard Module Series** 



##### **10.2. AT+CCLK  Clock** 

This command sets and queries the real time clock (RTC) of the module. The current setting is retained until the module is totally disconnected from power. 

###### **AT+CCLK  Clock** 

|Test Command|Response|
|---|---|
|**AT+CCLK=?**|**OK**|
|Read Command|Response|
|**AT+CCLK?**|**+CCLK: <time>**|
||**OK**|
|Write Command|Response|
|**AT+CCLK=<time>**|**OK**|
||If there is any error related to ME functionality:<br>**+CME ERROR: <err>**|
|Maximum Response Time|300 ms|
|Characteristics|The command takes effect immediately.<br>The configuration is not saved.|
|Reference||
|3GPP TS 27.007||



###### **Parameter** 

**<time>** String type. The format is "yy/MM/dd,hh:mm:ss±zz", indicating year (last two digits), month, day, hour, minutes, seconds and time zone (indicates the difference, expressed in quarters of an hour, between the local time and GMT; range: -48 to +56). E.g. May 6<sup>th</sup> , 1994, 22:10:00 GMT+2 hours equals to "94/05/06,22:10:00+08". 

**<err>** Error codes. For more details, see **_Chapter 12.4_** . 

###### **Example** 

**<mark>AT+CCLK?</mark>** <mark>//Query the local time.</mark> **<mark>+CCLK: "08/01/04,00:19:43+00" OK</mark>** 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    165 / 207** 

**LTE Standard Module Series** 



##### **10.3. AT+CBC  Battery Charge** 

This command returns battery charge status **<bcs>** and battery charge level **<bcl>** of the MT. 

|**AT+CBC  Battery Charge**||
|---|---|
|Test Command|Response|
|**AT+CBC=?**|**+CBC:** **(**list of supported**<bcs>**s**),(**list of supported<br>**<bcl>**s**),<voltage>**|
||**OK**|
|Execution Command<br>**AT+CBC**|Response<br>**+CBC: <bcs>,<bcl>,<voltage>**|
||**OK**|
||If there is any error related to ME functionality:<br>**+CME ERROR: <err>**|
|Maximum Response Time|300 ms|
|Characteristics|/|
|Reference||
|3GPP TS 27.007||



###### **Parameter** 

|**<bcs>**|Integer type.|Battery charge status.|
|---|---|---|
||0|ME is not charging|
||1|ME is charging|
||2|Charging has been finished|
|**<bcl>**|Integer type.|Battery charge level.|
||0–100|Remaining capacity of battery.|
|**<voltage>**|Battery volta|ge (Mv).|
|**<err>**|Error codes.|For more details, see**_Chapter 12.4_**.|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    166 / 207** 

**LTE Standard Module Series** 



##### **10.4. AT+QADC  Read ADC Value** 

This command reads the voltage value of ADC channel. 

|**AT+QADC  Read ADC Value**||
|---|---|
|Test Command|Response|
|**AT+QADC=?**|**+QADC: (**list of supported**<port>**s**)**|
||**OK**|
|Read Command|Response|
|**AT+QADC=<port>**|**+QADC: <status>,<value>**|
||**OK**|
|Maximum Response Time|300 ms|
|Characteristics|/|



###### **Parameter** 

**<port>** Integer type. Channel number of the ADC. 0    ADC Channel 0 1    ADC Channel 1 **<status>** Integer type. Whether the ADC value is read successfully. 0    Fail 1    Success **<value>** The voltage of specified ADC channel. Unit: mV. 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    167 / 207** 

**LTE Standard Module Series** 



##### **10.5. AT+QSCLK  Enable/Disable Low Power Mode** 

This command enables or disables low power mode. When low power mode is enabled, and both DTR and WAKEUP_IN are pulled up, the module directly enters into sleep mode. If low power mode is enabled, but both DTR and WAKEUP_IN are pulled down, only after the DTR and the WAKEUP_IN are pulled up, can the module enter into low power mode. 

###### **AT+QSCLK  Enable/Disable Low Power Mode** 

|Test Command|Response|
|---|---|
|**AT+QSCLK=?**|**+QSCLK: (**list of supported**<n>**s**)**|
||**OK**|
|Read Command<br>**AT+QSCLK?**|Response<br>**+QSCLK:** **<n>**|
||**OK**|
|Write Command<br>**AT+QSCLK=<n>**|Response<br>**OK**|
|Maximum Response Time|300 ms|
|Characteristics|The command takes effect immediately<br>The configuration is not saved|
|Reference||
|Quectel||



###### **Parameter** 

**<n>** Integer type. Disable or enable low power mode. <u>0</u> Disable 1 Enable. It is controlled by DTR pin and WAKEUP_IN pin. 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    168 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

# **11 Other Related Commands** 

##### **11.1. GNSS Related AT Commands (Not Supported Currently)** 

Please refer to _Quectel_EG800Q&EG91xQ_Series_GNSS_Application_Note_ for details of GNSS function. 

**Table 4: GNSS Related AT Commands** 

|**Commands**|**Description**|
|---|---|
|**AT+QGPSCFG**|GNSS configurations|
|**AT+QGPSDEL**|Deletes assistance data|
|**AT+QGPS**|Turns on GNSS|
|**AT+QGPSEND**|Turns off GNSS|
|**AT+QGPSLOC**|Acquires positioning information|
|**AT+QGPSGNMEA**|Acquires NMEA sentences|
|**AT+QGPSXTRA**|Enables gpsOneXTRA assistance function|
|**AT+QGPSXTRATIME**|Injects gpsOneXTRA time|
|**AT+QGPSXTRADATA**|Injects gpsOneXTRA data file|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    169 / 207** 

**LTE Standard Module Series** 



##### **11.2. DFOTA Related AT Command** 

See _Quectel_EG800Q&EG91xQ_Series_DFOTA_Upgrade_Guide_ for details of DFOTA function. 

###### **Table 5: DFOTA Related AT Command** 

|**Command**|**Description**|
|---|---|
|**AT+QFOTADL**|Upgrades firmware via DFOTA|



##### **11.3. FTP(S) Related AT Commands** 

See _Quectel_EG800Q&EG91xQ_Series_FTP(S)_Application_Note_ for details of FTP(S) function. 

**Table 6: FTP(S) Related AT Commands** 

|**Commands**|**Description**|
|---|---|
|**AT+QFTPCFG**|Configures parameters for FTP(S) server|
|**AT+QFTPOPEN**|Logins to FTP(S) server|
|**AT+QFTPCWD**|Configures the current directory on FTP(S) server|
|**AT+QFTPPWD**|Gets the current directory on FTP(S) server|
|**AT+QFTPPUT**|Uploads a file to FTP(S) server|
|**AT+QFTPGET**|Downloads a file from FTP(S) server|
|**AT+QFTPSIZE**|Gets the file size on FTP(S) server|
|**AT+QFTPDEL**|Deletes a file on FTP(S) server|
|**AT+QFTPMKDIR**|Creates a folder on FTP(S) server|
|**AT+QFTPRMDIR**|Deletes a folder on FTP(S) server|
|**AT+QFTPLIST**|Lists content of a directory on FTP(S) server|
|**AT+QFTPNLIST**|Lists file names of a directory on FTP(S) server|
|**AT+QFTPMLSD**|Lists standardized file and directory information|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    170 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

|**AT+QFTPMDTM**|Gets the file modification time on FTP(S) server|
|---|---|
|**AT+QFTPRENAME**|Renames a file or folder on FTP(S) server|
|**AT+QFTPLEN**|Gets the length of transferred data|
|**AT+QFTPSTAT**|Gets the status of FTP(S) server|
|**AT+QFTPCLOSE**|Logs out from FTP(S) server|



##### **11.4. HTTP(S) Related AT Commands** 

Please refer to _Quectel_EG800Q&EG91xQ_Series_HTTP(S)_Application_Note_ for details of HTTP(S) function. 

**Table 7: HTTP(S) Related AT Commands** 

|**Commands**|**Description**|
|---|---|
|**AT+QHTTPCFG**|Configures parameters for HTTP(S) server|
|**AT+QHTTPURL**|Sets URL of HTTP(S) server|
|**AT+QHTTPGET**|Sends GET request to HTTP(S) server|
|**AT+QHTTPGETEX**|Sends range GET request to HTTP(S) server|
|**AT+QHTTPPOST**|Sends POST request to HTTP(S) server via UART/USB|
|**AT+QHTTPPOSTFILE**|Sends POST request to HTTP(S) server via file|
|**AT+QHTTPREAD**|Reads response from HTTP(S) server via UART/USB|
|**AT+QHTTPREADFILE**|Reads response from HTTP(S) server via file|
|**AT+QHTTPSTOP**|Cancels HTTP(S) request|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    171 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

##### **11.5. TCP(IP) Related AT Commands** 

Please refer to _Quectel_EG800Q&EG91xQ_Series_TCP(IP)_Application_Note_ for details of TCP(IP) function. 

**Table 8: TCP(IP) Related AT Commands** 

|**Commands**|**Description**|
|---|---|
|**AT+QICSGP**|Configures parameters of a TCP/IP context|
|**AT+QIACT**|Activates a PDP context|
|**AT+QIDEACT**|Deactivates a PDP context|
|**AT+QIOPEN**|Opens a socket service|
|**AT+QICLOSE**|Closes a socket service|
|**AT+QISTATE**|Queries socket service status|
|**AT+QISEND**|Sends data|
|**AT+QIRD**|Retrieves the received TCP/IP data|
|**AT+QISENDEX**|Sends hex string|
|**AT+QISWTMD**|Switches data access modes|
|**AT+QPING**|Pings a remote server|
|**AT+QNTP**|Synchronizes local time with NTP server|
|**AT+QIDNSCFG**|Configures address of DNS server|
|**AT+QIDNSGIP**|Gets IP address by domain name|
|**AT+QICFG**|Configures optional parameters|
|**AT+QISDE**|Controls whether to echo the data for AT+QISEND|
|**AT+QIGETERROR**|Queries the last error code|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    172 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

##### **11.6. SSL Related AT Commands** 

Please refer to _Quectel_EG800Q&EG91xQ_Series_SSL_Application_Note_ for details of SSL function. 

**Table 9: SSL Related AT Commands** 

|**Commands**|**Description**|
|---|---|
|**AT+QSSLCFG**|Configures Parameters of an SSL Context|
|**AT+QSSLOPEN**|Opens an SSL Socket to Connect a Remote Server|
|**AT+QSSLSEND**|Sends Data via SSL Connection|
|**AT+QSSLRECV**|Receives Data via SSL Connection|
|**AT+QSSLCLOSE**|Closes an SSL Connection|
|**AT+QSSLSTATE**|Queries the State of SSL Connection|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    173 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

# **12 Appendix References** 

###### **Table 10: Related Documents** 

|**SN**|**Document Name**|
|---|---|
|[1]|V.25ter|
|[2]|3GPP TS 27.007|
|[3]|3GPP TS 27.005|
|[4]|Quectel_EG800Q&EG91xQ_Series_GNSS_Application_Note|
|[5]|Quectel_EG800Q&EG91xQ_Series_DFOTA_Upgrade_Guide|
|[6]|Quectel_EG800Q&EG91xQ_Series_FTP(S)_Application_Note|
|[7]|Quectel_EG800Q&EG91xQ_Series_HTTP(S)_Application_Note|
|[8]|Quectel_EG800Q&EG91xQ_Series_TCP(IP)_Application_Note|
|[9]|Quectel_EG800Q&EG91xQ_Series_SSL_Application_Note|



###### **Table 11: Terms and Abbreviations** 

|**Abbreviation**|**Description**|
|---|---|
|3GPP|3<sup>rd</sup>Generation Partnership Project|
|ACDB|Audio Calibration Database|
|ACK|Acknowledge Character|
|ACL|Access Control List|
|ADC|Analog-to-Digital Converter|
|AMR|Adaptive Multi-Rate|
|APN|Access Point Name|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    174 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

|ARFCN|Absolute Radio-Frequency Channel Number|
|---|---|
|ASCII|American Standard Code for Information Interchange|
|BCD|Binary-Coded Decimal|
|CBM|Cell Broadcast Message|
|CCH|Control Channel|
|CDMA|Code Division Multiple Access|
|CFU|Call Forwarding Unconditional|
|CLI|Calling Line Identity|
|CLIP|Calling Line Identification Presentation|
|CLIR|Calling Line Identification Restriction|
|COL|Connected Line|
|COLP|Connected Line Identification Presentation|
|COLR|Connected Line Identification Restriction|
|CPT|Communication Production Technology|
|CS|Circuit Switching|
|CSD|Circuit Switch Data|
|DCD|Dynamic Content Delivery|
|DCD|Data Carrier Detection|
|DCE|Data Circuit-terminating Equipment|
|DCH|Data Channel|
|DCS|Data Coding Scheme|
|DEC|Decode|
|DFOTA|Delta Firmware Over-The-Air|
|DNS|Domain Name Server|
|DPCH|Dedicated Physical Channel|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    175 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

|DRX|Discontinuous Reception|
|---|---|
|DTE|Data Terminal Equipment|
|DTMF|Dual Tone Multi Frequency|
|DTR|Data Terminal Ready|
|ECC|Emergency Call|
|ECT|Explicit Call Transfer supplementary service|
|EFS|Embedded File System|
|EGPRS|Enhanced General Packet Radio Service|
|EMM|EPS Mobility Management|
|ENC|Encode|
|EONS|Enhanced Operator Name String|
|EPS|Evolved Packet System|
|ESM|EPS Session Management|
|E-UTRAN|Evolved UMTS Terrestrial Radio Access Network|
|EVDO|Evolution,Data Only|
|FDD|Frequency Division Duplexing|
|FDPCH|Fraction-Dedicated Physical Channel|
|FEM|Front-end Module|
|FPLMN|Forbidden PLMN|
|FTM|Factory Test Mode|
|FTP(S)|File Transfer Protocol over SSL|
|GBK|Chinese Internal Code Specification|
|GERAN|GSM EDGE Radio Access Network|
|GGSN|Gateway GPRS Support Node|
|GMT|Greenwich Mean Time|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    176 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

|GNSS|Global Navigation Satellite System|
|---|---|
|GPIO|General-Purpose Input/Output|
|GPRS|General Packet Radio Service|
|GSM|Global System for Mobile Communications|
|HDR|High-Dynamic Range|
|HSDPA|High Speed Downlink Packet Access|
|HSUPA|High Speed Uplink Packet Access|
|HTTP(S)|Hypertext Transfer Protocol over SSL|
|ICCID|Integrated Circuit Card Identifier|
|IDSN|Integrated Services Digital Network|
|IETF|The Internet Engineering Task Force|
|IIC|Inter-Integrated Circuit|
|IMEI|International Mobile Equipment Identity|
|IMS|IP Multimedia Subsystem|
|IMSI|International Mobile Subscriber Identity|
|Ipv4|Internet Protocol version 4|
|Ipv6|Internet Protocol version 6|
|IRA|International Reference Alphabet|
|IRAT|Inter-Radio Access Technology|
|IWF|Interactive Website Framework|
|LIPA|Local IP Access|
|LTE|Long Term Evolution|
|MBN|Modem Software Configuration|
|MCC|Mobile Country Code|
|MCU|Microprogrammed Control Unit|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    177 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

|ME|Mobile Equipment|
|---|---|
|MMS|Multimedia Messaging Service|
|MNC|Mobile Network Code|
|MO|Mobile Originated|
|MPTY|Multi-party|
|MS|Mobile Station|
|MSC|Mobile Services Switching Center|
|MSISDN|Mobile Subscriber International ISDN/PSTN number|
|MT|Mobile Terminal|
|MTU|Maximum Transmission Unit|
|NMEA|National Marine Electronics Association|
|NIC|Network Interface Card|
||Network Identity and Time Zone / Network Informed Time Zone.|
|NITZ|It is a mechanism for provisioning local time and date, time zone and DST<br>offset, as well as network provider identity information, to mobile devices via<br>a wireless network.|
|NSAPI|Network Service Access Point Identifier|
|NVM|Non-Volatile Random Access Memory|
|OIR|Originating Identification Restriction|
|PCM|Pulse Code Modulation|
|PDN|Public Data Network|
|PDP|Packet Data Protocol|
|PDU|Protocol Data Unit|
|PIN|Personal Identification Number|
|PLMN|Public Land Mobile Network|
|PPP|Point to Point Protocol|
|PSC|Primary Synchronization Code|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    178 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

|PUK|Personal Identification Number Unlock Key|
|---|---|
|QCI|QoS Class Identifier|
|QMI|Qualcom Message Interface|
|QoS|Quality of Service|
|RAT|Radio Access Technology|
|RDI|Remote Defect Indication|
|RI|Ring Indicator|
|RLP|Radio Link Protocol|
|RPLMN|Registered PLMN|
|RTC|Real Time Clock|
|RTS/CTS|Request To Send/Clear To Send|
|RSCP|Received Signal Code Power|
|RxQual|Received Signal Quality|
|SAP|Service Access Point|
|SDU|Service Data Unit|
|SMS|Short Message Service|
|SMSC|Short Message Service Center|
|SMTP|Simple Mail Transfer Protocol|
|SN|Serial Number|
|SNDCP|Subnetwork Dependent Convergence Protocol|
|SSL|Secure Sockets Layer|
|TA|Terminal Adapter|
|TCP|Transmission Control Protocol|
|TDD|Time Division Duplexing|
|TDSCDMA|Time Division-Synchronous Code Division Multiple Access|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    179 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

|TE|Terminal Equipment|
|---|---|
|TFT|Traffic Flow Template|
|TTS|Text To Speech|
|UARFCN|UTRA Absolute Radio Frequency Channel Number|
|UART|Universal Asynchronous Receiver/Transmitter|
|UAC|USB Audio Class|
|UCS2|Unicode|
|UDH|User Data Header|
|UDI|Unique Device Identification|
|UDP|User Datagram Protocol|
|UDUB|User Determined User Busy|
|UE|User Equipment|
|UFS|User File System|
|UMTS|Universal Mobile Telecommunications System|
|UICC|Universal Integrated Circuit Card|
|URC|Unsolicited Result Code|
|USB|Universal Serial Bus|
|(U)SIM|Universal Subscriber Identity Module|
|USSD|Unstructured Supplementary Service Data|
|UTRAN|UMTS Terrestrial Radio Access Network|
||Voice (voice calls) over LTE. A standard high-speed wireless communication|
|VoLTE|for mobile phones and data terminals, including Internet of things devices<br>and wearables.|
|WCDMA|Wideband Code Division Multiple Access|
|WLAN|Wireless Local Area Network|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    180 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

##### **12.1. Factory Default Settings Restorable with AT&F** 

**Table 12: Factory Default Settings Restorable with AT&F** 

|**AT Command**|**Parameters**|**Factory Defaults**|
|---|---|---|
|ATE|<value>|1|
|ATQ|<n>|0|
|ATS3|<n>|13|
|ATS4|<n>|10|
|ATS5|<n>|8|
|ATV|<value>|1|
|ATX|<value>|4|
|AT&C|<value>|1|
|AT&D|<value>|2|
|AT+CREG|<n>|0|
|AT+CGREG|<n>|0|
|AT+CMEE|<n>|1|
|AT+CSCS|<chset>|"GSM"|
|AT+CSMS|<service>|0|
|AT+CMGF|<mode>|0|
|AT+CSMP|<fo>,<vp>,<pid>,<dcs>|17,167,0,0|
|AT+CSDH|<show>|0|
|AT+CSCB|<mode>|0|
|AT+CPMS|<mem1>,<mem2>,<mem3>|"ME","ME","ME"|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    181 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

|AT+CNMI|<mode>,<mt>,<bm>,<ds>,<bfr>|2,1,0,0,0|
|---|---|---|
|AT+CMMS|<n>|0|
|AT+CTZR|<reporting>|0|
|AT+CGEREP|<mode>,<brf>|0,0|
|AT+CEREG|<n>|0|



##### **12.2. AT Command Settings Storable with AT&W** 

**Table 13: AT Command Settings Storable with AT&W** 

|**AT Command**|**Parameters**|**Display with AT&V**|
|---|---|---|
|ATE|<value>|Yes|
|ATQ|<n>|Yes|
|ATV|<value>|Yes|
|ATX|<value>|Yes|
|AT&C|<value>|Yes|
|AT&D|<value>|Yes|
|AT+IPR|<rate>|No|
|AT+CREG|<n>|No|
|AT+CGREG|<n>|No|
|AT+CEREG|<n>|No|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    182 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

##### **12.3. AT Command Settings Storable with ATZ** 

**Table 14: AT Command Settings Storable with ATZ** 

|**AT Command**|**Parameters**|**Factory Defaults**|
|---|---|---|
|ATE|<value>|1|
|ATQ|<n>|0|
|ATV|<value>|1|
|ATX|<value>|4|
|AT&C|<value>|1|
|AT&D|<value>|2|
|AT+CREG|<n>|0|
|AT+CGREG|<n>|0|
|AT+CEREG|<n>|0|



##### **12.4. Summary of CME ERROR Codes** 

Final result code **+CME ERROR: <err>** indicates an error related to mobile equipment or network. The operation is similar to **ERROR** result code. None of the following commands in the same command line is executed. Neither **ERROR** nor **OK** result code shall be returned. 

**<err>** values are mostly used by common message commands. The following table lists most of general and GRPS related **ERROR** codes. For some GSM protocol failure cause described in GSM specifications, the corresponding **ERROR** codes are not included. 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    183 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

**Table 15: Different Coding Schemes of +CME ERROR: <err>** 

|**Code of <err>**|**Meaning**|
|---|---|
|0|Phone failure|
|1|No connection to phone|
|2|Phone-adaptor link reserved|
|3|Operation not allowed|
|4|Operation not supported|
|5|PH-SIM PIN required|
|6|PH-FSIM PIN required|
|7|PH-FSIM PUK required|
|10|SIM not inserted|
|11|SIM PIN required|
|12|SIM PUK required|
|13|SIM failure|
|14|SIM busy|
|15|SIM wrong|
|16|Incorrect password|
|17|SIM PIN2 required|
|18|SIM PUK2 required|
|20|Memory full|
|21|Invalid index|
|22|Not found|
|23|Memory failure|
|24|Text string too long|
|25|Invalid characters in text string|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    184 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

|26|Dial string too long|
|---|---|
|27|Invalid characters in dial string|
|30|No network service|
|31|Network timeout|
|32|Network not allowed - emergency calls only|
|40|Network personalization PIN required|
|41|Network personalization PUK required|
|42|Network subset personalization PIN required|
|43|Network subset personalization PUK required|
|44|Service provider personalization PIN required|
|45|Service provider personalization PUK required|
|46|Corporate personalization PIN required|
|47|Corporate personalization PUK required|
|901|Audio unknown error|
|902|Audio invalid parameters|
|903|Audio operation not supported|
|904|Audio device busy|



##### **12.5. Summary of CMS ERROR Codes** 

Final result code **+CMS ERROR: <err>** indicates an error related to mobile equipment or network. The operation is similar to ERROR result code. None of the following commands in the same command line is executed. Neither **ERROR** nor **OK** result code shall be returned. 

**<err>** values are mostly used by common message commands: 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    185 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

**Table 16: Different Coding Schemes of +CMS ERROR: <err>** 

|**Code of <err>**|**Meaning**|
|---|---|
|107|Other General problems|
|300|ME failure|
|301|SMS ME reserved|
|302|Operation not allowed|
|303|Operation not supported|
|304|Invalid PDU mode|
|305|Invalid text mode|
|310|SIM not inserted|
|311|SIM pin necessary|
|312|PH SIM pin necessary|
|313|SIM failure|
|314|SIM busy|
|315|SIM wrong|
|316|SIM PUK required|
|317|SIM PIN2 required|
|318|SIM PUK2 required|
|320|Memory failure|
|321|Invalid memory index|
|322|Memory full|
|330|SMSC address unknown|
|331|No network|
|332|Network timeout|
|340|No +CNMA acknowledgement expected|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    186 / 207** 

**LTE Standard Module Series** 



|350|Unknown|
|---|---|
|500|Unknown|
|510|Message blocked|



##### **12.6. Summary of URC** 

###### **Table 17: Summary of URC** 

|**Index**|**URC Display**|**Meaning**|**Condition**|
|---|---|---|---|
|1|+CREG: <stat>|Indicate registration status of the ME|**AT+CREG=1**|
|2|+CREG: <stat>[,<lac>,<ci>[,<<br>Act>]]|After cell neighborhood changing<br>shows whether the network has<br>currently indicated the registration of<br>the ME, with location area code|**AT+CREG=2**|
|3|+CGREG: <stat>|Indicate network registration status of<br>the ME|**AT+CGREG=1**|
|4|+CGREG: <stat>[,<lac>,<ci>[,<br><Act>]]|Indicate network registration and<br>location information of the ME|**AT+CGREG=2**|
|5|+CTZV: <tz>|Time zone reporting|**AT+CTZR=1**|
|6|+CTZE: <tz>,<dst>,<time>|Extended time zone reporting|**AT+CTZR=2**|
|7|+CMTI: <mem>,<index>|New message is received, and saved<br>to memory|See**AT+CNMI**|
|8|+CMT: [<alpha>],<length><C<br>R><LF><pdu>|New short message is received and<br>output directlyto TE (PDU mode)|See**AT+CNMI**|
|9|+CMT: <oa>,[<alpha>],<scts><br>[,<tooa>,<fo>,<pid>,<dcs>,<sc<br>a>,<tosca>,<length>]<CR><L<br>F><data>|New short message is received and<br>output directly to TE (Text mode)|See**AT+CNMI**|
||^HCMT:|||
|10|<oa>,<scts>,<lang>, <fmt>,<le<br>ngth>,<prt>,<prv>,<type>,<sta<br>t><CR><LF><data>|New short message is received and<br>output directly to TE|See**AT+CNMI**|
|11|+CBM: <length><CR><LF><p<br>du>|New CBM is received and output<br>directly(PDU mode)|See**AT+CNMI**|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    187 / 207** 

**LTE Standard Module Series** 



|12|+CBM: <sn>,<mid>,<dcs>,<pa<br>ge>,<pages><CR><LF><dat<br>a>|New CBM is received and output<br>directly to TE (Text mode)|See**AT+CNMI**|
|---|---|---|---|
|13|+CDS: <length><CR><LF><p<br>du>|New CDS is received and output<br>directly(PDU mode)|See**AT+CNMI**|
|14|+CDS: <fo>,<mr>,[<ra>],[<tor<br>a>],<scts>,<dt>,<st>|New CDS is received and output<br>directlyto TE(Text mode)|See**AT+CNMI**|
|15|+CDSI: <mem>,<index>|New<br>message<br>status<br>report<br>is<br>received, and saved to memory|See**AT+CNMI**|
||^HCDS:|||
|16|<oa>,<scts>,<lang>, <fmt>,<le<br>ngth>,<prt>,<prv>,<type>,<sta<br>t><CR><LF><data>|New CDS is received and output<br>directly to TE|See**AT+CNMI**|
|17|RDY|ME initialization is successful|N/A|
|18|+CFUN: 1|All function of the ME is available|N/A|
|19|+CPIN: <state>|SIM card pin state|N/A|
|20|+QIND: SMS DONE|SMS initialization finished|N/A|
|21|+QIND: PB DONE|Phonebook initialization finished|N/A|
|22|POWERED DOWN|Module power down|**AT+QPOWD**|
|23|+CGEV: REJECT <PDP_typ<br>e>, <PDP_addr>|A network request for PDP activation,<br>and was automaticallyrejected.|**AT+CGEREP=2,1**|
|24|+CGEV: NW REACT <PDP_t<br>ype>, <PDP_addr>,[<cid>]|The network request PDP<br>reactivation|**AT+CGEREP=2,1**|
|25|+CGEV: NW DEACT <PDP_t<br>ype>, <PDP_addr>, [<cid>]|The network has forced a context<br>deactivation|**AT+CGEREP=2,1**|
|26|+CGEV: ME DEACT <PDP_t<br>ype>, <PDP_addr>,[<cid>]|The ME has forced a context<br>deactivation.|**AT+CGEREP=2,1**|
|27|+CGEV: NW DETACH|The network has forced a Packet<br>Domain detach.|**AT+CGEREP=2,1**|
|28|+CGEV: ME DETACH|The mobile equipment has forced a<br>Packet Domain detach.|**AT+CGEREP=2,1**|
|29|+CGEV: NW CLASS <class>|The network has forced a change of<br>MS class.|**AT+CGEREP=2,1**|
|30|+CGEV: ME CLASS <class>|The mobile equipment has forced a<br>change of MS class.|**AT+CGEREP=2,1**|
|31|+USIM: 0|Use SIM card|N/A|
|32|+USIM: 1|Use USIM card|N/A|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    188 / 207** 

**LTE Standard Module Series** 



##### **12.7. SMS Character Sets Conversions** 

In _3GPP TS 23.038_ DCS (Data Coding Scheme) defined three kinds of alphabets in SMS, GSM 7 bit default alphabet, 8 bit data and UCS2(16 bit). **AT+CSMP** can set the DCS in text mode ( **AT+CMGF=1** ). In text mode, DCS (Data Coding Scheme) and **AT+CSCS** determine the way of SMS text input or output. 

**Table 18: The Way of SMS Text Input or Output** 

|**DCS**|**AT+CSCS**|**The Way of SMS Text Input or Output**|
|---|---|---|
|GSM 7 bit|GSM|Input or output GSM character sets.|
|||Input or output IRA character sets.|
|GSM 7 bit|IRA|Input: UE will convert IRA characters to GSM characters.<br>Output: UE will convert GSM characters to IRA characters.|
|GSM 7 bit|UCS2|Input or output a hex string similar to PDU mode. So only support<br>characters 0–9 and A–F.<br>Input: UE will convert the UCS2 hex string to GSM characters.<br>Output: UE will convert the GSM characters to UCS2 hex string.|
|UCS2|-|Ignore the value of AT+CSCS, input or output a hex string similar to<br>PDU mode. So onlysupport characters 0–9 and A–F .|
|8 bit|-|Ignore the value of AT+CSCS**,**input or output a hex string similar to<br>PDU mode. So onlysupport characters 0–9 and A–F.|



When DCS = GSM 7 bit, the input or output needs conversion. The detailed conversion tables are shown as below. 

**Table 19: The Input Conversions Table (DCS = GSM 7 bit and AT+CSCS="GSM")** 

||**0**|**1**|**2**|**3**|**4**|**5**|**6**|**7**|
|---|---|---|---|---|---|---|---|---|
|0|00|10|20|30|40|50|60|70|
|1|01|11|21|31|41|51|61|71|
|2|02|12|22|32|42|52|62|72|
|3|03|13|23|33|43|53|63|73|
|4|04|14|24|34|44|54|64|74|
|5|05|15|25|35|45|55|65|75|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    189 / 207** 

**LTE Standard Module Series** 



|6|06|16|26|36|46|56|66|76|
|---|---|---|---|---|---|---|---|---|
|7|07|17|27|37|47|57|67|77|
|8|08|18|28|38|48|58|68|78|
|9|09|19|29|39|49|59|69|79|
|A|0A|Submit|2A|3A|4A|5A|6A|7A|
|B|0B|Cancel|2B|3B|4B|5B|6B|7B|
|C|0C|1C|2C|3C|4C|5C|6C|7C|
|D|0D|1A|2D|3D|4D|5D|6D|7D|
|E|0E|1E|2E|3E|4E|5E|6E|7E|
|F|0F|1F|2F|3F|4F|5F|6F|7F|



**Table 20: The Output Conversions Table (DCS = GSM 7 bit and AT+CSCS="GSM")** 

||**0**|**1**|**2**|**3**|**4**|**5**|**6**|**7**|
|---|---|---|---|---|---|---|---|---|
|0|00|10|20|30|40|50|60|70|
|1|01|11|21|31|41|51|61|71|
|2|02|12|22|32|42|52|62|72|
|3|03|13|23|33|43|53|63|73|
|4|04|14|24|34|44|54|64|74|
|5|05|15|25|35|45|55|65|75|
|6|06|16|26|36|46|56|66|76|
|7|07|17|27|37|47|57|67|77|
|8|08|18|28|38|48|58|68|78|
|9|09|19|29|39|49|59|69|79|
|A|0D0A||2A|3A|4A|5A|6A|7A|
|B|0B||2B|3B|4B|5B|6B|7B|
|C|0C|1C|2C|3C|4C|5C|6C|7C|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    190 / 207** 

**LTE Standard Module Series** 



|D|0D|1A|2D|3D|4D|5D|6D|7D|
|---|---|---|---|---|---|---|---|---|
|E|0E|1E|2E|3E|4E|5E|6E|7E|
|F|0F|1F|2F|3F|4F|5F|6F|7F|



###### **Table 21: GSM Extended Characters** 

|**0**<br>**1**|**2**|**3**|**4**|**5**<br>**6**<br>**7**|
|---|---|---|---|---|
|0|||1B40||
|1|||||
|2|||||
|3|||||
|4<br>1B14|||||
|5|||||
|6|||||
|7|||||
|8|1B28||||
|9|1B29||||
|A|||||
|B|||||
|C||1B3C|||
|D||1B3D|||
|E||1B3E|||
|F|1B2F||||



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    191 / 207** 

**LTE Standard Module Series** 



**Table 22: The Input Conversions Table (DCS = GSM 7 bit and AT+CSCS="IRA")** 

||**0**|**1**|**2**|**3**|**4**|**5**|**6**|**7**|
|---|---|---|---|---|---|---|---|---|
|0||20|20|30|00|50|20|70|
|1|20|20|21|31|41|51|61|71|
|2|20|20|22|32|42|52|62|72|
|3|20|20|23|33|43|53|63|73|
|4|20|20|02|34|44|54|64|74|
|5|20|20|25|35|45|55|65|75|
|6|20|20|26|36|46|56|66|76|
|7|20|20|27|37|47|57|67|77|
|8|backspace|20|28|38|48|58|68|78|
|9|20|20|29|39|49|59|69|79|
|A|0A|Submit|2A|3A|4A|5A|6A|7A|
|B|20|Cancel|2B|3B|4B|1B3C|6B|1B28|
|C|20|20|2C|3C|4C|1B2F|6C|1B40|
|D|0D|20|2D|3D|4D|1B3E|6D|1B29|
|E|20|20|2E|3E|4E|1B14|6E|1B3D|
|F|20|20|2F|3F|4F|11|6F|20|



**Table 23: IRA Extended Characters** 

||**A**|**B**|**C**|**D**|**E**|**F**|
|---|---|---|---|---|---|---|
|0|20|20|20|20|7F|20|
|1|40|20|20|5D|20|7D|
|2|20|20|20|20|20|08|
|3|01|20|20|20|20|20|
|4|24|20|5B|20|7B|20|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    192 / 207** 

**LTE Standard Module Series** 



|5|03|20|0E|20|0F|20|
|---|---|---|---|---|---|---|
|6|20|20|1C|5C|1D|7C|
|7|5F|20|09|20|20|20|
|8|20|20|20|0B|04|0C|
|9|20|20|1F|20|05|06|
|A|20|20|20|20|20|20|
|B|20|20|20|20|20|20|
|C|20|20|20|5E|07|7E|
|D|20|20|20|20|20|20|
|E|20|20|20|20|20|20|
|F|20|60|20|1E|20|20|



**Table 24: The Output Conversions Table (DCS = GSM 7 bit and AT+CSCS="IRA")** 

||**0**|**1**|**2**|**3**|**4**|**5**|**6**|**7**|
|---|---|---|---|---|---|---|---|---|
|0|40|20|20|30|A1|50|BF|70|
|1|A3|5F|21|31|41|51|61|71|
|2|24|20|22|32|42|52|62|72|
|3|A5|20|23|33|43|53|63|73|
|4|E8|20|A4|34|44|54|64|74|
|5|E9|20|25|35|45|55|65|75|
|6|F9|20|26|36|46|56|66|76|
|7|EC|20|27|37|47|57|67|77|
|8|F2|20|28|38|48|58|68|78|
|9|C7|20|29|39|49|59|69|79|
|A|0D0A||2A|3A|4A|5A|6A|7A|
|B|D8||2B|3B|4B|C4|6B|E4|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    193 / 207** 

**LTE Standard Module Series** 



|C|F8|C6|2C|3C|4C|D6|6C|F6|
|---|---|---|---|---|---|---|---|---|
|D|0D|E6|2D|3D|4D|D1|6D|F1|
|E|C5|DF|2E|3E|4E|DC|6E|FC|
|F|E5|C9|2F|3F|4F|A7|6F|E0|



###### **Table 25: GSM Extended Characters** 

|**0**|**1**|**2**|**3**|**4**|**5**<br>**6**<br>**7**|
|---|---|---|---|---|---|
|0||||7C||
|1||||||
|2||||||
|3||||||
|4|5E|||||
|5||||||
|6||||||
|7||||||
|8||7B||||
|9||7D||||
|A||||||
|B||||||
|C|||5B|||
|D|||7E|||
|E|||5D|||
|F||5C||||



Because the low 8 bit of UCS2 character is the same as the IRA character: 

The conversion table of DCS = GSM 7 bit and **AT+CSCS="UCS2"** is similar to **AT+CSCS="IRA"** . The conversion table of fmt = GSM 7 bit and **AT+CSCS="GSM"** is similar to **AT+CSCS="GSM"** . 

**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    194 / 207** 

**LTE Standard Module Series** 



The conversion table of fmt = GSM 7 bit and **AT+CSCS="IRA"** is similar to **AT+CSCS="IRA"** . The conversion table of fmt = GSM 7 bit and **AT+CSCS="UCS2"** is similar to **AT+CSCS="IRA"** . 

The difference is the way of SMS text input or output. Please refer to **_Table 17_** : Summary of URC for more details. 

##### **12.8. Release Cause Text List of AT+CEER** 

**Table 26: List of Location ID List** 

|**Location ID**|**Meaning**|
|---|---|
|0|CS internal cause|
|1|CS network cause|
|2|CS network reject|
|3|PS internal cause|
|4|PS network cause|
|5|PS LTE cause|
|6|PS LTE local cause|



###### **Table 27: List of Cause** 

|**CS Internal Cause**|**Meaning**|
|---|---|
|-1|No cause information available (default)|
|0|Phone is offline|
|21|No service available|
|25|Network release, no reason given|
|27|Received incoming call|
|29|Client ended call|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    195 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

|34|UIM not present|
|---|---|
|35|Access attempt already in progress|
|36|Access failure, unknown source|
|38|Concur service not supported by network|
|39|No response received from network|
|45|GPS call ended for user call|
|46|SMS call ended for user call|
|47|Data call ended for emergency call|
|48|Rejected during redirect or handoff|
|100|Lower-layer ended call|
|101|Call origination request failed|
|102|Client rejected incoming call|
|103|Client rejected setup indication|
|104|Network ended call|
|105|No funds available|
|106|No service available|
|108|Full service not available|
|109|Maximum packet calls exceeded|
|301|Video connection lost|
|302|Video call setup failure|
|303|Video protocol closed after setup|
|304|Video protocol setup failure|
|305|Internal error|
|**CS Network Cause**|**Meaning**|
|1|Unassigned/unallocated number|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    196 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

|3|No route to destination|
|---|---|
|6|Channel unacceptable|
|8|Operator determined barring|
|16|Normal call clearing|
|17|User busy|
|18|No user responding|
|19|User alerting, no answer|
|21|Call rejected|
|22|Number changed|
|26|Non selected user clearing|
|27|Destination out of order|
|28|Invalid/incomplete number|
|29|Facility rejected|
|30|Response to status enquiry|
|31|Normal, unspecified|
|34|No circuit/channel available|
|38|Network out of order|
|41|Temporary failure|
|42|Switching equipment congestion|
|43|Access information discarded|
|44|Requested circuit/channel not available|
|47|Resources unavailable, unspecified|
|49|Quality of service unavailable|
|50|Requested facility not subscribed|
|55|Incoming calls barred within the CUG|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    197 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

|57|Bearer capability not authorized|
|---|---|
|58|Bearer capability not available|
|63|Service/option not available|
|65|Bearer service not implemented|
|68|ACM >= ACM max|
|69|Requested facility not implemented|
|70|Only RDI bearer is available|
|79|Service/option not implemented|
|81|Invalid transaction identifier value|
|87|User not member of CUG|
|88|Incompatible destination|
|91|Invalid transit network selection|
|95|Semantically incorrect message|
|96|Invalid mandatory information|
|97|Message non-existent/not implemented|
|98|Message type not compatible with state|
|99|IE non-existent/not implemented|
|100|Conditional IE error|
|101|Message not compatible with state|
|102|Recovery on timer expiry|
|111|Protocol error, unspecified|
|127|Interworking, unspecified|
|**CS Network Reject**|**Meaning**|
|2|IMSI unknown in HLR|
|3|Illegal MS|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    198 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

|4|IMSI unknown in VLR|
|---|---|
|5|IMEI not accepted|
|6|Illegal ME|
|7|GPRS services not allowed|
|8|GPRS and non GPRS services not allowed|
|9|MS identity cannot be derived|
|10|Implicitly detached|
|11|PLMN not allowed|
|12|Location area not allowed|
|13|Roaming not allowed|
|14|GPRS services not allowed in PLMN|
|15|No suitable cells in location area|
|16|MSC temporary not reachable|
|17|Network failure|
|20|MAC failure|
|21|Synch failure|
|22|Congestion|
|23|GSM authentication unacceptable|
|32|Service option not supported|
|33|Requested service option not subscribed|
|34|Service option temporary out of order|
|38|Call cannot be identified|
|40|No PDP context activated|
|95|Semantically incorrect message|
|96|Invalid mandatory information|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    199 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

|97|Message type non-existent|
|---|---|
|98|Message type not compatible with state|
|99|Information element non-existent|
|101|Message not compatible with state|
|161|RR release indication|
|162|RR random access failure|
|163|RRC release indication|
|164|RRC close session indication|
|165|RRC open session failure|
|166|Low level failure|
|167|Low level failure no redial allowed|
|168|Invalid SIM|
|169|No service|
|170|Timer T3230 expired|
|171|No cell available|
|172|Wrong state|
|173|Access class blocked|
|174|Abort message received|
|175|Other cause|
|176|Timer T303 expired|
|177|No resources|
|178|Release pending|
|179|Invalid user data|
|**PS Internet Cause**|**Meaning**|
|0|Invalid connection identifier|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    200 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

|1|Invalid NSAPI|
|---|---|
|2|Invalid primary NSAPI|
|7|PDP establish timeout|
|3|Invalid field|
|4|SNDCP failure|
|5|RAB setup failure|
|6|No GPRS context|
|8|PDP activate timeout|
|9|PDP modify timeout|
|10|PDP inactive max timeout|
|11|PDP lower layer error|
|12|PDP duplicate|
|13|Access technology change|
|14|PDP unknown reason|
|**PS Network Cause**|**Meaning**|
|25|LLC or SNDCP failure|
|26|Insufficient resources|
|27|Missing or unknown APN|
|28|Unknown PDP address or PDP type|
|29|User authentication failed|
|30|Activation rejected by GGSN|
|31|Activation rejected, unspecified|
|32|Service option not supported|
|33|Requested service option not subscribed|
|34|Service option temporary out of order|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    201 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

|35|NSAPI already used (not sent)|
|---|---|
|36|Regular deactivation|
|37|QoS not accepted|
|38|Network failure|
|39|Reactivation required|
|40|Feature not supported|
|41|Semantic error in the TFT operation|
|42|Syntactical error in the TFT operation|
|43|Unknown PDP context|
|44|PDP context without TFT already activated|
|45|Semantic errors in packet filter|
|46|Syntactical errors in packet filter|
|81|Invalid transaction identifier|
|95|Semantically incorrect message|
|96|Invalid mandatory information|
|97|Message non-existent/not implemented|
|98|Message type not compatible with state|
|99|IE non-existent/not implemented|
|100|Conditional IE error|
|101|Message not compatible with state|
|111|Protocol error, unspecified|
|**PS LTE Cause**|**Meaning**|
|8|Operator determined barring|
|26|Insufficient Resources|
|27|Missing or unknown APN|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    202 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

|28|Unknown PDP address or PDP type|
|---|---|
|29|User Aauthentication faile|
|30|Activation rejected by Servicing GW or PDN GW|
|31|Activation rejected, unspecified|
|32|Service option not supported|
|33|Requested service option not subscribed|
|34|Service option temporarily out of order|
|35|PTI already In use|
|36|Regular deactivation|
|37|QoS not accepted|
|38|Network failure|
|39|Reactivation required|
|40|Feature not supported|
|41|Semantic error in the TFT operation|
|42|Syntactical error in the TFT operation|
|43|Unknown Bearer context|
|44|Semantic errors in packet filter|
|45|Syntactical errors in packet filter|
|46|Bearer Context without TFT already Active|
|47|PTI mismatch|
|49|PDN disconnected, not allowed|
|50|PDN type IPV4 only Allowed|
|51|PDN type IPV6 only Allowed|
|52|Single ADR bearers only Allowed|
|53|ESM info not recieved|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    203 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

|54|PDN connection does not exist|
|---|---|
|55|Multiple PDN connection for given APN not allowed|
|56|Collision with network init request|
|59|Unsupported QCI value|
|81|Invalid PTI value|
|95|Symantically invalid message|
|96|Invalid mandatory information|
|97|Message non-existent/not implemented|
|98|Message type not compatible with state|
|99|Info Elemant non existent|
|100|Conditional IE error|
|101|Message type not compatible with state|
|111|Protocol error, unspecified|
|112|APN restrict value incompatible with ACT context|
|**PS LTE Local Cause**|**Meaning**|
|3|Illegal UE|
|6|Illegal ME|
|7|EPS services not allowed|
|9|UE id can't be driven by network|
|10|Implicitly Detached|
|11|PLMN not allowed|
|12|tracking area not allowed|
|13|Roaming not allowed in this tracking area|
|15|No Suitable cells in tracking area|
|18|CS Domain Not available|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    204 / 207** 

**LTE Standard Module Series** 



<!-- Start of picture text -->
QRUWECCTEL<br><!-- End of picture text -->

|25|Not Authorized for this CSG|
|---|---|
|38|CS fallback call EST not allowed|
|39|CS domain temperarly not allowed|
|43|Unknown EPS bearer context|
|256|Released AT RRC|
|257|Signal Connection Released|
|258|EMM detached|
|259|EMM attach failed|
|260|EMM attach started|
|261|NAS service request failed|
|262|ESM activate dedicated bearer reactivater by network|
|263|Lower layer failure|
|264|Lower layer failure|
|265|Network activater dedicated bearer with ID of deffered bearer|
|266|BAD OTA message|
|267|DS rejected the call|
|268|Context transferred due to IRAT|
|269|DS explicit deactivation|
|270|ESM MSGR failure|
|271|Local Cause not Available|
|272|Rejected due to connected state|
|273|Nas Service request failed, no throttle|
|274|ACL failure|
|275|Nas Service request failed, DS disallow|
|276|EMM T3417 expired|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    205 / 207** 

**LTE Standard Module Series** 



|277|EMM T3417 ext expired|
|---|---|
|278|Nas LRRC UL data CNF failure TXN|
|279|Nas LRRC UL data CNF failure HO|
|280|Nas LRRC UL data CNF failure Conn release|
|281|Nas LRRC UL data CNF failure RLF|
|282|Nas LRRC UL data CNF failure control Not CONN|
|283|NAS LRRC connection EST success|
|284|NAS LRRC connection EST failure|
|285|NAS LRRC connection EST failure, aborted|
|286|NAS LRRC connection EST failure, access barrer|
|287|NAS LRRC connection EST failure, CELL resel|
|288|NAS LRRC connection EST failure, config failure|
|289|NAS LRRC connection EST failure, timer expired|
|290|NAS LRRC connection EST failure, link failure|
|291|NAS LRRC connection EST failure, not camped|
|292|NAS LRRC connection EST failure, SI failure|
|293|NAS LRRC connection EST failure, CONN reject|
|294|NAS LRRC connection release normal|
|295|NAS LRRC connection release RLF|
|296|NAS LRRC connection release CRE failure|
|297|NAS LRRC connection release QOS during CRE|
|298|NAS LRRC connection release aborted|
|299|NAS LRRC connection release SIB read error|
|300|NAS LRRC connection release aborted IRAT Success|
|301|Nas Reject LRRC radio link failure|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    206 / 207** 

**LTE Standard Module Series** 



|302|Nas service request failure, LTE network reject|
|---|---|
|303|Nas detach with reattach, LTE network detach|
|304|NAS detach without reacttach, LTE network detach|



**EG800Q&EG91xQ_Series_AT_Commands_Manual                                    207 / 207** 

