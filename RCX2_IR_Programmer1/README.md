Lego Brick RCX 2.0 IR Programmer Version 1
================

Arduino based RCX 2.0 Programmer to be used with lejosfirmdl.exe as a substitute for the Lego IR Tower. It is an incredible simple hardware design with zero soldering and zero wiring. Made to upload HaikuVM and LeJOS firmware and programs to the RCX 2.0.

Part list
-----------
* Arduino (I have a Yun)
* IR LED (I used SFH-409)
* IR Receiver (I used a TSOP 31236)

![IR LED](http://www.conrad.de/medias/global/ce/1000_1999/1500/1540/1543/154380_BB_00_FB.EPS_250.jpg)
![TSOP](http://elcodis.com/photos/15/68/156835/vishaysemitsop38238_sml.jpg)

See the assembled design:

![Programmer Backside](http://haiku-vm.sourceforge.net/images/ArdPro1-back.png)

See the Programmer in action:

![Programmer in Action](http://haiku-vm.sourceforge.net/images/ArdPro1-action.png)

Hardware layout for the RCX Programmer.
---------------------

```
Pin usage IR LED and TSOP for 950nm and 36kHz carrier:

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
I guess similar TSOPs with 950nm and 36kHz (or even better 38kHz, which is closer to the RCX 2.0 specs.) carrier will do as well.

Precondition
---------------------
Arduino drivers are installed.


Description
-----
Producing the carrier 38khz when sending
a data byte over IR is done busy waiting and not using interrupts.
All timing values in function send(data) are manualy optimized for
a 16MHz ATmega32u4. So this (not using interrupts) is a more 
generic solution which should work on many Arduino flavours.

A future version might use interrupts.

Usage
-----
If the Arduino pops up as e.g. COM16 you have to use port \\\\\\.\COM16 for lejosfirmdl:
```
lejosfirmdl --tty=\\\.\COM16 HelloWorld.srec
```

Works best within a distance of less than 30cm between the Arduino IR Programmer and the Lego RCX 2.0 Brick.



