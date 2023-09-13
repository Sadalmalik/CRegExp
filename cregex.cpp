#include "cregex.hpp"

#include <stdio.h>
#include <stack>

namespace LightRegex
{
    void LinkStates(TState*a, TState*b)
    {
        if (a != nullptr) a->next = b;
        if (b != nullptr) b->prev = a;
    }

    void SetupState(TState* state, char type, char match)
    {
        state->type  = type;
        state->match = match;
        state->prev  = nullptr;
        state->next  = nullptr;
        state->inner = nullptr;
    }

    TRegex* Create(std::string pattern)
    {
        int usedStates = 0;
        int bufferSize = pattern.size() + 1;

        void*memory = malloc(sizeof(TRegex) + bufferSize * sizeof(TState));
        TRegex*regexp = (TRegex*)memory;
        regexp->groups = 0;
        TState*states = (TState*) (regexp + 1);

        auto it = pattern.begin();
        auto end = pattern.end();

        TState *prev = nullptr;
        TState *current = nullptr;
        std::stack<TState*> statesStack;
        while (it != end)
        {
            if (usedStates>=bufferSize)
            {
                printf("LightRegex::Error: Unexpected out of memory!\n");
                Destroy(regexp); // Free memory on fail
                return nullptr;
            }
            switch (*it)
            {
            case '^':
            case '$':
            case '.':
                current = &states[usedStates++];
                SetupState(current, *it, '\0');
                break;
            case '?':
            case '*':
                if (prev == nullptr)
                    return nullptr;
                current = &states[usedStates++];
                SetupState(current, *it, '\0');
                current->inner = prev;          // Put prev as child
                prev = prev->prev;              // Step back
                current->inner->prev = current; // Link parent
                break;
            case '(':
                current = &states[usedStates++];
                SetupState(current, *it, '\0');
                statesStack.push(current);
                break;
            case ')':
                if (statesStack.empty())
                {
                    printf("LightRegex::Error: incorrect order of parentheses ( ) !\n");
                    Destroy(regexp); // Free memory on fail
                    return nullptr;
                }
                current = statesStack.top();
                statesStack.pop();

                prev = current->prev;
                current->inner = current->next;
                current->next = nullptr;
                regexp->groups++;
                break;
            default:
                current = &states[usedStates++];
                SetupState(current, 'c', *it);
            }

            LinkStates(prev, current);
            prev = current;
            current = nullptr;

            ++it;
        }

        if (prev)
        {
            current = &states[usedStates++];
            SetupState(current, '\0', '\0');
            LinkStates(prev, current);
        }

        if (!statesStack.empty())
        {
            printf("LightRegex::Error: incorrect order of parentheses ( ) !\n");
            Destroy(regexp); // Free memory on fail
            return nullptr;
        }

        regexp = (TRegex*) realloc(memory, sizeof(TRegex) + usedStates * sizeof(TState));
        new (&regexp->raw) std::string();
        regexp->raw = pattern;
        regexp->size = usedStates;
        return regexp;
    }

    void Destroy(TRegex* regexp)
    {
        if (regexp != nullptr)
            free(regexp);
    }

    TMatch Regexp_MatchInternal(TRegex* state, std::string str)
    {
        /*
        RegexpMatch RegexpMatch;
        RegexpMatch.success = false;
        //RegexpMatch.start = str;
        //RegexpMatch.end = str;

        if (state == nullptr)
        {
            return RegexpMatch;
        }

        if (state->type == '\0')
        {
            RegexpMatch.success = true;
            return RegexpMatch;
        }

        while (state->type != '\0')
        {
            switch (state->type)
            {
                case '^':
                    // Error!
                    RegexpMatch.success = false;
                    return RegexpMatch;
                case '$':
                    // Check for end
                    RegexpMatch.success = (*str == '\0');
                    return RegexpMatch;
                case 'c':
                    if (*str == state->ch)
                        return RegexpStatep_RegexpMatchInternal(state->next, str + 1);
                    break;
                case '.':
                    if (*str != '\0')
                        return RegexpStatep_RegexpMatchInternal(state->next, str + 1);
                    break;
                case '*':
                    while(true)
                    {
                        RegexpMatch subRegexpMatchRepeat = RegexpStatep_RegexpMatchInternal(state->inner, str + 1);
                        if (!subRegexpMatchRepeat.success)
                            break;
                        str = subRegexpMatchRepeat.end;
                    }
                    break;
                case '?':
                    RegexpMatch subRegexpMatchBranch = RegexpStatep_RegexpMatchInternal(state->inner, str + 1);
                    if (subRegexpMatchBranch.success)
                        str = subRegexpMatchBranch.end;
                    break;
            }
            state = state->next;
            str++;
        }
        //*/

        TMatch match;
        match.success = true;
        return match;
    }

    TMatch Match(TRegex* regexp, std::string str)
    {
        TMatch match;
        match.success = false;

        /*
        if (RegexpStatep->type == '^')
        {
            return RegexpStatep_RegexpMatchInternal(RegexpStatep->next, str);
        }
        do
        {
            match = RegexpStatep_RegexpMatchInternal(RegexpStatep, str);
            if (match.success)
                return match;
        }
        while (*str++ != '\0');
        //*/

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

        DumpState(regexp->states, 0);
    }
}
