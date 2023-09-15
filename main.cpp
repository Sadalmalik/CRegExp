#include <iostream>
#include <string>

#include "bitmask.hpp"
#include "cregex.hpp"

using namespace std;

/*
void TestMatch(TRegex*reg, std::string str)
{

    RegexpMatch match = Regexp_Match(reg, str);
    printf("Match of '%s' and regex: ", str.c_str());
    if (match.success)
    {
        printf("true\n\n");
    }
    else
    {
        printf("false\n\n");
    }
}
//* /
void TestSetBit(BitMask*mask, int bit)
{
    printf("Try set %d bit\n", bit);
    Bitmask_SetBit(mask, bit, true);
}

void TestGetBit(BitMask*mask, int bit)
{
    bool val = Bitmask_GetBit(mask, bit);
    if (val) printf("bit[%d] = true\n", bit);
    else     printf("bit[%d] = false\n", bit);
}
//*/

void TestParse(std::string str)
{
    LiRex::TRegex*reg = LiRex::Create(str);
    printf("regex '%s':\n", reg->raw.c_str());
    LiRex::Dump(reg);
    LiRex::Destroy(reg);
    printf("\n");

}

int main()
{
    //*
    //RegexState*reg;

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

    /*/

    BitMask *mask  = Bitmask_Create(256);

    printf("Set bits:\n");
    TestSetBit(mask, 0);
    TestSetBit(mask, 17);
    TestSetBit(mask, 230);
    TestSetBit(mask, 255);
    TestSetBit(mask, 290);
    TestSetBit(mask, -15);
    printf("\n");

    printf("Get bits:\n");
    TestGetBit(mask, -25);
    TestGetBit(mask, -15);
    TestGetBit(mask, -1);
    TestGetBit(mask, 0);
    TestGetBit(mask, 1);
    TestGetBit(mask, 229);
    TestGetBit(mask, 230);
    TestGetBit(mask, 231);
    TestGetBit(mask, 254);
    TestGetBit(mask, 255);
    TestGetBit(mask, 256);
    TestGetBit(mask, 290);
    printf("\n");

    Bitmask_Destroy(mask);
    //*/
    printf("Done!\n");

    return 0;
}
