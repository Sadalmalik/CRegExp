#ifndef REGEXP
#define REGEXP

#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string>

namespace LightRegexp
{
    typedef struct RegexpState RegexpState;
    typedef struct RegexpMatch RegexpMatch;

    struct RegexpState {
        char type;
        char match;
        RegexpState* prev;
        RegexpState* next;
        RegexpState* inner;
    };

    struct RegexpMatch
    {
        bool success;
        std::string substring;
    };

    RegexpState*    Regexp_Create   (std::string pattern);
    void            Regexp_Destroy  (RegexpState* regexp);
    RegexpMatch     Regexp_Match    (RegexpState* regexp, std::string str);
    RegexpMatch     Regexp_Find     (RegexpState* regexp, std::string str);
    void            Regexp_Dump     (RegexpState* regexp, int indent);
}

#endif
