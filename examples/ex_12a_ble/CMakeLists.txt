cmake_minimum_required(VERSION 3.13.1)

set(BOARD_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../..")
set(BOARD nrf52_dwm1001)

include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(zephyr-dwm1001)

target_sources(app PRIVATE ../../main.c)
target_sources(app PRIVATE ex_12a_main.c)

target_sources(app PRIVATE ../../platform/port.c)

target_sources(app PRIVATE ../../ble/ble_device.c)
target_sources(app PRIVATE ../../ble/ble_base.c)
target_sources(app PRIVATE ../../ble/ble_service.c)

target_include_directories(app PRIVATE ../../decadriver/)
target_include_directories(app PRIVATE ../../platform/)
target_include_directories(app PRIVATE ../../compiler/)
target_include_directories(app PRIVATE ../../ble/)

zephyr_library_include_directories($ENV{ZEPHYR_BASE}/samples/bluetooth)
zephyr_library_include_directories($ENV{ZEPHYR_BASE}/samples/bluetooth/gatt/)
