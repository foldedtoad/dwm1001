cmake_minimum_required(VERSION 3.13.1)

set(BOARD_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../..")
set(BOARD nrf52_dwm1001)

include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(spi_slave)

target_sources(app PRIVATE main.c)
target_sources(app PRIVATE spi_slave.c)

target_include_directories(app PRIVATE ./)
