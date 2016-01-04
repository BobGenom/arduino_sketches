RCX2_IR_Programmer1
================

Arduino based RCX2 Programmer to be used with lejosfirmdl.exe


Hardware layout for the RCX Programmer.
---------------------

```
Pin usage IR LED and TSOP for 950nm and 38kHz carrier:

 IR-LED:SFH-409                 TSOP 31236
 cathode  anode
    -       +          Data           VCC   GND
    |       |            |             |     |
    |       |            |             |     |
    |       |            |             |     |
    |       |            |             |     |
            |            |             |     |
    9      10           11     12     13    GND

ARDUINO
```

Precondition
---------------------
Arduino drivers are installed.


Description
-----
This is a half duplex solution. Therefore it needs aprox. twice the time
for programming the lego RCX brick when compared to the original lego tower.
For example: for programming a 7850 Bytes program you need about 
150 secs whereas with a full duplex hardware you need
 80 secs.

This is, because producing the carrier 38khz when sending
a data byte over IR is done busy waiting and not using interupts.
All timing values in function send(data) are manualy optimized for
a 16MHz ATMega328p. So this (not using interupts) is a more 
generic solution which should work on many Arduino flavours.

A future version might use interupts and could be able to do
transmission in full duplex to speed up data transfer.

If the Arduino pops up as e.g. COM16 you have to use port \\\.\COM16 for lejosfirmdl:
```
lejosfirmdl --tty=\\\.\COM16 HelloWorld.srec
```



