# hoymiles-nrf-comm_demo

! WORK IN PROGRESS !
------------------------

Demo project for establishing communication with Hoymiles microinverters (HM-300/400/600/...) via proprietary 2.4 GHz NRF wireless modules.

Everyone is invited to participate in reverse engineering of the protocol. Further information see: https://www.mikrocontroller.net/topic/525778

-----------------

## Radio interface

The hoymiles inverters integrate a NRF24LE1E radio communication interface that sends data packets at 2.4 GHz with a proprietary protocol. Normally a *DTU* is used, that transmits the inverter actual values into the manufacturer's cloud.
The goal of this project is, to reverse engineer the protocol for building own interfaces with the widespread NRF24L01 modules, to enable integration of the inverter actual values in own smart home installations without cloud connection.

### NRF Settings

The inverters communicate with the DTU on the following, alternating channels (for evading WiFi?)

|Channel|Frequency|
|---|---|
|3|2.403 GHz|
|23|2.423 GHz|
|40|2.440 GHz|
|61|2.461 GHz|
|75|2.475 GHz|

NRF Data Rate is 250 kBit, Payload length depends on data to send.

### NRF Frame - Enhanced Shockburst used in HM inverters

| PREAMBEL | ADDRESS | PACKET CONTROL | DATA | CRC |
|---|---|---|---|---|
|0x55 or 0xAA|5 Byte|9 Bit| up to 32 Byte|2 Byte|
| |Defined by user*|Contains data packet length|Actual data**||

\*Address[4..1]: last 8 digits of inverter serial no. in "little endian".  
Example:  
Ser.No: 1010 1234 5678 ---> Address[4..1] = {0x78, 0x56, 0x34, 0x12}.  
Address[0] = 0x01

\** Actual data length varies, depending on transmitted data.  
This is the part that contains the inverter actual values. The structure and how to parse is described in the following section.  

## Hoymiles data protocol

A HM inverter does not start communication on its own. It needs to be polled by a *DTU (=Data Transfer Unit)*.  
Therefore it is neccessary to initiate the communication. Packets sent by the user device are named as *DTU* packets, response of the inverter is always labeled with *INV* or *HM*.

### Data packet structure
The data packets seem to be structured like this:

| Command Byte | Receiver Address | Transmitter Address | Flag & Packet ID | Data ID | Data | CRC8 |
|---|---|---|---|---|---|---|
| 1 Byte | 4 Byte | 4 Byte | 1 Byte | 1 Byte | 0..16 Byte | 1 Byte |

### CRC checking

The data packets are checked with a CRC8 - Polynome 0x01 - Start Value 0x00 - Final XOR 0x00.  
It is calculated over the complete data packet from command byte to last data byte.

### Packet contents

The following data packets have been identified by now:


Type Length 
