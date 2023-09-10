#ifndef REGEXP
#define REGEXP

#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

// Определяем размер начального блока памяти для стейтов.
#define INITIAL_STATE_CAPACITY 100

typedef enum {
    Char,
    Any,
    Repeat,
    Optional,
    Charlist,
    Variants,
    Group,
    GroupEnd,
    End
} StateType;

typedef struct RegexState {
    char type;
    char ch;
    int params;
    struct RegexState* prev;
    struct RegexState* next;
    struct RegexState* inner;
} RegexState;

typedef struct {
    const char* start;
    const char* end;
} Substring;

typedef struct {
    int count;
    Substring* matches;
} RegexMatch;

void InitRegex(int initialCapacity);
RegexState* ParseRegexp(const char* pattern);
bool isMatch(RegexState* state, const char* str);
bool Match(RegexState* regexp, const char* str);
RegexMatch* findMatchFromPosition(RegexState* state, const char* str, const char* startPos);
RegexMatch* Find(RegexState* regexp, const char* str);
void DumpRegex(RegexState*state);

#endif
