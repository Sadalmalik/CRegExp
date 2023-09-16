#include "bitarray.hpp"

#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

namespace Bitarray
{
    size_t SizeOfBits(int size)
    {
        return (size / 8) + ((size % 8) > 0);
    }

    BitArray* Create(int size)
    {
        size_t byteLen = SizeOfBits(size);
        BitArray* mask = (BitArray*) malloc(sizeof(BitArray) + byteLen);
        if (mask == nullptr)
            return mask;
        mask->size = size;
        return mask;
    }

    void Destroy(BitArray* mask)
    {
        free(mask);
    }

    void SetBit(BitArray* mask, int bit, bool value)
    {
        if (bit < 0 || mask->size <= bit)
        {
            printf("Bitarray::Error: Try access bit outside of bitarray!\n");
            //exit(10000);
            return;
        }
        int i = bit / 8;
        int s = bit % 8;
        if (value)
            mask->bytes[i] |= 1 << s;
        else
            mask->bytes[i] &= ~(1 << s);
    }

    bool GetBit(BitArray* mask, int bit)
    {
        if (bit < 0 || mask->size <= bit)
        {
            printf("Bitarray::Error: Try access bit outside of bitarray!\n");
            //exit(10000);
            return false;
        }
        int i = bit / 8;
        int s = bit % 8;
        return 0 != (mask->bytes[i] & (1 << s));
    }

}