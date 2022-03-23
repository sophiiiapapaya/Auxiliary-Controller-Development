# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# initalize pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
include(pico_sdk_import.cmake)

project(Auxiliary_Controller C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(Auxiliary_Controller Auxiliary_Controller.c )

pico_set_program_name(Auxiliary_Controller "Auxiliary_Controller")
pico_set_program_version(Auxiliary_Controller "0.1")

pico_enable_stdio_uart(Auxiliary_Controller 1)
pico_enable_stdio_usb(Auxiliary_Controller 1)

# Add the standard library to the build
target_link_libraries(Auxiliary_Controller pico_stdlib)

# Add any user requested libraries
target_link_libraries(Auxiliary_Controller
        hardware_pio
        hardware_timer
        hardware_watchdog
        hardware_clocks
	hardware_pwm
        )

pico_add_extra_outputs(Auxiliary_Controller)

