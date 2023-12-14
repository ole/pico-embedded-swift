#include "pico/stdlib.h"
#include "SwiftLib/SwiftLib.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{
    stdio_init_all();
    printf("main() start\n");
#ifndef PICO_DEFAULT_LED_PIN
#warning Program requires a board with a regular LED (e.g. *not* the Pico W as its LED is wired to the Wi-Fi chip)
#else
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    printf("main loop start\n");
    while (true)
    {
        printf("loop tick\n");
        // Call into SwiftLib
        swiftlib_gpioSet(LED_PIN, true);
        sleep_ms(swiftlib_ledOnDuration());
        swiftlib_gpioSet(LED_PIN, false);
        sleep_ms(swiftlib_ledOffDuration());
    }
#endif
}
