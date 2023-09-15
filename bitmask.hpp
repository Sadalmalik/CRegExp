#ifndef BITMASK
#define BITMASK

#include <stdbool.h>
#include <stdint.h>

namespace bitmask
{
    typedef struct BitMask {
        int size;
        int8_t bytes[0];
    } BitMask;

    size_t      SizeOfBits(int size);
    BitMask*    Create(int size);
    void        Destroy(BitMask* mask);
    void        SetBit(BitMask* mask, int bit, bool value);
    bool        GetBit(BitMask* mask, int bit);
}

#endif
