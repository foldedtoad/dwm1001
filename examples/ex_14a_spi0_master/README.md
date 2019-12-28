
# DWM1001 - ex_14a_main

## Overview

This example project give a simple template for constructing a 'SPI Slave' device.
This allows the DWM1001 board to appear to a 'SPI Master' system (PC, RasPi, etc.) as a SPI device. 

From this start, you can add your own unique command/response sequences.
Currently, this example does not implement a interrupt line to the SPI Master host, although the Device Tree does define a potential line for interrupts.

Be forewarned, this is a rather complex project to setup and run.  
It requires elements which must be provided by the user:

* Host computer or system to act as SPI Master.
* Software on host to initiate SPI transactions.
* Logic analyzer or other SPI transaction protocol tracing facility.
* Soldering skills: Soldering 2x4 0.1" header to J7 pads.
* Cables to connect host to DWM1001 board J7 header.


## Hardware Setup

First, solder a 2x4 0.1" header onto the J7 pads on the DWM1001 board. Alternatively, you can solder two 1x4 0.1" headers.

Cabling between the host's SPI interface and the DWM1001 J7 header is needed. It is suggested to buy ready-made ribbon cables with female headers attached. 

Probably the easiest way to test is with a PC-based SPI interface adapter.
In this case the Total Phase "Aardvark I2C/SPI" adapter with the "Control Center" program for Linux running on Ubuntu 18.04 was used.

Alternatively, you can use a Raspberry-Pi to act as the SPI Master. This document will not describe the setup for this, but it is well-documented on the internet: google's your friend.

Header J7 is used to connect a SPI host to the DWM1001 board.
The following J7 header pins are used.


```
    SCK     M Pin21     P0.01   J7-1 (square hole) 
    MOSI    M Pin22     P0.00   J7-2 
    MISO    M Pin6      P0.12   J7-3 
    CS      M Pin13     P0.27   J7-4 
    INT     M Pin16     P0.23   J7-7 
```

## Requirements


## Building and Running


## Sample Output
