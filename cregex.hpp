#ifndef REGEXP
#define REGEXP

#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string>
#include <vector>

// LiRex - Light Regex
namespace LiRex
{
    using namespace std;

    typedef struct Regex TRegex;
    typedef struct State TState;
    typedef struct Match TMatch;

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
        TState*start;
        int groups;
        int size;
        TState states[0];
    };

    struct Match
    {
        bool success = false;
        string match;
        string::const_iterator last;
        vector<string> groups;
    };

    TRegex* Create   (std::string pattern);
    void    Destroy  (TRegex* regexp);

    TMatch  Match    (TRegex* regexp, std::string str);
    TMatch  Find     (TRegex* regexp, std::string str);
    void    Dump     (TRegex* regexp);
}

#endif
