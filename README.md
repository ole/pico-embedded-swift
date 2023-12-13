# Embedded Swift on the Raspberry Pi Pico

A proof of concept for executing Embedded Swift code on the [Raspberry Pi Pico](https://www.raspberrypi.com/products/raspberry-pi-pico/). The main program is written in C (built with the official Pico C SDK) and calls into a statically linked Swift library.

## Prerequisites and Installation

### Hardware

- A [Raspberry Pi Pico](https://www.raspberrypi.com/products/raspberry-pi-pico/)
- Optional but recommended: a [Raspberry Pi Debug Probe](https://www.raspberrypi.com/products/debug-probe/) for more convenient flashing

### Software

- Host OS: macOS 13.x or 14.x

- A recent nightly Swift toolchain from [swift.org](https://www.swift.org/download/). Tested with the December 7, 2023 toolchain.

- A clone of the [Raspberry Pi Pico C/C++ SDK](https://github.com/raspberrypi/pico-sdk/):

  ```sh
  # This project expects to find pico-sdk in a sibling directory to itself.
  cd ..
  git clone https://github.com/raspberrypi/pico-sdk.git
  cd pico-sdk/
  git submodule update --init
  cd ..
  ```

- [CMake](https://cmake.org/) and [Ninja](https://ninja-build.org/):

  ```sh
  brew install cmake ninja
  ```

## Configuration

Open the file `CMakeLists.txt` in the root folder. Edit these two lines to match your setup:

```cmake
…
    set(PICO_SDK_PATH "${CMAKE_CURRENT_LIST_DIR}/../pico-sdk")
…
    set(Swift_Toolchain "org.swift.59202312071a")
…
```

## Building

```sh
cd pico-embedded-swift
mkdir build
cd build
cmake -G Ninja ..
ninja
```

This produces the executable `PicoLinksSwift.elf` in the `build` directory. You have two options to copy this to the Pico:

1. Via the USB Mass Storage interface: use the provided `build/elf2uf2/elf2uf2` tool to convert the `.elf` file to `.uf2`.
2. Via the debug probe. I use [probe.rs](https://probe.rs/) for this, which is a tool from the Rust community, but it works in this context too:

    ```sh
    probe-rs run --chip RP2040 PicoLinksSwift.elf
    ```
