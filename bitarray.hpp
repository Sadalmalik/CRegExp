#ifndef BITARRAY
#define BITARRAY

#include <stdbool.h>
#include <stdint.h>

namespace Bitarray
{
    typedef struct BitArray {
        int size;
        int8_t bytes[0];
    } BitArray;

    size_t      SizeOfBits(int size);
    BitArray*   Create(int size);
    void        Destroy(BitArray* mask);
    void        SetBit(BitArray* mask, int bit, bool value);
    bool        GetBit(BitArray* mask, int bit);
}

#endif
