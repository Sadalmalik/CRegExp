#include "cregex.hpp"

#include <stdio.h>
#include <stack>

namespace LightRegexp
{
    void LinkStates(RegexpState*a, RegexpState*b)
    {
        if (a != nullptr) a->next = b;
        if (b != nullptr) b->prev = a;
    }

    void SetupState(RegexpState* state, char type, char match)
    {
        state->type  = type;
        state->match = match;
        state->prev  = nullptr;
        state->next  = nullptr;
        state->inner = nullptr;
    }

    RegexpState* Regexp_Create(std::string pattern)
    {
        std::stack<RegexpState*> statesStack;

        int usedStates = 0;
        int bufferSize = pattern.size() + 1;
        RegexpState*regexp = (RegexpState*) malloc(bufferSize * sizeof(RegexpState));

        auto it = pattern.begin();
        auto end = pattern.end();

        RegexpState *prev = nullptr;
        RegexpState *current = nullptr;
        while (it != end)
        {
            if (usedStates>=bufferSize)
            {
                printf("LightRegexpStatep::Error: Unexpected out of memory!\n");
                Regexp_Destroy(regexp); // Free memory on fail
                return nullptr;
            }
            switch (*it)
            {
            case '^':
            case '$':
            case '.':
                current = &regexp[usedStates++];
                SetupState(current, *it, '\0');
                break;
            case '?':
            case '*':
                if (prev == nullptr)
                    return nullptr;
                current = &regexp[usedStates++];
                SetupState(current, *it, '\0');
                current->inner = prev;          // Put prev as child
                prev = prev->prev;              // Step back
                current->inner->prev = current; // Link parent
                break;
            case '(':
                current = &regexp[usedStates++];
                SetupState(current, *it, '\0');
                statesStack.push(current);
                break;
            case ')':
                if (statesStack.empty())
                {
                    printf("LightRegexpStatep::Error: incorrect order of parentheses ( ) !\n");
                    Regexp_Destroy(regexp); // Free memory on fail
                    return nullptr;
                }
                current = statesStack.top();
                statesStack.pop();

                prev = current->prev;
                current->inner = current->next;
                current->next = nullptr;
                break;
            default:
                current = &regexp[usedStates++];
                SetupState(current, 'c', *it);
            }

            LinkStates(prev, current);
            prev = current;
            current = nullptr;

            ++it;
        }

        if (prev)
        {
            current = &regexp[usedStates++];
            SetupState(current, '\0', '\0');
            LinkStates(prev, current);
        }

        if (!statesStack.empty())
        {
            printf("LightRegexpStatep::Error: incorrect order of parentheses ( ) !\n");
            Regexp_Destroy(regexp); // Free memory on fail
            return nullptr;
        }

        regexp = (RegexpState*) realloc(regexp, usedStates * sizeof(RegexpState));
        return regexp;
    }

    void Regexp_Destroy(RegexpState* regexp)
    {
        if (regexp != nullptr)
            free(regexp);
    }

    RegexpMatch Regexp_MatchInternal(RegexpState* state, std::string str)
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

        RegexpMatch match;
        match.success = true;
        match.substring = str;
        return match;
    }

    RegexpMatch Regexp_Match(RegexpState* RegexpStatep, std::string str)
    {
        RegexpMatch match;
        match.success = false;
        match.substring = str;

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

    RegexpMatch Regexp_Find(RegexpState* RegexpStatep, std::string str)
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
        RegexpMatch match;
        match.success = false;
        match.substring = str;
        return match;
    }

    void pind(int indent)
    {
        while(indent-->0)
            printf("  ");
    }

    void Regexp_Dump(RegexpState*state, int indent)
    {
        if (state==nullptr)
        {
            pind(indent);
            printf("null state!\n");
            return;
        }

        pind(indent); printf("State '%c' : '%c' %p\n", state->type, state->match, (void*) state);
        if (state->inner != nullptr)
            Regexp_Dump(state->inner, indent + 1);
        if (state->next != nullptr)
            Regexp_Dump(state->next, indent);
    }
}
