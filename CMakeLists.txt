cmake_minimum_required(VERSION 3.28)

# Path to Raspberry Pi Pico C/C++ SDK
#
# This assumes the SDK is cloned to a sibling directory of this project.
# Alternatively, you can set PICO_SDK_PATH as an environment variable
# in your shell or pass the path directly to cmake:
#
#     cmake -DPICO_SDK_PATH="/path/to/pico-sdk"`
if (NOT (DEFINED ENV{PICO_SDK_PATH}) AND (NOT PICO_SDK_PATH))
    set(PICO_SDK_PATH "${CMAKE_CURRENT_LIST_DIR}/../pico-sdk")
    message("PICO_SDK_PATH is not set, using value from CMakeLists.txt ('${PICO_SDK_PATH}')")
endif()

# Initialize the SDK based on PICO_SDK_PATH
# Note: this must happen before `project()`.
#
# This include sets PICO_SDK_PATH and then includes 
# PICO_SDK_PATH/pico_sdk_init.cmake (Pico SDK pre-init).
include(pico_sdk_import.cmake)

# Configure Swift
#
# CMAKE_Swift_COMPILER must be set to an up-to-date nightly Swift toolchain
# with Embedded Swift support. The default value (at least on macOS and Linux)
# is /usr/bin/swiftc.
#
# You can set this either by passing the flag directly to CMake or by setting
# CMAKE_Swift_COMPILER=/path/to/swiftc as an environment variable.
#
# Example for macOS:
#
#     cmake -DCMAKE_Swift_COMPILER="/Library/Developer/Toolchains/swift-DEVELOPMENT-SNAPSHOT-2023-12-07-a.xctoolchain/usr/bin/swiftc"
if ((DEFINED ENV{CMAKE_Swift_COMPILER}) AND (NOT CMAKE_Swift_COMPILER))
    set(CMAKE_Swift_COMPILER "$ENV{CMAKE_Swift_COMPILER}")
    message("CMAKE_Swift_COMPILER: ${CMAKE_Swift_COMPILER} (using value set in environment)")
endif()
set(targetTripleLLVM "armv6m-none-none-eabi")
set(CMAKE_Swift_FLAGS
    # -wmo: Whole-module optimization is always required for Embedded Swift.
    # -Xfrontend -function-sections: enables dead stripping of unused Swift 
    #                                runtime functions.
    "-target ${targetTripleLLVM} \
    -enable-experimental-feature Embedded \
    -wmo \
    -Xfrontend -function-sections"
)
# Skip CMake’s automatic Swift compiler check
# The check would always fail for Embedded Swift because it tries to compile
# a Swift program containing `print()`, which isn't available (yet) in
# Embedded Swift.
message(STATUS "Skipping CMake's check for working Swift compiler.")
set(CMAKE_Swift_COMPILER_FORCED TRUE)

# === Begin project configuration

project(SwiftPico LANGUAGES C CXX Swift)

# Abort if the Swift compiler doesn't support the target architecture.
#
# I'd also like to test if the Swift compiler supports Embedded Swift,
# but I don't know how to do that on the command line
# (see https://forums.swift.org/t/how-to-test-if-swiftc-supports-an-upcoming-experimental-feature/69095).
#
# Note: This must happen after `project()` because we can now rely on
# the `CMAKE_Swift_COMPILER` variable not being empty.
message("CMAKE_Swift_COMPILER: ${CMAKE_Swift_COMPILER}")
execute_process(
    COMMAND "${CMAKE_Swift_COMPILER}" -target "${targetTripleLLVM}" -print-target-info
    RESULT_VARIABLE swiftcSupportsTarget
    OUTPUT_QUIET
    ERROR_QUIET
)
if (NOT(swiftcSupportsTarget EQUAL "0"))
    message(FATAL_ERROR "Swift compiler '${CMAKE_Swift_COMPILER}' doesn't support target '${targetTripleLLVM}'. Are you using an up-to-date nightly Swift toolchain that supports Embedded Swift?\nPass the path to swiftc to CMake via `cmake -DCMAKE_Swift_COMPILER=/path/to/swiftc` or by setting CMAKE_Swift_COMPILER=/path/to/swiftc in the environment.\nExample for macOS: `cmake -DCMAKE_Swift_COMPILER=\"/Library/Developer/Toolchains/swift-DEVELOPMENT-SNAPSHOT-2023-12-07-a.xctoolchain/usr/bin/swiftc\"`")
endif()

# Initialize the Raspberry Pi Pico SDK
pico_sdk_init()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

add_executable(
    SwiftPico
        main.c
)

add_subdirectory(SwiftLib)

target_link_libraries(
    SwiftPico
        pico_stdlib
        # $<TARGET_OBJECTS:> is a generator expression:
        # <https://cmake.org/cmake/help/v3.28/manual/cmake-generator-expressions.7.html#manual:cmake-generator-expressions(7)>
        #
        # Using just "SwiftLib" here results in a linker error I can't explain:
        # "unknown argument: '--specs=nosys.specs'""
        # I think this is because CMake tries to use `swiftc` as the linker in
        # this case. I don’t know why.
        $<TARGET_OBJECTS:SwiftLib>
)

# create map/bin/hex file etc.
pico_add_extra_outputs(SwiftPico)
