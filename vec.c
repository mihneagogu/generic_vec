#include "vec.h"

static uint32_t next_pow_of_two(uint32_t v) {
    uint32_t n = v;
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}

