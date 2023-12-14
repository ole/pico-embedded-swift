// SIO = the RP2040’s single-cycle I/O block.
// Reference documentation: https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf#tab-registerlist_sio
let SIO_BASE: UInt = 0xd0000000
let SIO_GPIO_OUT_SET_OFFSET: Int = 0x00000014
let SIO_GPIO_OUT_CLR_OFFSET: Int = 0x00000018

@_cdecl("swiftlib_ledOnDuration")
public func ledOnDuration() -> UInt32 {
    200
}

@_cdecl("swiftlib_ledOffDuration")
public func ledOffDuration() -> UInt32 {
    400
}

/// Drive a GPIO output pin high or low.
@_cdecl("swiftlib_gpioSet")
public func gpioSet(pin: Int32, high: CBool) {
    let mask: UInt32 = 1 << pin
    let sioBasePtr = UnsafeMutableRawPointer(bitPattern: SIO_BASE)!
    if high {
        sioBasePtr.storeBytes(of: mask, toByteOffset: SIO_GPIO_OUT_SET_OFFSET, as: UInt32.self)
    } else {
        sioBasePtr.storeBytes(of: mask, toByteOffset: SIO_GPIO_OUT_CLR_OFFSET, as: UInt32.self)
    }
}
