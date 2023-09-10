#include <stdlib.h>
#include <ctype.h>

// Определяем размер начального блока памяти для стейтов.
#define INITIAL_STATE_CAPACITY 100

typedef int bool;
#define false 0
#define true 1

typedef enum {
    CHAR,   // соответствует конкретному символу
    ANY,    // соответствует любому символу
    SPLIT,  // соответствует выбору между двумя стейтами
    MATCH   // терминальный стейт, указывающий на успешное совпадение
    // Далее могут быть добавлены другие типы для операций *, ? и так далее
} StateType;

typedef struct RegexState {
    StateType type;
    char ch;  // используется, если type == CHAR
    struct RegexState* out;      // для базового перехода или первого перехода у SPLIT
    struct RegexState* out1;     // только для SPLIT, представляет второй выбор
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
