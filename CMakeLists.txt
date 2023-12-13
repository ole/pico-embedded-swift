# Build, flash, and run:
#
# mkdir build
# cd build
# cmake -G Ninja ..
# ninja
# probe-rs run --chip RP2040 PicoLinksSwift.elf

cmake_minimum_required(VERSION 3.28)

# User configuration
# Change these values to match your config.
# 
# 1) Path to Raspberry Pi Pico C/C++ SDK
#    This assumes SDK is cloned to a sibling directory of this project.
#    Alternatively, you can set PICO_SDK_PATH as an environment variable in your shell.
if (NOT (DEFINED ENV{PICO_SDK_PATH}) AND (NOT PICO_SDK_PATH))
    set(PICO_SDK_PATH "${CMAKE_CURRENT_LIST_DIR}/../pico-sdk")
    message("PICO_SDK_PATH is not set in environment, using value from CMakeLists.txt ('${PICO_SDK_PATH}')")
endif ()
# 2) Swift toolchain 
#    Change this to the current Swift nightly toolchain you installed.
#    The ID is in /Library/Developer/Toolchains/[TOOLCHAIN].xctoolchain/Info.plist
#    Alternatively, set the TOOLCHAINS environment variable.
if (NOT (DEFINED ENV{TOOLCHAINS}))
    set(Swift_Toolchain "org.swift.59202312071a")
    message("Swift toolchain: ('${Swift_Toolchain}') (using value from CMakeLists.txt as TOOLCHAINS is not set in environment)")
else ()
    set(Swift_Toolchain "$ENV{TOOLCHAINS}")
    message("Swift toolchain: ('${Swift_Toolchain}') (using TOOLCHAINS value from environment)")
endif ()

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

# Configure Swift. This must happen before `project()`. I don't know why.
execute_process(
    COMMAND xcrun --toolchain "${Swift_Toolchain}" --find swiftc
    OUTPUT_VARIABLE swiftc_Path
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
# Use nightly Swift compiler, configured for Embedded Swift.
set(CMAKE_Swift_COMPILER
    "${swiftc_Path}"
)
set(CMAKE_Swift_FLAGS
    # - WMO is always required for Embedded Swift.
    # - The define `-D…` isn't necessary. It's just a convenient marker for
    #   searching the build log for Swift commands.
    "-enable-experimental-feature Embedded -wmo -target armv6-none-none-eabi -Dmyswiftlib_arg"
)
# Disable CMAKE Swift compiler check. The compiler check always fails for
# Embedded Swift because it tries to compile a Swift program that includes
# `print()`, which isn't available in Embedded Swift.
set(CMAKE_Swift_COMPILER_FORCED TRUE)

project(PicoLinksSwift LANGUAGES C CXX Swift)

# Initialize the Raspberry Pi Pico SDK
pico_sdk_init()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

add_subdirectory(my-swift-lib)

add_executable(PicoLinksSwift
    main.c
)

target_link_libraries(PicoLinksSwift
    $<TARGET_OBJECTS:myswiftlib>
    pico_stdlib

    # Prints debugging information (register values and stack contents)
    # to stdout when an exception occurs. Example output:
    # 
    # ```
    # -----------------------------------------------------------
    # Unhandled Hard Fault exception on core 0
    # -----------------------------------------------------------
    #   R0: 000000c8   R1: 0000012c   R2: 00001000   R3: e0900001
    #   R4: 1000026c   R5: d0000000   R6: 02000000   R7: 00000000
    #   R8: ffffffff   R9: ffffffff  R10: ffffffff  R11: ffffffff
    #  R12: 40014000   SP: 20041fe8   LR: 10000339   PC: e0900000
    #                 MSP: 20041fe8  PSP: fffffffc XPSR: 01000000
    #          EXC_RETURN: fffffff9             PRIMASK: 00000000
    # -----------------------------------------------------------
    # 20041fe8: 00000001 1000026c 20041f01 18000000
    # 20041ff8: 00000000 1000022b                  
    # 20042008:                                    
    # 20042018:                                    
    # 20042028:                                    
    # 20042038:                                    
    # 20042048:                                    
    # 20042058:    
    # ```
    #
    # The pico_debug module is introduced in a pull request from 2022-09:
    # Print registers, stack & other details on unhandled exception
    # <https://github.com/raspberrypi/pico-sdk/pull/1026>, which hasn't been
    # merged as of 2023-12-13.
    #
    # You need to check out this branch in your PICO_SDK_PATH folder to use it.
    pico_debug
)

# create map/bin/hex file etc.
pico_add_extra_outputs(PicoLinksSwift)
