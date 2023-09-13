#ifndef REGEXP
#define REGEXP

#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string>

namespace LightRegex
{
    using namespace std;

    typedef struct Regex TRegex;
    typedef struct State TState;
    typedef struct Match TMatch;
    typedef struct Group TGroup;


    struct State
    {
        char type;
        char match;
        TState* prev;
        TState* next;
        TState* inner;
    };

    struct Regex
    {
        string raw;
        int groups;
        int size;
        TState states[0];
    };

    struct Group
    {
        std::string substring;
    };

    struct Match
    {
        bool success;
        int size;
        TGroup groups[0];
    };

    TRegex* Create   (std::string pattern);
    void    Destroy  (TRegex* regexp);
    TMatch  Match    (TRegex* regexp, std::string str);
    TMatch  Find     (TRegex* regexp, std::string str);
    void    Dump     (TRegex* regexp);
}

#endif
