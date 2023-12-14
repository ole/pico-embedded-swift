// === Symbols required by Swift
//
// Provide empty implementations to silence the linker.
// This is fine so long as Swift doesn’t actually call these functions.

#include <stdbool.h>
#include <stdio.h>

int posix_memalign(void **memptr, size_t alignment, size_t size) {
    printf("posix_memalign called\n");
    return -1;
}

_Bool __atomic_compare_exchange_4(volatile void *, void *, unsigned int, _Bool, int, int) {
    printf("__atomic_compare_exchange_4 called\n");
    return false;
};

unsigned int __atomic_fetch_add_4(volatile void *, unsigned int, int) {
    printf("__atomic_fetch_add_4 called\n");
    return 0;
}

unsigned int __atomic_fetch_sub_4(volatile void *, unsigned int, int) {
    printf("__atomic_fetch_sub_4 called\n");
    return 0;
}

unsigned int __atomic_load_4(const volatile void *, int) {
    printf("__atomic_load_4 called\n");
    return 0;
}

void __atomic_store_4(volatile void *, unsigned int, int) {
    printf("_store_4 called\n");
};
