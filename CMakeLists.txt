cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(ssd1306pico C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()
add_executable(ssd1306pico
  main.c
  i2cSend/i2cSend.c
  digitRenderer/digitRenderer.c
)
pico_enable_stdio_usb(ssd1306pico 1)
#pico_enable_stdio_uart(ssd1306pico 1)
pico_add_extra_outputs(ssd1306pico)
target_link_libraries(ssd1306pico pico_stdlib hardware_i2c hardware_adc)