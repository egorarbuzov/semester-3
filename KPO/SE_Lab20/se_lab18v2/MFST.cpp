#include "stdafx.h"
#include "MFST.h"
#include <cstring>
#include <cstdio>
#include <iostream>
#include <vector>

namespace MFST
{
    MfstState::MfstState()
        : lenta_position(0), nrule(-1), nrulechain(-1)
    {
    }

    MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrule, short pnrulechain)
        : lenta_position(pposition), nrule(pnrule), nrulechain(pnrulechain), st(pst)
    {
    }

    Mfst::MfstDiagnosis::MfstDiagnosis()
        : lenta_position(-1), rc_step(SURPRISE), nrule(-1), nrule_chain(-1)
    {
    }

    Mfst::MfstDiagnosis::MfstDiagnosis(short p, RC_STEP rc, short r, short c)
        : lenta_position(p), rc_step(rc), nrule(r), nrule_chain(c)
    {
    }

    static const char* rcName(Mfst::RC_STEP rc);
    static void printTrace(Mfst& m, Mfst::RC_STEP rc);

    Mfst::Mfst()
        : lenta(nullptr), lenta_position(0), nrule(-1), nrulechain(-1),
          lenta_size(0), step_counter(0), have_resume(false)
    {
        lex.lextable.maxsize = 0;
        lex.lextable.size = 0;
        lex.lextable.table = nullptr;
        lex.idtable.maxsize = 0;
        lex.idtable.size = 0;
        lex.idtable.table = nullptr;
    }

    Mfst::Mfst(LEX::LEX& plex, GRB::Greibach pgrebach)
        : lenta(nullptr), lenta_position(0), nrule(-1), nrulechain(-1),
          lenta_size(0), grebach(pgrebach), lex(plex),
          step_counter(0), have_resume(false)
    {
        if (lex.lextable.size <= 0 || lex.lextable.size > 32000 || lex.lextable.table == nullptr)
            ERROR_THROW(207);

        if (lex.lextable.table[lex.lextable.size - 1].lexema[0] != '$')
            ERROR_THROW(207);

        lenta_size = (short)lex.lextable.size;
        lenta = new GRBALPHABET[lenta_size];
        for (int k = 0; k < lex.lextable.size; ++k)
            lenta[k] = GRB::Rule::Chain::T(lex.lextable.table[k].lexema[0]);

        while (!st.empty()) st.pop();
        st.push(grebach.stbottomT);
        st.push(grebach.startN);
    }

    Mfst::RC_STEP Mfst::step()
    {
        RC_STEP rc = SURPRISE;
        ++step_counter;

        if (lenta_position >= lenta_size)
        {
            if (st.empty() || (st.size() == 1 && st.top() == grebach.stbottomT))
            {
                rc = LENTA_END;
            }
            else
            {
                savediagnosis(LENTA_END);
                rc = reststate() ? TS_NOK : NS_NORULE;
            }
            printTrace(*this, rc);
            return rc;
        }

        if (st.empty())
        {
            savediagnosis(NS_NORULE);
            rc = reststate() ? NS_NORULECHAIN : NS_NORULE;
            printTrace(*this, rc);
            return rc;
        }

        GRBALPHABET top = st.top();

        if (GRB::Rule::Chain::isN(top))
        {
            GRB::Rule rule;
            short ruleIndex = grebach.getRule(top, rule);

            if (ruleIndex < 0)
            {
                nrule = -1;
                savediagnosis(NS_NORULE);
                rc = reststate() ? NS_NORULECHAIN : NS_NORULE;
            }
            else
            {
                nrule = ruleIndex;

                short startChain = 0;
                if (have_resume)
                {
                    startChain = nrulechain + 1;
                    have_resume = false;
                }
                else
                {
                    nrulechain = -1;
                    startChain = 0;
                }

                GRB::Rule::Chain chain;
                short chainIndex = rule.getNextChain(lenta[lenta_position], chain, startChain);

                if (chainIndex < 0)
                {
                    savediagnosis(NS_NORULECHAIN);
                    rc = reststate() ? NS_NORULECHAIN : NS_NORULE;
                }
                else
                {
                    nrulechain = chainIndex;

                    if (chainIndex + 1 < rule.size)
                    {
                        savestate();
                        std::cout << "  savestate: rule " << nrule
                                  << ", chain " << nrulechain << std::endl;
                    }

                    st.pop();
                    push_chain(chain);
                    rc = NS_OK;
                }
            }
        }
        else
        {
            have_resume = false;

            if (top == lenta[lenta_position])
            {
                st.pop();
                ++lenta_position;
                rc = TS_OK;
            }
            else
            {
                savediagnosis(TS_NOK);
                rc = reststate() ? TS_NOK : NS_NORULE;
            }
        }

        printTrace(*this, rc);
        return rc;
    }

    bool Mfst::push_chain(GRB::Rule::Chain chain)
    {
        for (int k = chain.size - 1; k >= 0; --k)
            st.push(chain.nt[k]);
        return true;
    }

    bool Mfst::savestate()
    {
        storestate.push(MfstState(lenta_position, st, nrule, nrulechain));
        return true;
    }

    bool Mfst::reststate()
    {
        if (storestate.empty())
            return false;

        MfstState state = storestate.top();
        storestate.pop();

        lenta_position = state.lenta_position;
        st = state.st;
        nrule = state.nrule;
        nrulechain = state.nrulechain;
        have_resume = true;

        std::cout << "  reststate: правило " << nrule
                  << ", next chain " << (nrulechain + 1) << std::endl;
        return true;
    }

    bool Mfst::savediagnosis(RC_STEP prc_step)
    {
        short k = 0;
        while (k < MFST_DIAGN_NUMBER &&
               diagnosis[k].lenta_position >= lenta_position)
            ++k;

        if (k >= MFST_DIAGN_NUMBER)
            return false;

        for (int i = MFST_DIAGN_NUMBER - 1; i > k; --i)
            diagnosis[i] = diagnosis[i - 1];

        diagnosis[k] = MfstDiagnosis(lenta_position, prc_step, nrule, nrulechain);
        return true;
    }

    bool Mfst::start()
    {
        bool ok = false;
        RC_STEP rc = step();

        while (rc == NS_OK || rc == TS_OK || rc == NS_NORULECHAIN || rc == TS_NOK)
        {
            if (step_counter > 200000)
            {
                std::cout << "Слишком много шагов, остановка" << std::endl;
                break;
            }
            rc = step();
        }

        if (rc == LENTA_END &&
            (st.empty() || (st.size() == 1 && st.top() == grebach.stbottomT)))
        {
            ok = true;
            std::cout << "Синтаксический анализ закончился успешно" << std::endl;
        }
        else
        {
            std::cout << "Синтаксический анализ провален: " << rcName(rc) << std::endl;
            char dbuf[MFST_DIAGN_MAXSIZE];
            for (short i = 0; i < MFST_DIAGN_NUMBER; ++i)
            {
                getDiagnosis(i, dbuf);
                if (dbuf[0] != 0)
                    std::cout << "Диагностика: " << dbuf << std::endl;
            }
        }

        return ok;
    }

    char* Mfst::getCSt(char* buf)
    {
        if (buf == nullptr) return buf;

        MFSTSTSTACK tmp = st;
        std::vector<short> items;
        while (!tmp.empty())
        {
            items.push_back(tmp.top());
            tmp.pop();
        }

        int j = 0;
        for (int i = (int)items.size() - 1; i >= 0 && j < MFST_DIAGN_MAXSIZE - 1; --i)
            buf[j++] = GRB::Rule::Chain::alphabet_to_char(items[i]);

        buf[j] = 0;
        return buf;
    }

    char* Mfst::getCLenta(char* buf, short pos, short n)
    {
        if (buf == nullptr) return buf;
        if (pos < 0) pos = 0;

        int end = pos + n;
        if (end > lenta_size) end = lenta_size;

        int j = 0;
        for (int i = pos; i < end && j < MFST_DIAGN_MAXSIZE - 1; ++i)
            buf[j++] = GRB::Rule::Chain::alphabet_to_char(lenta[i]);

        buf[j] = 0;
        return buf;
    }

    char* Mfst::getDiagnosis(short n, char* buf)
    {
        if (buf == nullptr) return buf;
        buf[0] = 0;

        if (n < 0 || n >= MFST_DIAGN_NUMBER)
            return buf;
        if (diagnosis[n].lenta_position < 0)
            return buf;

        int errid = GRB_ERROR_SERIES;
        if (diagnosis[n].nrule >= 0 && diagnosis[n].nrule < grebach.size)
            errid = grebach.getRule(diagnosis[n].nrule).iderror;

        Error::Error err = Error::geterror(errid);

        int line = 0;
        if (lex.lextable.size > 0 && lex.lextable.table != nullptr)
        {
            int p = diagnosis[n].lenta_position;
            if (p < 0) p = 0;
            if (p >= lex.lextable.size) p = lex.lextable.size - 1;
            line = lex.lextable.table[p].sn;
        }

        sprintf_s(buf, MFST_DIAGN_MAXSIZE,
            "%d: %s (line %d, pos %d)",
            err.id, err.message, line, (int)diagnosis[n].lenta_position);
        return buf;
    }

    static const char* rcName(Mfst::RC_STEP rc)
    {
        switch (rc)
        {
        case Mfst::NS_OK: return "NS_OK";
        case Mfst::NS_NORULE: return "NS_NORULE";
        case Mfst::NS_NORULECHAIN: return "NS_NORULECHAIN";
        case Mfst::NS_ERROR: return "NS_ERROR";
        case Mfst::TS_OK: return "TS_OK";
        case Mfst::TS_NOK: return "TS_NOK";
        case Mfst::LENTA_END: return "LENTA_END";
        case Mfst::SURPRISE: return "SURPRISE";
        default: return "?";
        }
    }

    static void printTrace(Mfst& m, Mfst::RC_STEP rc)
    {
        char ruleBuf[MFST_DIAGN_MAXSIZE] = { 0 };
        char lentaBuf[MFST_DIAGN_MAXSIZE] = { 0 };
        char stackBuf[MFST_DIAGN_MAXSIZE] = { 0 };

        if (m.nrule >= 0)
        {
            GRB::Rule r = m.grebach.getRule(m.nrule);
            r.getCRule(ruleBuf, m.nrulechain);
        }
        else
        {
            strcpy_s(ruleBuf, MFST_DIAGN_MAXSIZE, "-");
        }

        m.getCLenta(lentaBuf, m.lenta_position);
        m.getCSt(stackBuf);

        std::cout << "Шаг " << m.step_counter
                  << " | Правило=" << ruleBuf
                  << " | Лента=" << lentaBuf
                  << " | Стек=" << stackBuf << std::endl;
    }
}
