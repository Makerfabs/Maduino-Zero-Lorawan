# Maduino Zero Lorawan


```c++
/*
Version:		V1.0
Author:			Vincent
Create Date:	2021/5/19
Note:
	
*/
```


![](md_pic/main.jpg)


[toc]

# Makerfabs

[Makerfabs home page](https://www.makerfabs.com/)

[Makerfabs Wiki](https://www.makerfabs.com/wiki/index.php?title=Main_Page)

# Maduino Zero Lorawan
## Intruduce

Product Link ：[]() 

Wiki Link :  []() 



## Feature

- ATSAMD21G18, 32-Bit ARM Cortex M0+
- Interface: I2C/SPI/UART/18*GPIO
- With super anti-interference ability, it can work normally in complex interference environment.
- Minimum receiving sensitivity: -137dBm (SF=12/BW=125KHz)
- Maximum transmit power +21dBm
- Working frequency: (Ra-07) 410mhz ~ 525mhz (default), (Ra-07h) 803mhz ~ 930mhz (default)
- Power supply voltage input: 3.3V
- Transmitting working current: 107mA (full load power consumption)
- Receive working current: 6mA
- Sleep current: 3uA

### Front:

![front](md_pic/front~1.jpg)

### Back:
![back](md_pic/back~1.jpg)



# Example


## Equipment list

- Maduino Zero Lorawan


## Compiler Options
**If you have any questions，such as how to install the development board, how to download the code, how to install the library. Please refer to :[Makerfabs_FAQ](https://github.com/Makerfabs/Makerfabs_FAQ)**

- Install library : DHT_sensor_library.
- Upload codes, select "Arduino Zero" and "Native USB"


## Example List

### at-test

Simple at command test demo

### at-test-lorawan

Register the Lorawan gateway in OTA mode and send data to the TTN. And send message to ThingSpeak.
![thingspeak](md_pic/thingspeak.jpg)

## AT Command Explain

### AT+CDEVEUI?
*Inquire DEVEUI right now.*

AT+CDEVEUI?

### AT+CDEVEUI=<DevEUI>

*Set DEVEUI, APPEUI and APPKEY.*

AT+CDEVEUI=<DevEUI:length is 16>
AT+CAPPEUI=<AppEUI:length is 16>
AT+CAPPKEY=<AppKey:length is 32>

OTAA join mode need these three numbers.

### AT+CJOINMODE=<MODE>
*Set join mode via OTAA.*

AT+CJOINMODE=<MODE>
<MODE>
0：OTAA
1：ABP
Defualt mode is OTAA.

AT+CJOINMODE=0


### AT+CJOIN=<ParaValue1>,[ParaValue2],…[ParaValue4]
*Join Lorawan to Lorawan webgate.*

AT+CJOIN=<ParaValue1>,[ParaValue2],…[ParaValue4]
<ParaTag1>
0: Stop JOIN
1: Start JOIN
[ParaValue2]
0: Disable Auto JOIN
1: Enable Auto JOIN
[ParaValue3]
JOIN cycle,range 7 to 255, unit is second。
Default value is 8.
[ParaValue4]
Maximum number of access attempts,range 1 to 256.

AT+CJOIN=1,0,10,1
Set JOIN parameter: Start join, disable automatic join, try period is 10s, maximum attempt times is 8.

### AT+DTRX=[confirm],[nbtrials],<Length>,<Payload>
*Send one message to webgate,and receive message.*

AT+DTRX=[confirm],[nbtrials],<Length>,<Payload>
[confirm]
0: UnConfirmed up message
1: Confirmed up message
[nbtrials]
Set sending times.
Confirm and nbtrials refer to the corresponding AT instruction, valid for this dispatch only, optional.
<Length>
Represents the number of strings. The maximum value is specified in the access specification. 
The length of bytes allowed to be transmitted is different at different rates (see the Lorawan protocol for details). 
0 indicates that empty packets are sent.
<Payload>
Hexadecimal (2 characters for 1 number)

AT+DTRX=1,2,5,0123456789
OK+SEND:05
OK+SENT:01
OK+RECV:02,01,00
Confirm data sent successfully, valid data received by server should be 0x01 0x23 0x45 0x67 0x89.
And received downlink confirmation 0x02,0x01,0x00.
