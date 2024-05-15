cmake_minimum_required(VERSION 3.20.0)

set(BOARD_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../..")
set(DTS_ROOT   "${CMAKE_CURRENT_SOURCE_DIR}/../..")
set(BOARD nrf52_dwm1001)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(spi_slave)

target_sources(app PRIVATE main.c)
target_sources(app PRIVATE spi_master.c)

target_include_directories(app PRIVATE ./)

zephyr_compile_options(-save-temps)
