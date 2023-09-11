#ifndef REGEXP
#define REGEXP

#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct RegexState RegexState;
typedef struct RegexMatch RegexMatch;

struct RegexState {
    char type;
    char ch;
    int params;
    RegexState* prev;
    RegexState* next;
    RegexState* inner;
};

struct RegexMatch
{
    bool success;
    const char* start;
    const char* end;
};

RegexState* Regexp_Parse    (const char* pattern);
RegexMatch  Regexp_Match    (RegexState* regexp, const char* str);
RegexMatch  Regexp_Find     (RegexState* regexp, const char* str);
//void        Regexp_Dump     (RegexState*state);

#endif
