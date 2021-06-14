cmake_minimum_required(VERSION 3.12)
include(pico_sdk_import.cmake)
include(pico_extras_import.cmake)
project(main_project)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(
  multicore
  multicore.cpp
)

pico_enable_stdio_uart(multicore 1)
pico_add_extra_outputs(multicore)
target_link_libraries(multicore pico_stdlib pico_multicore)
