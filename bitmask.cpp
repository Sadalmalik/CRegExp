#include "bitmask.hpp"

#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

size_t Bitmask_SizeOfBits(int size)
{
    return size / 8 + ((size%8) > 0);
}

BitMask* Bitmask_Create(int size)
{
    int byteLen = Bitmask_SizeOfBits(size);
    BitMask* mask = (BitMask*) malloc(sizeof(BitMask) + byteLen);
    mask->size = size;
    return mask;
}

void Bitmask_Destroy(BitMask* mask)
{
    free(mask);
}

void Bitmask_SetBit(BitMask*mask, int bit, bool value)
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

bool Bitmask_GetBit(BitMask*mask, int bit)
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
