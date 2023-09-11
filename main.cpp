#include <iostream>
#include "bitmask.hpp"
#include "cregex.hpp"

using namespace std;

//*
void TestMatch(RegexState*reg, const char* str)
{
    RegexMatch match = Regexp_Match(reg, str);
    printf("Match of '%s' and regexp: ", str);
    if (match.success)
    {
        printf("true\n\n");
    }
    else
    {
        printf("false\n\n");
    }
}
//*/

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

int main()
{
    //*
    RegexState*reg = Regexp_Parse("abc?x..");

    TestMatch(reg, "abcxyz");
    TestMatch(reg, "abcxy");
    TestMatch(reg, "abcx");
    TestMatch(reg, "abxyz");
    TestMatch(reg, "abdxyz");
    TestMatch(reg, "bddrcg");
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
