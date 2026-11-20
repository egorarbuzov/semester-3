#pragma once
#include <stack>
#include <vector>
#include "GRB.h"
#include "LT.h"
#include "IT.h"
#include "Error.h"

#define MFST_DIAGN_NUMBER 3
#define MFST_DIAGN_MAXSIZE 1024

typedef std::stack<short> MFSTSTSTACK;

namespace LEX
{
    struct LEX
    {
        LT::LexTable lextable;
        IT::IdTable idtable;

        LEX() : lextable{ 0, 0, nullptr }, idtable{ 0, 0, nullptr } {}
        LEX(LT::LexTable& plt, IT::IdTable& pit) : lextable(plt), idtable(pit) {}
    };
}

namespace MFST
{
    struct MfstState
    {
        short lenta_position;
        short nrule;
        short nrulechain;
        MFSTSTSTACK st;

        MfstState();
        MfstState(short pposition, MFSTSTSTACK pst, short pnrule, short pnrulechain);
    };

    struct Mfst
    {
        enum RC_STEP
        {
            NS_OK,
            NS_NORULE,
            NS_NORULECHAIN,
            NS_ERROR,
            TS_OK,
            TS_NOK,
            LENTA_END,
            SURPRISE
        };

        struct MfstDiagnosis
        {
            short lenta_position;
            RC_STEP rc_step;
            short nrule;
            short nrule_chain;

            MfstDiagnosis();
            MfstDiagnosis(short p, RC_STEP rc, short r, short c);
        } diagnosis[MFST_DIAGN_NUMBER];

        GRBALPHABET* lenta;
        short lenta_position;
        short nrule;
        short nrulechain;
        short lenta_size;
        GRB::Greibach grebach;
        LEX::LEX lex;
        MFSTSTSTACK st;
        std::stack<MfstState> storestate;
        int step_counter;
        bool have_resume;

        Mfst();
        Mfst(LEX::LEX& plex, GRB::Greibach pgreibach);

        char* getCSt(char* buf);
        char* getCLenta(char* buf, short pos, short n = 25);
        char* getDiagnosis(short n, char* buf);
        bool savestate();
        bool reststate();
        bool resetstate() { return reststate(); }
        bool push_chain(GRB::Rule::Chain chain);

        RC_STEP step();
        bool start();
        bool savediagnosis(RC_STEP prc_step);
    };
}
