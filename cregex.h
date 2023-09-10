#include <stdlib.h>
#include <ctype.h>

// ���������� ������ ���������� ����� ������ ��� �������.
#define INITIAL_STATE_CAPACITY 100

typedef int bool;
#define false 0
#define true 1

typedef enum {
    CHAR,   // ������������� ����������� �������
    ANY,    // ������������� ������ �������
    SPLIT,  // ������������� ������ ����� ����� ��������
    MATCH   // ������������ �����, ����������� �� �������� ����������
    // ����� ����� ���� ��������� ������ ���� ��� �������� *, ? � ��� �����
} StateType;

typedef struct RegexState {
    StateType type;
    char ch;  // ������������, ���� type == CHAR
    struct RegexState* out;      // ��� �������� �������� ��� ������� �������� � SPLIT
    struct RegexState* out1;     // ������ ��� SPLIT, ������������ ������ �����
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
