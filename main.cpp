#include <iostream>
#include "cregex.h"

using namespace std;

void TestMatch(RegexState*reg, const char* str)
{
    bool match = Match(reg, str);
    cout << "Test match of " << str << " and regexp: " << match << endl;
}

int main()
{
    InitRegex(100);
    RegexState*reg = ParseRegexp("abc?x..");

    TestMatch(reg, "abcxyz");
    TestMatch(reg, "abcxy");
    TestMatch(reg, "abcx");
    TestMatch(reg, "abxyz");
    TestMatch(reg, "abdxyz");
    TestMatch(reg, "bddrcg");

    cout << "Done!" << endl;

    return 0;
}
