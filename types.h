#ifndef TYPES_H
#define TYPES_H

#include <cstdint>

typedef union {
    uint16_t W;
    struct {
#ifdef LOW_ENDIAN
        uint8_t l;
        uint8_t h;
#else
        uint8_t h;
        uint8_t l;
#endif
    } __attribute__((packed)) B;
} word_t;

#endif
