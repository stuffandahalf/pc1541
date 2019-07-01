#ifndef CBMDRIVEEMU_H
#define CBMDRIVEEMU_H

#include <cstdint>
#include <string>

#ifndef NDEBUG
//extern int printf(const char *fmt, ...);
#include <cstdio>
#define printdf(fmt, ...) printf("[%s.%d] >> " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define printdf(fmt, ...)
#endif

/*typedef union {
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
} word_t;*/

// From https://stackoverflow.com/questions/1583791/constexpr-and-endianness
class Endian
{
private:
    static constexpr uint32_t uint32_ = 0x01020304;
    static constexpr uint8_t magic_ = (const uint8_t&)uint32_;
public:
    static constexpr bool little = magic_ == 0x04;
    static constexpr bool middle = magic_ == 0x02;
    static constexpr bool big = magic_ == 0x01;
    static_assert(little || middle || big, "Cannot determine endianness!");
private:
    Endian() = delete;
};


struct config {
    struct {
        std::size_t size;
        uint8_t *data;
    } firmware;
    std::string *devPath;
    long int baud;
};

#endif /* CBMDRIVEEMU_H */
