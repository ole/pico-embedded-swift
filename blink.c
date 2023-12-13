#include "pico/stdlib.h"
#include "swiftlib/swiftlib.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{
    stdio_init_all();
    printf("main() start\n");
#ifndef PICO_DEFAULT_LED_PIN
#warning blink example requires a board with a regular LED
#else
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    int32_t a = 200;
    int32_t delay = add(a, a);
    printf("main loop start\n");
    while (true)
    {
        gpio_put(LED_PIN, 1);
        sleep_ms(delay);
        gpio_put(LED_PIN, 0);
        sleep_ms(delay);
    }
#endif
}

// Dummy implementation to silence the linker.
int posix_memalign(void **memptr, size_t alignment, size_t size)
{
    printf("posix_memalign called\n");
    return -1;
    // *memptr = malloc(size);
    // return 0;
}
