DWM1001 - ex_13a_main
#########################

Overview
********
This example shows how to read the DWM1001's LIS2DH accelerometer.
It uses a simple polling method, but could be changed to support 
triggers (interrupts): QED.

Requirements
************
Segger JLink support, including their RTT Client utility.
After installing JLink, you can run the rtt.sh script to see the 
output.

Building and Running
********************
Same as the other DWM1001 examples.


Sample Output
=============
Below is a sample of the output from the accelerometer.
Note that the displayed values are in g-units; so that, for example, 
the z-axis show about 9.6g which is earth's gravity.
Sampling is at a 1Hz rate.

::
  
  ***** Booting Zephyr OS build zephyr-v2.0.0-882-g89a984e64424 *****

  accel_thread

  [00:00:02.678,405] <inf> accel: x=-1.885376g  y=-1.617920g  z=9.704832g
  [00:00:03.686,401] <inf> accel: x=-1.923584g  y=-1.617920g  z=9.590208g
  [00:00:04.693,115] <inf> accel: x=-1.961792g  y=-1.617920g  z=9.628416g
  [00:00:05.699,890] <inf> accel: x=-1.923584g  y=-1.579712g  z=9.666624g
  [00:00:06.706,604] <inf> accel: x=-1.961792g  y=-1.617920g  z=9.666624g
  [00:00:07.713,317] <inf> accel: x=-1.885376g  y=-1.617920g  z=9.666624g
  [00:00:08.720,031] <inf> accel: x=-1.923584g  y=-1.656128g  z=9.666624g
  [00:00:09.726,745] <inf> accel: x=-1.923584g  y=-1.579712g  z=9.590208g
  [00:00:10.733,489] <inf> accel: x=-1.923584g  y=-1.656128g  z=9.666624g

