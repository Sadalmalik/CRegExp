#include "cregex.hpp"

#include <stdio.h>
#include <stack>

namespace LiRex
{
    void LinkStates(TState*a, TState*b)
    {
        if (a != nullptr) a->next = b;
        if (b != nullptr) b->prev = a;
    }

    TRegex* AllocateRegexp(string&pattern, size_t bufferSize)
    {
        TRegex*regexp = (TRegex*) malloc(sizeof(TRegex) + bufferSize * sizeof(TState));
        if (regexp == nullptr)
            return regexp;
        new (&regexp->raw) string(pattern);
        regexp->start = nullptr;
        regexp->groups = 0;
        regexp->size = 0;
        return regexp;
    }

    TState* CreateState(TRegex*regexp, char type, char match)
    {
        TState *state = &regexp->states[regexp->size];
        regexp->size++;
        state->type  = type;
        state->match = match;
        state->prev  = nullptr;
        state->next  = nullptr;
        state->inner = nullptr;
        return state;
    }

    TRegex* TrimRegexp(TRegex *regexp)
    {
        return (TRegex*) realloc((void*) regexp, sizeof(TRegex) + regexp->size * sizeof(TState));
    }

    TRegex* Create(std::string pattern)
    {
        size_t bufferSize = pattern.size() * 2 + 1;
        TRegex*regexp = AllocateRegexp(pattern, bufferSize);

        std::string::iterator it = pattern.begin();
        std::string::iterator sEnd = pattern.end();

        TState *prev = nullptr;
        TState *current = nullptr;
        std::stack<TState*> statesStack;
        while (it != sEnd)
        {
            if (regexp->size >= bufferSize)
            {
                printf("LiRex::Error: Unexpected out of memory!\n");
                Destroy(regexp); // Free memory on fail
                return nullptr;
            }

            switch (*it)
            {
            case '^':
            case '$':
            case '.':
                current = CreateState(regexp, *it, '\0');
                break;
            case '?':
            case '*':
                if (prev == nullptr)
                {
                    printf("LiRex::Error: symbol '%c' can't be first in pattern!\n", *it);
                    Destroy(regexp); // Free memory on fail
                    return nullptr;
                }
                current = CreateState(regexp, *it, '\0');
                current->inner = prev;          // Put prev as child
                prev = prev->prev;              // Step back
                current->inner->prev = current; // Link parent
                if (prev == nullptr)
                {
                    regexp->start = current;
                }
                break;
            case '(':
                current = CreateState(regexp, *it, '\0');
                statesStack.push(current);
                break;
            case ')':
                if (statesStack.empty())
                {
                    printf("LiRex::Error: incorrect order of parentheses ( ) !\n");
                    Destroy(regexp); // Free memory on fail
                    return nullptr;
                }
                regexp->groups++;
                current = statesStack.top();
                statesStack.pop();
                prev = current->prev;
                current->inner = current->next;
                current->next = nullptr;
                break;
            case '\\':
                ++it;
                if (it == sEnd)
                {
                    printf("LiRex::Error: symbol '%c' can't be last in pattern!\n", *it);
                    Destroy(regexp); // Free memory on fail
                    return nullptr;
                }
                current = CreateState(regexp, 'c', *it);
                break;
            default:
                current = CreateState(regexp, 'c', *it);
                break;
            }

            if (regexp->start == nullptr)
                regexp->start = current;
            LinkStates(prev, current);
            prev = current;
            current = nullptr;

            ++it;
        }

        if (!statesStack.empty())
        {
            printf("LiRex::Error: incorrect order of parentheses ( ) !\n");
            Destroy(regexp); // Free memory on fail
            return nullptr;
        }

        regexp = TrimRegexp(regexp);
        return regexp;
    }

    void Destroy(TRegex* regexp)
    {
        if (regexp != nullptr)
        {
            regexp->raw.~string();
            free(regexp);
        }
    }

    typedef struct Frame TFrame;
    struct Frame
    {
        TState*state;
        string::const_iterator start;
    };

    TMatch MatchInternal(
            TState* state,
            string::const_iterator sBegin,
            string::const_iterator sEnd)
    {
        TMatch match = TMatch();
        match.success = true;

        // Empty regexp match anything!
        if (state==nullptr)
            return match;

        TMatch subMatch;
        string::const_iterator it = sBegin;

        while ( it != sEnd )
        {
            switch (state->type)
            {
            case '^':
                // Error!
                match.success = false;
                return match;
            case '$':
                // Check for end
                match.success = (*it == '\0');
                return match;
            case 'c':
                if (*it == state->match)
                    break;
                match.success = false;
                return match;
            case '.':
                if (*it != '\0')
                    break;
                // Anything except end of string;
                match.success = false;
                return match;
            case '?':
                subMatch = MatchInternal(state->inner, it, sEnd);
                if (subMatch.success)
                    it = subMatch.end;
                break;
            case '*':
                do {
                    subMatch = MatchInternal(state->inner, it, sEnd);
                    if (subMatch.success)
                        it = subMatch.end;
                } while (subMatch.success);
                break;
            case '(':
                subMatch = MatchInternal(state->inner, it, sEnd);
                if (!subMatch.success)
                {
                    match.success = false;
                    return match;
                }
                match.groups.push_back(string(it, subMatch.end));
                continue;
            }
            state = state->next;
            ++it;
        }

        return match;
    }

    TMatch Match(TRegex* regexp, std::string str)
    {
        TMatch match;
        match.success = false;

        string::const_iterator sBegin = str.begin();
        string::const_iterator sEnd = str.end();

        TState*state = &regexp->states[0];
        if (state->type == '^')
        {
            return MatchInternal(state->next, sBegin, sEnd);
        }
        do
        {
            match = MatchInternal(state, sBegin, sEnd);
            if (match.success)
                return match;
        }
        while (*sBegin++ != '\0');

        match.success = false;
        return match;
    }

    TMatch Find(TRegex* regexp, std::string str)
    {
        /*
        RegexpMatch *RegexpMatches = NULL, *lastRegexpMatch = NULL;
        const char* currentPos = str;

        while (*currentPos != '\0') {
            RegexpMatch* RegexpMatch = findRegexpMatchFromPosition(RegexpStatep, currentPos, currentPos);
            if (RegexpMatch) {
                if (RegexpMatches == NULL) {
                    RegexpMatches = RegexpMatch;
                } else {
                    //lastRegexpMatch->next = RegexpMatch;
                }
                lastRegexpMatch = RegexpMatch;
                currentPos = 0;//RegexpMatch->end + 1;  // Начнем поиск со следующей позиции после найденного совпадения
            } else {
                currentPos++;
            }
        }
        //*/
        TMatch match;
        match.success = false;
        return match;
    }

    void pind(int indent)
    {
        while(indent-->0)
            printf("  ");
    }

    void DumpState(TState* state, int indent)
    {
        if (state==nullptr)
        {
            pind(indent);
            printf("null state!\n");
            return;
        }

        pind(indent); printf("State '%c' : '%c' %p\n", state->type, state->match, (void*) state);
        if (state->inner != nullptr)
            DumpState(state->inner, indent + 1);
        if (state->next != nullptr)
            DumpState(state->next, indent);
    }

    void Dump(TRegex* regexp)
    {
        if (regexp==nullptr)
        {
            printf("null regexp!\n");
            return;
        }

        printf("Dump regexp '%s' with %d groups:\n", regexp->raw.c_str(), regexp->groups);
        DumpState(regexp->start, 0);
    }
}
