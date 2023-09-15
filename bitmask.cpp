#include "bitmask.hpp"

#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

namespace bitmask
{
    size_t SizeOfBits(int size)
    {
        return size / 8 + ((size % 8) > 0);
    }

    BitMask* Create(int size)
    {
        size_t byteLen = SizeOfBits(size);
        BitMask* mask = (BitMask*) malloc(sizeof(BitMask) + byteLen);
        if (mask == nullptr)
            return mask;
        mask->size = size;
        return mask;
    }

    void Destroy(BitMask* mask)
    {
        free(mask);
    }

    void SetBit(BitMask* mask, int bit, bool value)
    {
        if (bit < 0 || mask->size <= bit)
        {
            printf("Try access bit outside of bitmask!\n");
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

    bool GetBit(BitMask* mask, int bit)
    {
        if (bit < 0 || mask->size <= bit)
        {
            printf("Try access bit outside of bitmask!\n");
            //exit(10000);
            return false;
        }
        int i = bit / 8;
        int s = bit % 8;
        return 0 != (mask->bytes[i] & (1 << s));
    }

}