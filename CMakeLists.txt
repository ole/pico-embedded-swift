# Call CMake with a Swift toolchain that supports Embedded Swift:
#
# mkdir build
# cd build
# env TOOLCHAINS=org.swift.59202312071a cmake -G Ninja ..

cmake_minimum_required(VERSION 3.28)

# include(cmake/swift-embedded/swift-embedded.cmake)
# Use nightly Swift compiler, configured for Embedded Swift
set(CMAKE_Swift_COMPILER
    /Library/Developer/Toolchains/swift-DEVELOPMENT-SNAPSHOT-2023-12-07-a.xctoolchain/usr/bin/swiftc
)
# WMO is always required for Embedded Swift
set(CMAKE_Swift_FLAGS
    "-DHalloWelt -enable-experimental-feature Embedded -wmo -target armv6-none-none-eabi"
)
# Disable CMAKE Swift compiler check, which tries to compile a simple Swift
# program that includes `print()`, which isn't available in Embedded Swift.
set(CMAKE_Swift_COMPILER_FORCED TRUE)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(blink-with-Swift LANGUAGES C CXX Swift)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project

list(APPEND CMAKE_FIND_ROOT_PATH /Library/Developer/Toolchains/swift-DEVELOPMENT-SNAPSHOT-2023-12-07-a.xctoolchain/usr)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

# Imported object library
# add_library(swiftlib OBJECT IMPORTED)

# set_property(TARGET swiftlib PROPERTY 
#     IMPORTED_OBJECTS swiftlib.o
# )

add_subdirectory(swiftlib)

add_executable(blink
    blink.c
    # $<TARGET_OBJECTS:swiftlib>
)

target_link_libraries(
    blink
    pico_stdlib
    $<TARGET_OBJECTS:swiftlib>
)

# create map/bin/hex file etc.
pico_add_extra_outputs(blink)
