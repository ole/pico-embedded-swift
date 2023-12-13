#include "pico/stdlib.h"
#include "my-swift-lib/my-swift-lib.h"
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
    // Call into Swift for calculation
    int32_t delay = myswiftlib_add(200, 300);
    printf("main loop start\n");
    while (true)
    {
        printf("loop tick\n");
        gpio_put(LED_PIN, 1);
        sleep_ms(delay);
        gpio_put(LED_PIN, 0);
        sleep_ms(delay);
    }
#endif
}

// === Symbols required by Swift. Dummy implementations to silence the linker.

int posix_memalign(void **memptr, size_t alignment, size_t size)
{
    printf("posix_memalign called\n");
    return -1;
}

_Bool __atomic_compare_exchange_4(volatile void *, void *, unsigned int, _Bool, int, int)
{
    printf("__atomic_compare_exchange_4 called\n");
    return false;
};

unsigned int __atomic_fetch_add_4(volatile void *, unsigned int, int)
{
    printf("__atomic_fetch_add_4 called\n");
    return 0;
}

unsigned int __atomic_fetch_sub_4(volatile void *, unsigned int, int)
{
    printf("__atomic_fetch_sub_4 called\n");
    return 0;
}

unsigned int __atomic_load_4(const volatile void *, int)
{
    printf("__atomic_load_4 called\n");
    return 0;
}

void __atomic_store_4(volatile void *, unsigned int, int)
{
    printf("_store_4 called\n");
};