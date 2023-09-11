#include "cregex.hpp"

void LinkStates(RegexState*a, RegexState*b)
{
    if (a != nullptr) a->next = b;
    if (b != nullptr) b->prev = a;
}

RegexState* CreateState(char type)
{
    RegexState* state = (RegexState*) malloc(sizeof(RegexState));
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

RegexState* Regexp_ParseInternal(const char** pattern)
{
    RegexState *head, *current, *prev = nullptr;

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
                return nullptr;
            current = CreateState(**pattern);
            current->inner = prev;
            // Step back
            prev = prev->prev;
            break;
        case '(':
            (*pattern)++;
            RegexState*temp = Regexp_ParseInternal(pattern);
            if (!*pattern || **pattern != ')')
                return nullptr;
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
        current = CreateState('\0');
        LinkStates(prev, current);
    }

    return head;
}

RegexState* Regexp_Parse(const char* pattern)
{
    return Regexp_ParseInternal(&pattern);
}

RegexMatch Regexp_MatchInternal(RegexState* state, const char* str)
{
    RegexMatch match;
    match.success = false;
    match.start = str;
    match.end = str;

    if (state == nullptr)
    {
        return match;
    }

    if (state->type == '\0')
    {
        match.success = true;
        return match;
    }

    while (state->type != '\0')
    {
        switch (state->type)
        {
            case '^':
                // Error!
                match.success = false;
                return match;
            case '$':
                // Check for end
                match.success = (*str == '\0');
                return match;
            case 'c':
                if (*str == state->ch)
                    return Regexp_MatchInternal(state->next, str + 1);
                break;
            case '.':
                if (*str != '\0')
                    return Regexp_MatchInternal(state->next, str + 1);
                break;
            case '*':
                while(true)
                {
                    RegexMatch subMatchRepeat = Regexp_MatchInternal(state->inner, str + 1);
                    if (!subMatchRepeat.success)
                        break;
                    str = subMatchRepeat.end;
                }
                break;
            case '?':
                RegexMatch subMatchBranch = Regexp_MatchInternal(state->inner, str + 1);
                if (subMatchBranch.success)
                    str = subMatchBranch.end;
                break;
        }
        state = state->next;
    }

    match.success = true;
    match.end = str;
    return match;
}

RegexMatch Regexp_Match(RegexState* regexp, const char* str)
{
    RegexMatch match;
    match.success = false;
    match.start = str;
    match.end = str;

    if (regexp->type == '^')
    {
        return Regexp_MatchInternal(regexp->next, str);
    }
    do
    {
        match = Regexp_MatchInternal(regexp, str);
        if (match.success)
            return match;
    }
    while (*str++ != '\0');

    match.success = false;
    return match;
}

RegexMatch Regexp_Find(RegexState* regexp, const char* str)
{
    /*
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
            currentPos = 0;//match->end + 1;  // Начнем поиск со следующей позиции после найденного совпадения
        } else {
            currentPos++;
        }
    }
    //*/
    RegexMatch match;
    match.success = false;
    match.start = nullptr;
    match.end = nullptr;
    return match;
}
