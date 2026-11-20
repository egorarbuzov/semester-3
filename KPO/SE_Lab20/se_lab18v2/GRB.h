#pragma once
#include <cstdarg>
#include "Error.h"

#define GRB_ERROR_SERIES 600
#define GRB_MAX_RULE_STRING 256

typedef short GRBALPHABET;

namespace GRB
{
    struct Rule
    {
        GRBALPHABET nn;
        int iderror;
        short size;

        struct Chain
        {
            short size;
            GRBALPHABET* nt;

            Chain() : size(0), nt(nullptr) {}
            Chain(short psize, GRBALPHABET s, ...);

            char* getCChain(char* b);

            static GRBALPHABET T(char t) { return GRBALPHABET(t); }
            static GRBALPHABET N(char n) { return -GRBALPHABET(n); }
            static bool isT(GRBALPHABET s) { return s > 0; }
            static bool isN(GRBALPHABET s) { return s < 0; }
            static char alphabet_to_char(GRBALPHABET s)
            {
                return isT(s) ? char(s) : char(-s);
            }
        } *chains;

        Rule() : nn(0), iderror(0), size(0), chains(nullptr) {}
        Rule(GRBALPHABET pnn, int piderror, short psize, Chain c, ...);

        char* getCRule(char* b, short nchain);
        short getNextChain(GRBALPHABET t, Chain& pchain, short j);
    };

    struct Greibach
    {
        short size;
        GRBALPHABET startN;
        GRBALPHABET stbottomT;
        Rule* rules;

        Greibach() : size(0), startN(0), stbottomT(0), rules(nullptr) {}
        Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottomT, short psize, Rule r, ...);

        short getRule(GRBALPHABET pnn, Rule& prule);
        Rule getRule(short n);
    };

    Greibach getGreibach();
}
