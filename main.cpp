#include <iostream>
#include <string>
#include <stdio.h>

#include "bitarray.hpp"
#include "light_regexp.hpp"


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

void TestMatch(LiRex::TRegex* regexp, std::string text)
{
    LiRex::TMatch m = LiRex::Match(regexp, text);
    if (m.success)
    {
        int len = (int) m.groups.size();
        std::printf("Match '%s' -> Success\n  Match: '%s'\n  Groups %d:\n", text.c_str(), m.match.c_str(), len);
        for (int i=0;i<len;i++)
            std::printf("    %d: '%s'\n", i, m.groups[i].c_str());
    }
    else
    {
        std::printf("Match '%s' -> Fail\n", text.c_str());
    }
}

void TestRegExp()
{
    printf("Regexp tests.\n\n");

    printf("Test parse.\n");
    TestParse("a");
    TestParse("a?");
    TestParse("a*");
    TestParse("abc");
    TestParse("abc?");
    TestParse("abc?x..");
    TestParse("a(bc)de");
    TestParse("a(bc)?d");
    TestParse("a(bcx)*ef?t");
    printf("\n");

    std::string raw_regex = "a(bc)*de*f";
    LiRex::TRegex* reg = LiRex::Create(raw_regex);
    printf("Test regex: '%s'\n", raw_regex.c_str());
    TestMatch(reg, "a");
    TestMatch(reg, "adf");
    TestMatch(reg, "adef");
    TestMatch(reg, "abdf");
    TestMatch(reg, "bcdf");
    TestMatch(reg, "abdf");
    TestMatch(reg, "abcdf");
    TestMatch(reg, "abcdef");
    TestMatch(reg, "abcbcdf");
    TestMatch(reg, "adeeef");
    TestMatch(reg, "adeeeff");
    TestMatch(reg, "areeeff");
    LiRex::Destroy(reg);
    printf("\n");

    raw_regex = "a(bc)*de*f";
    reg = LiRex::Create(raw_regex);
    printf("Test regex: '%s'\n", raw_regex.c_str());
    TestMatch(reg, "areeeff");
    LiRex::Destroy(reg);
    printf("\n");

    printf("Regexp tests done.\n\n\n");
}