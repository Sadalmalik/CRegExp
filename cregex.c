#include "cregex.h"

static RegexState* stateMemory = NULL;
static int stateIndex = 0; // текущий индекс в stateMemory для следующего выделения
static int stateCapacity = 0; // текущая емкость блока памяти stateMemory

void InitRegex(int initialCapacity) {
    stateMemory = (RegexState*)malloc(sizeof(RegexState) * initialCapacity);
    stateCapacity = initialCapacity;
    stateIndex = 0;
}

RegexState* CreateState() {
    if (stateIndex >= stateCapacity) {
        // Удваиваем память, если она закончилась
        stateCapacity *= 2;
        stateMemory = (RegexState*)realloc(stateMemory, sizeof(RegexState) * stateCapacity);
    }
    return &stateMemory[stateIndex++];
}

RegexState* StateChar(char ch) {
    RegexState* s = CreateState();
    s->type = CHAR;
    s->ch = ch;
    s->out = NULL;
    s->out1 = NULL;
    return s;
}

RegexState* StateAny() {
    RegexState* s = CreateState();
    s->type = ANY;
    s->out = NULL;
    s->out1 = NULL;
    return s;
}

RegexState* StateSplit(RegexState* out, RegexState* out1) {
    RegexState* s = CreateState();
    s->type = SPLIT;
    s->out = out;
    s->out1 = out1;
    return s;
}

RegexState* ParseRegexp(const char* pattern) {
    RegexState *start, *current, *prev = NULL;

    while (*pattern) {
        switch (*pattern) {
        case '.':
            current = StateAny();
            break;
        case '?':
            if (prev == NULL) {
                // ошибка: ? без предыдущего символа или оператора
                return NULL;
            }
            current = StateSplit(prev, CreateState());
            break;
        default:
            current = StateChar(*pattern);
        }

        if (prev) {
            prev->out = current;
        } else {
            start = current; // сохраняем начальный стейт, если это первый символ/оператор
        }

        prev = current;
        pattern++;
    }

    // Добавляем терминальный состояние MATCH в конце
    if (prev) {
        RegexState* match = CreateState();
        match->type = MATCH;
        prev->out = match;
    }

    return start;
}

bool isMatch(RegexState* state, const char* str) {
    if (state == NULL) {
        return false;
    }

    if (state->type == MATCH) {
        return *str == '\0'; // Если мы достигли состояния MATCH, строка также должна быть закончена
    }

    switch (state->type) {
        case CHAR:
            if (*str == state->ch) {
                return isMatch(state->out, str + 1);
            }
            break;
        case ANY:
            if (*str != '\0') {
                return isMatch(state->out, str + 1);
            }
            break;
        case SPLIT:
            return isMatch(state->out, str) || isMatch(state->out1, str);
    }

    return false;
}

bool Match(RegexState* regexp, const char* str) {
    return isMatch(regexp, str);
}
RegexMatch* findMatchFromPosition(RegexState* state, const char* str, const char* startPos) {
    if (isMatch(state, str)) {
        RegexMatch* match = (RegexMatch*)malloc(sizeof(RegexMatch));
        match->start = startPos;
        match->end = str;  // Указатель на последний символ совпадения
        match->next = NULL;
        return match;
    }
    return NULL;
}

RegexMatch* Find(RegexState* regexp, const char* str) {
    RegexMatch *matches = NULL, *lastMatch = NULL;
    const char* currentPos = str;

    while (*currentPos != '\0') {
        RegexMatch* match = findMatchFromPosition(regexp, currentPos, currentPos);
        if (match) {
            if (matches == NULL) {
                matches = match;
            } else {
                lastMatch->next = match;
            }
            lastMatch = match;
            currentPos = match->end + 1;  // Начнем поиск со следующей позиции после найденного совпадения
        } else {
            currentPos++;
        }
    }

    return matches;
}

