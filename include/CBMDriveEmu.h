#ifndef CBMDRIVEEMU_H
#define CBMDRIVEEMU_H

#include <cstdint>
#include <string>

#ifndef NDEBUG
extern int printf(const char *fmt, ...);
#define printdf(fmt, ...) printf("[%s.%d] >> " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define printdf(fmt, ...)
#endif

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
    long int baud;
};

#endif /* CBMDRIVEEMU_H */
