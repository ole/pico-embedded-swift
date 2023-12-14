// SwiftLib’s public C API. The main C program uses this to call into SwiftLib.

#include <inttypes.h>
#include <stdbool.h>

extern uint32_t swiftlib_ledOnDuration();
extern uint32_t swiftlib_ledOffDuration();
extern void swiftlib_gpioSet(int32_t pin, bool is_high);