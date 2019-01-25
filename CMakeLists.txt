cmake_minimum_required(VERSION 2.8)

set(CMAKE_SYSTEM_NAME Generic) # Bare metal system
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
set(CMAKE_OSX_SYSROOT "")
set(CMAKE_VERBOSE_MAKEFILE "ON")

set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m7") # Set cpu version
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfpu=fpv5-sp-d16") # Set FPU Version
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfloat-abi=softfp") # float-abi describes floating point interface. hard can't link with soft compiled targets.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mtune=cortex-m7") # tune for cpu
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os") # Optimize for size (same as -O2 without optimizations that impact size)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffunction-sections") # Split functions in their own section (better for --gc-sections)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fdata-sections") # Split data in their own section (better for --gc-sections)

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}") # Copy C flags over to CXX
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti") # Disable C++ runtime type information
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions") # Disable C++ Exceptions

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g3")
endif ()

include_directories(
    "CMSIS/DEVICE/ST/STM32F7xx/Include"
    "CMSIS/INCLUDE"
    "../mbedtls-2.16.0/include"
    ".")

project(stm32_bootloader.elf)
add_executable(${PROJECT_NAME} "main.cpp" "startup/startup.cpp" "Hw/factory.cpp" "Hw/interrupt.cpp" "Bootloader/bootloader.cpp" "Bootloader/loader.cpp")

set(LINKER_SCRIPT "-T ${CMAKE_SOURCE_DIR}/gcc.ld")

set_property(TARGET ${PROJECT_NAME} PROPERTY LINK_FLAGS "--specs=nano.specs --specs=nosys.specs -Wl,--gc-sections -Wl,-Map=output.map ${LINKER_SCRIPT}")
set_property(TARGET ${PROJECT_NAME} PROPERTY COMPILE_FLAGS "-Wall -Wextra -DSTM32F746xx")
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
