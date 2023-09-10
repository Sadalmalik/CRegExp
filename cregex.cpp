#include "cregex.hpp"

static RegexState* stateMemory = NULL;
static int stateIndex = 0;
static int stateCapacity = 0;

void InitRegex(int initialCapacity)
{
    stateMemory = (RegexState*) malloc(sizeof(RegexState) * initialCapacity);
    stateCapacity = initialCapacity;
    stateIndex = 0;
}

void LinkStates(RegexState*a, RegexState*b)
{
    if (a != nullptr) a->next = b;
    if (b != nullptr) b->prev = a;
}

RegexState* CreateState(char type)
{
    if (stateIndex >= stateCapacity)
    {
        stateCapacity *= 2;
        // Breaks all links to existing regexps!
        stateMemory = (RegexState*)realloc(stateMemory, sizeof(RegexState) * stateCapacity);
    }
    RegexState* state = &stateMemory[stateIndex++];
    state->type = type;
    state->ch = 0;
    state->prev = nullptr;
    state->next = nullptr;
    state->inner = nullptr;
    return state;
}

RegexState* CreateStateChar(char ch)
{
    RegexState* s = CreateState('c');
    s->ch = ch;
    return s;
}

RegexState* InternalParseRegexp(const char** pattern)
{
    int storedIndex = stateIndex;
    RegexState *stack[32];
    RegexState *head, *current, *prev = nullptr;

    goto start;
terminate:
    // If we can't parse regex - we will return system to initial state
    // Also - I think we need add some error messages somewhere here
    stateIndex = storedIndex;
    return nullptr;
start:

    while (*pattern &&
          **pattern != '\0' &&
          **pattern != ')')
    {
        switch (**pattern)
        {
        case '^':
            current = CreateState('^');
            break;
        case '$':
            current = CreateState('$');
            break;
        case '.':
            current = CreateState('.');
            break;
        case '?':
        case '*':
            if (prev == nullptr)
                goto terminate;
            current = CreateState(**pattern);
            current->inner = prev;
            // Step back
            prev = prev->prev;
            break;
        case '(':
            (*pattern)++;
            RegexState*temp = InternalParseRegexp(pattern);
            if (!*pattern || **pattern != ')')
                goto terminate;
            current = CreateState('(');
            current->inner = temp;
            break;
        }

        if (current == nullptr)
            current = CreateStateChar(**pattern);

        if (head == nullptr)
            head = current;
        LinkStates(prev, current);
        prev = current;
        current = nullptr;

        (*pattern)++;
    }

    if (prev)
    {
        RegexState* match = CreateState('\0');
        LinkStates(prev, current);
    }

    return head;
}

RegexState* ParseRegexp(const char* pattern)
{
    return InternalParseRegexp(&pattern);
}

bool isMatch(RegexState* state, const char* str)
{
    if (state == nullptr)
        return false;

    if (state->type == '\0')
        return true;

    while (state->type != '\0')
    {
        switch (state->type)
        {
            case '^':
                // Error!
                return false;
            case '$':
                // Check for end
                return *str == '\0';
            case 'c':
                if (*str == state->ch)
                    return isMatch(state->next, str + 1);
                break;
            case '.':
                if (*str != '\0')
                    return isMatch(state->next, str + 1);
                break;
            case '*':
                if (*str != '\0')
                    return isMatch(state->next, str + 1);
                break;
            case '?':
                if (*str != '\0')
                    return isMatch(state->next, str + 1);
                break;
        }
    }

    return false;
}

bool Match(RegexState* regexp, const char* str)
{
    if (regexp->type == '^')
        return isMatch(regexp->next, str);

    do
    {
        if (isMatch(regexp, str))
            return true;
    }
    while (*str++ != '\0');
    return false;
}

RegexMatch* findMatchFromPosition(RegexState* state, const char* str, const char* startPos) {
    if (isMatch(state, str)) {
        RegexMatch* match = (RegexMatch*)malloc(sizeof(RegexMatch));
        //match->start = startPos;
        //match->end = str;  // ”казатель на последний символ совпадени€
        //match->next = NULL;
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
                //lastMatch->next = match;
            }
            lastMatch = match;
            currentPos = 0;//match->end + 1;  // Ќачнем поиск со следующей позиции после найденного совпадени€
        } else {
            currentPos++;
        }
    }

    return matches;
}
