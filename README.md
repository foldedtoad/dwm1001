# DWM1001 and Zephyr
This project contains examples on how to use the Ultra Wideband (UWB) and Bluetooth hardware based DWM1001 module together with Zephyr RTOS. It's an adaptation of Decawave's examples distributed along with their driver. 

This project is a clone of the Decawave's Zephyr project, in response to the original code not being buildable with the latest versions of Zephyr. 

This project assumes some familiarity with Zephyr.  Zephyr is not too difficult to install and learn, but there are good tutorials available which explain how to establish a working version of Zephyr on your development system.

The major changes from the original Decawave project are:

* Change to using a custom board profile. This allows the nrf52_dwm1001 board to be defined within this project, thereby eliminating the need for adding the board defintion in the base Zephyr sources.

* Change hardware-related reference to Device Tree Symbolics (DTS). Some DTS symbols were already being used, but there were a few cases which needed to be converted.

* The Bluetooth example has been completely replaced with a new example. The original BLE support used what appeared to be a BLE-SIG defined UUID, named `dps`, a 16-bit UUID, which is no longer available in Zephyr.
This example uses a custom 128-bit UUID for both the DWM1001 service and characteristics. This serves as a better starting-point example, as most developers will interested in custom BLE services/charactersistics. 

* The original code had comment lines which extended well past 80 columns.  This is very inconvienent for development within VMs on laptops where screen real-estate limited. So the code was reformatted to 80-column max lines.  It's just easier to read and understand: that is the point of examples, right?!

## Getting Started

## What's required?
### OS
Linux, Mac or Windows

This project was developed in a VirtualBox VM running Ubuntu 18.04 (LTS), but there is no reason these changes should work with the other OSes.

### Hardware
You will need at least one `DWM1001-dev` board and a `micro-USB` cable.  
Many of the examples will require two boards or more, such as the micro-location examples.

NOTE: Because the DWM1001 board incorporates a Segger JLink debugger (on-board), it is highly recommended to install the Segger JLink package on your development system: it's free, and provides support for gdb, pyocd, and Ozone (Segger's debugger).

Because this board incorporates JLink support, Segger's RTT console support is used for logging.  This eliminates the need to configure and run a seperated UART-based console when developing.

### Board Support (new)
A major feature of this project is the defining of the DWM1001 board as part of this project. The original project from which this project was cloned, required the use of a special version of the Zephyr project which had the DWM1001 board definitions inserted into the base Zephyr project. Later versions of Zephyr now have a method for defining custom boards within a project, eliminating the need to modify (and maintain) Zephyr itself.

Under this project's root directory, there is a the following file tree structure: 

```
boards/
└── arm
    └── nrf52_dwm1001
        ├── board.cmake
        ├── doc
        ├── Kconfig
        ├── Kconfig.board
        ├── Kconfig.defconfig
        ├── nrf52_dwm1001_defconfig
        ├── nrf52_dwm1001.dts
        └── nrf52_dwm1001.yaml
```
In each example sub-project, the CMakeList.txt file has been updated with the following statement. This statement merges the custom board definition into the Zephyr board configuration process. 

```
 set(BOARD_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../..")
```

### Software
There's quite a lot to install if you haven't already. First we're going to build the firmware, after which we can flash it on the board.

#### Building
Follow the instructions from Zephyr [here](https://docs.zephyrproject.org/latest/getting_started/index.html#set-up-a-development-system).

NOTE: The toolchain is now provided in the latest version of Zephyr, so you will not need to install or build them yourself.
This provides build-consistency across Zephyr projects.

This project was developed using only `cmake`, not `west` or `ninja`, but you should be able to use them if you prefer.

#### Flashing
There are two ways to flash one of the example project's firmware onto a DWM1001 board.
* Use the debugger (gdb, Ozone, etc) to flash.
* Use the Nordic-provided `nrfjprog` utility via running make: `make flash`

In order to flash the boards with `nrfjprog` you will need to install `nrfjprog`. This tool is also available on all 3 main OS's. You can find it [here](https://www.nordicsemi.com/Software-and-Tools/Development-Tools/nRF5-Command-Line-Tools). After installing, make sure that your system PATH contains the path to where it is installed.

After installing `nrfjprog`, quickest way is probably using `make flash`. 
But note, this does not allow you to see the console messages.
If you are developing a new project or modifying an existig project, the you will probably be using an debugger with some form of graphical interface. 

### Zephyr Environment Variables
Now change your active directory:
```
    cd zephyr
```

Now source the script zephyr-env.sh (linux & macOS) or run zephyr-env.cmd to make sure all the environment variables are set correctly.

### Build Your First Application
The github repository is the one that contains the specific DWM1001 example code.
Download or clone [this](https://github.com/foldedtoad/dwm1001) repository to your local computer:
```
    git clone https://github.com/foldedtoad/dwm1001.git
```
NOTE: The original code can be found [here](https://github.com/RT-LOC/zephyr-dwm1001)

Start building the real examples from the project.  
NOTE: The procedure is identical for all examples.  

We will proceed with building the first simple example: `./examples/ex_01a_simple_tx/`.
```
    cd examples/ex_01a_simple_tx
```
Configure the build system with `cmake` as follows:
```
    cmake -B build -DBOARD=nrf52_dwm1001 .
```
NOTE: You will need to re-do the above step whenever new "C"-type files are added to your project.  The Zephyr cmake support is good at detecting changes in existing files, but doesn't detect newly added files; thus the need to run the above again.

NOTE: Sometimes you might get error messages from the above configuration procedure. If so, delete the whole build directory and try again; often there are residual files in an existing build directory which appear to collide with the new configuration definitions. 

OPTIONAL: If you are developing on a Linux or OSX system, then you may use the script `configure.sh`, which does the same operation.

And we actually build or firmware with `make`:
```
    cd build
    make
```

### Flash
Now let's flash the binary file that we just built onto the board. 
Make sure you have nrfjprog properly installed and that it is in the system PATH.

#### Program the binary file on the board:
```
make flash
```

### Console Messages (JLink RTT Console)
If you are developing on a Linux or OSX system and have installled the JLink package, then you can use the `rtt.sh` script (in the root directory) to start console instance.  Something like the `rtt.sh` script may be possible on Windows, but it has not be tried.  Be sure to follow the directions displayed when `rtt.sh` starts: `h`, `r`, `g` in the JLinkExe shell.

If you have RTT message support and started, then you should see the following

```
***** Booting Zephyr OS build zehpher-V2.0.0-882-g89a984e64424 *****
main_thread
SIMPLE TX 13
device_id: deca0130
```


## Examples
The following examples are provided (checkbox checked if all functionality of the example is fully functional):
 - Example 1 - transmission
  - [ ] ex_01a_simple_tx
  - [ ] ex_01b_tx_sleep
  - [ ] ex_01c_tx_sleep_auto
  - [ ] ex_01d_tx_timed_sleep
  - [ ] ex_01e_tx_with_cca
 - Example 2 - reception
  - [ ] ex_02a_simple_rx
  - [ ] ex_02b_rx_preamble_64
  - [ ] ex_02c_rx_diagnostics
  - [ ] ex_02d_rx_sniff
  - [ ] ex_02e_rx_dbl_buff
 - Example 3 - transmission + wait for response
  - [ ] ex_03a_tx_wait_resp
  - [ ] ex_03b_rx_send_resp
  - [ ] ex_03c_tx_wait_resp_leds
  - [ ] ex_03d_tx_wait_resp_interrupts
 - Example 4 - continuous transmission
  - [ ] ex_04a_cont_wave
  - [ ] ex_04b_cont_frame
 - Example 5 - double-sided two-way ranging
  - [ ] ex_05a_ds_twr_init
  - [ ] ex_05b_ds_twr_resp
  - [ ] ex_05c_ds_twr_resp_ble
 - Example 6 - single-sided two-way ranging
  - [ ] ex_06a_ss_twr_init
  - [ ] ex_06b_ss_twr_resp
 - Example 7 - acknownledgements
  - [ ] ex_07a_ack_data_tx
  - [ ] ex_07b_ack_data_rx
 - Example 8 - low power listen
  - [ ] ex_08a_low_power_listen_rx
  - [ ] ex_08b_low_power_listen_tx
 - Example 9 - bandwidth power
  - [ ] ex_09a_bandwidth_power_ref_meas
  - [ ] ex_09b_bandwidth_power_comp
 - Example 10 - GPIO
  - [ ] ex_10a_gpio
 - Example 11 - IO
  - [ ] ex_11a_button
  - [ ] ex_11b_leds
 - Example 12 - BLE
  - [ ] ex_12a_ble 
 - Example 13 - Acccelerometer
  - [ ] ex_13a_accelerometer  

## What's next?
* Examples completion
* (Mobile) readout app (alternative: use Nordic `nrf-connect` app (iOS and Android)
* Add DTS/Bindings structure to allow non-Zephyr-included drivers to be accessed from custom app
* Create a VirtualBox VM image which contains a turn-key Zephyr + DWM1001 development system.
