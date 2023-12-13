# Use nightly Swift compiler, configured for Embedded Swift
# set(CMAKE_Swift_COMPILER
#     /Library/Developer/Toolchains/swift-DEVELOPMENT-SNAPSHOT-2023-12-07-a.xctoolchain/usr/bin/swiftc
# )
# set(CMAKE_Swift_FLAGS
#     -enable-experimental-feature Embedded
#     # WMO is always required for Embedded Swift
#     -wmo
#     -target armv6-none-none-eabi
# )
# Disable CMAKE Swift compiler check, which tries to compile a simple Swift
# program that includes `print()`, which isn't available in Embedded Swift.
# set(CMAKE_Swift_COMPILER_FORCED TRUE)

add_library(swiftlib lib.swift)
