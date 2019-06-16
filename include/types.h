#ifndef TYPES_H
#define TYPES_H

#include <cstdint>
#include <string>

typedef union {
    uint16_t W;
    struct {
#ifdef LOW_ENDIAN
        uint8_t h;
        uint8_t l;
#else
        uint8_t l;
        uint8_t h;
#endif
    } __attribute__((packed)) B;
} word_t;

struct config {
    struct {
        std::size_t size;
        uint8_t *data;
    } firmware;
    std::string *devPath;
    int baud;
};

#endif
