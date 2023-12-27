# Docker image for building Embedded Swift programs on Linux
#
# Build this image:
#
#     docker build --tag ole/embedded-swift .
#
# Start a container:
#
#     docker run --rm --privileged --interactive --tty --volume "$(pwd):/src" --workdir /src ole/embedded-swift

FROM swiftlang/swift:nightly-jammy

# Install dependencies
RUN apt -y update
RUN apt -y install gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib ninja-build python3

# Build CMake from source because we need a newer version than what
# Ubuntu 22.04 ships with.
WORKDIR /root
RUN apt install -y wget
RUN wget https://github.com/Kitware/CMake/releases/download/v3.28.1/cmake-3.28.1.tar.gz
RUN tar xvzf cmake-3.28.1.tar.gz
WORKDIR cmake-3.28.1
RUN mkdir build
WORKDIR build
RUN ../bootstrap --generator=Ninja -- -DCMAKE_USE_OPENSSL=OFF ..
RUN ninja
RUN ninja install

WORKDIR /
