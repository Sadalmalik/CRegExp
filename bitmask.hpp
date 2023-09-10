#ifndef BITMASK
#define BITMASK

#include <stdbool.h>
#include <stdint.h>

typedef struct BitMask {
    int size;
    int8_t bytes[0];
} BitMask;

size_t      Bitmask_SizeOfBits  (int size);
BitMask*    Bitmask_Create      (int size);
void        Bitmask_Destroy     (BitMask*mask);
void        Bitmask_SetBit      (BitMask*mask, int bit, bool value);
bool        Bitmask_GetBit      (BitMask*mask, int bit);

#endif
