cmake_minimum_required(VERSION 3.20.0)

set(BOARD_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../..")
set(DTS_ROOT   "${CMAKE_CURRENT_SOURCE_DIR}/../..")
set(BOARD nrf52_dwm1001)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(zephyr-dwm1001)

add_definitions(-DEX_05C_DEF)

target_sources(app PRIVATE ../../main.c)
target_sources(app PRIVATE ex_05c_main.c)

target_sources(app PRIVATE ../../decadriver/deca_device.c)
target_sources(app PRIVATE ../../decadriver/deca_params_init.c)

target_sources(app PRIVATE ../../platform/port.c)
target_sources(app PRIVATE ../../platform/deca_mutex.c)
target_sources(app PRIVATE ../../platform/deca_range_tables.c)
target_sources(app PRIVATE ../../platform/deca_sleep.c)
target_sources(app PRIVATE ../../platform/deca_spi.c)
target_sources(app PRIVATE ../../platform/port.c)

target_sources(app PRIVATE ../../ble/ble_device.c)
target_sources(app PRIVATE ../../ble/ble_base.c)
target_sources(app PRIVATE ../../ble/ble_service.c)

target_include_directories(app PRIVATE ../../decadriver/)
target_include_directories(app PRIVATE ../../platform/)
target_include_directories(app PRIVATE ../../compiler/)
target_include_directories(app PRIVATE ../../ble/)

zephyr_library_include_directories($ENV{ZEPHYR_BASE}/include/bluetooth)
zephyr_library_include_directories($ENV{ZEPHYR_BASE}/include/bluetooth/services/)
 