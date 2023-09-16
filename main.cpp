#include <iostream>
#include <string>
#include <stdio.h>

#include "bitarray.hpp"
#include "cregex.hpp"


void TestBitArray();
void TestRegExp();

int main()
{
    TestBitArray();
    TestRegExp();

    return 0;
}


//----------------------------------------------------------------------------//
// Bitmask tests

void TestSetBit(Bitarray::BitArray* mask, int bit)
{
    std::printf("Try set %d bit\n", bit);
    Bitarray::SetBit(mask, bit, true);
}

void TestGetBit(Bitarray::BitArray* mask, int bit)
{
    bool val = Bitarray::GetBit(mask, bit);
    if (val) std::printf("bit[%d] = true\n", bit);
    else     std::printf("bit[%d] = false\n", bit);
}

void TestBitArray()
{
    printf("Bitmask tests.\n\n");
    Bitarray::BitArray* bitArray = Bitarray::Create(256);

    printf("Set bits:\n");
    TestSetBit(bitArray, 0);
    TestSetBit(bitArray, 17);
    TestSetBit(bitArray, 230);
    TestSetBit(bitArray, 255);
    TestSetBit(bitArray, 290);
    TestSetBit(bitArray, -15);
    printf("\n");

    printf("Get bits:\n");
    TestGetBit(bitArray, -25);
    TestGetBit(bitArray, -15);
    TestGetBit(bitArray, -1);
    TestGetBit(bitArray, 0);
    TestGetBit(bitArray, 1);
    TestGetBit(bitArray, 229);
    TestGetBit(bitArray, 230);
    TestGetBit(bitArray, 231);
    TestGetBit(bitArray, 254);
    TestGetBit(bitArray, 255);
    TestGetBit(bitArray, 256);
    TestGetBit(bitArray, 290);
    printf("\n");

    Bitarray::Destroy(bitArray);
    printf("Bitmask tests done.\n\n\n");
}

//----------------------------------------------------------------------------//
// Regexp tests

void TestParse(std::string str)
{
    LiRex::TRegex* reg = LiRex::Create(str);
    LiRex::Dump(reg);
    LiRex::Destroy(reg);
    std::printf("\n");

}

void TestRegExp()
{
    TestParse("a");
    TestParse("a?");
    TestParse("a*");
    TestParse("abc");
    TestParse("abc?");
    TestParse("abc?x..");
    TestParse("a(bc)de");
    TestParse("a(bc)?d");
    TestParse("a(bcx)*ef?t");

    /*
    TestMatch(reg, "abcxyz");
    TestMatch(reg, "abcxy");
    TestMatch(reg, "abcx");
    TestMatch(reg, "abxyz");
    TestMatch(reg, "abdxyz");
    TestMatch(reg, "bddrcg");
    //*/
}