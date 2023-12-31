#include "light_regexp.hpp"

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

    void AddGroups(TMatch& dest, TMatch& source)
    {
        dest.groups.insert(
            dest.groups.end(),
            source.groups.begin(),
            source.groups.end());
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

        while (it != sEnd && state)
        {
            //printf("  step: '%c':'%c' == '%c'\n", state->type, state->match, *it);
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
                // Only symbol, defined as match
                match.success = false;
                return match;
            case '.':
                if (*it != '\0')
                    break;
                // Anything except end of string
                match.success = false;
                return match;
            case '?':
                subMatch = MatchInternal(state->inner, it, sEnd);
                if (subMatch.success)
                {
                    it = subMatch.last;
                    AddGroups(match, subMatch);
                    break;
                }
                --it;
                break;
            case '*':
                while(it!=sEnd)
                {
                    subMatch = MatchInternal(state->inner, it, sEnd);
                    if (!subMatch.success) break;
                    AddGroups(match, subMatch);
                    it = subMatch.last;
                    ++it;
                }
                --it;
                break;
            case '(':
                subMatch = MatchInternal(state->inner, it, sEnd);
                if (!subMatch.success)
                {
                    match.success = false;
                    return match;
                }
                AddGroups(match, subMatch);
                match.groups.push_back(string(it, subMatch.last+1));
                it = subMatch.last;
                break;
            }

            state = state->next;
            match.last = it;
            ++it;
        }

        if (state != nullptr && it == sEnd)
            match.success = false;

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
            match = MatchInternal(state->next, sBegin, sEnd);
            match.match = std::string(sBegin, match.last+1);
            return match;
        }
        do
        {
            match = MatchInternal(state, sBegin, sEnd);
            if (match.success)
            {
                match.match = std::string(sBegin, match.last+1);
                return match;
            }
        }
        while (++sBegin != sEnd);

        match.success = false;
        match.match = std::string("");
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
                currentPos = 0;//RegexpMatch->end + 1;  // ������ ����� �� ��������� ������� ����� ���������� ����������
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
