
# DWM1001 - ex_14b_spi0_slave

## Overview

This example project give a simple template for constructing a 'SPI Slave' device.
This allows the DWM1001 board to appear to a 'SPI Master' system (PC, RasPi, etc.) as a SPI device. Using a second DWM1001 programmed as a SPI-master (host) also possible: see *ex_14a_spi0_master* example.

The focus of this project is to provide an example of SPI-Slave IO connectivity, and not so much the SPI-device "functionality".  The functionlity will be determined by your specific project's needs.
In other words, you can add your own unique command/response sequences.

Currently, this example does not implement a interrupt line to the SPI Master host, although the Device Tree does define a potential line for interrupts.

The choice using of "SPI_0" is due to "SPI-1" being used for DWM1000 communications. Also note that SPI_0 and I2C_0 can not operated at the same time, as the share a common serial controller on the nRF52 SoC. 

Be forewarned, this is a more complex project to setup and run.  
It requires elements which must be provided by the user:

* Host system to act as SPI Master.
* Software on host to initiate SPI transactions.
* Logic analyzer or other SPI transaction protocol tracing facility.
* Soldering skills: Soldering 2x4 0.1" header to J7 pads.
* Cables to connect host to DWM1001 board J7 header.


## Hardware Setup

First, solder a 2x4 0.1" header onto the J7 pads on the DWM1001 board. Alternatively, you can solder two 1x4 0.1" headers.

Cabling between the host's SPI interface and the DWM1001 J7 header is needed. It is suggested to buy ready-made ribbon cables with female headers attached. 

Probably the easiest way to test is with another DWM1001 board acting as the SPI-Master (again see the *ex_14a_spi0_master* example).

Other alternative SPI-Master host are

* Raspberry Pi
* Nordic's nRF52 Eval Boards, PCA10040 and the ilk. For the PCA10040, see this [project](https://github.com/foldedtoad/spi_slave.git).
* PC with SPI adapter, ex. Bus-Pirate, Total-Phase Aardvark, etc. 

**Note:** Be sure the master-side is operating at 3.3V logic levels or that a SPI level-converter is provided. SparkFun sells them; last seen [here](https://www.sparkfun.com/products/12009).

**Note:** Be sure to connect the two SPI platforms together with common ground. There is a "Gnd Loop Connector", TP8, on the DWM1001 which can be used.


There is a photo in the *docs* directory which shows two DWM1001 boards connected in a Master-to-Slave configuration: see spim2spis.jpg.

Header J7 is used to connect a SPI host to the DWM1001 board.
The following J7 header pins are used.


```
    MOSI    M Pin6      P0.12   J7-3 
    MISO    M Pin13     P0.27   J7-4 
    SCK     M Pin16     P0.23   J7-7 
    CS      M Pin17     P0.13   J7-8 
```

**Note:** This is the same pin configuration as used in the *ex_14a_spi0_master* side.

## Building and Running
The build and running is similar to the other examples.
The only complexity is coordinating the operation of two SPI-based systems. But as a DWM1001 developer this is standard procedures.  ;-)

## Sample Output

The *docs* directory contains a SPI trace showing one SPI transaction cycle: see spim2spis_trace.png.

Below is the output from the DWM1001 SPI-Master side, followed by the output from the DWM1001 SPI-Slave side.  

<u>
**Master Side**</u>

```
    - - - - - - - - - - - - - - - - - TARGET RESET - - - - - - - - - - - - - - - - -
    ***** Booting Zephyr OS build zephyr-v2.0.0-882-g89a984e64424 *****
    
    spi_master_thread
    
    SPI Master example application
    
    rx_data buffer at 0x20000b8c
    
    spi_master_init: SPI_0 master config [wordsize(8), mode(0/0/0)]
    
    spi_master_init: SPI pin config -- MOSI(P0.12), MISO(P0.27), SCK(P0.23), CS(P0.13)
    
    Sent: 0x1234
    
    Received: 0x5678
    
    Response is ok
    
    Sent: 0x1234
    
    Received: 0x5678
    
    Response is ok
```


<u>
**Slave Side**</u>

```
    - - - - - - - - - - - - - - - - - TARGET RESET - - - - - - - - - - - - - - - - -
    ***** Booting Zephyr OS build zephyr-v2.0.0-882-g89a984e64424 *****
    
    spi_slave_thread
    
    SPI Slave example application
    
    rx_data buffer at 0x20000b90
    
    spi_slave_init: slave config @ 0x20000b94: wordsize(8), mode(0/0/0)
    
    spi_slave_init: SPI pin config -- MOSI(P0.12), MISO(P0.27), SCK(P0.23), CS(P0.13)
    
    Received: 0x1234 -- correct
    
    Received: 0x1234 -- correct

```

