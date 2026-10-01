#include "FST.h"
#include <cstdarg>

namespace FST
{
    RELATION::RELATION(char c, short ns)
    {
        symbol = c;
        nnode = ns;
    }

    NODE::NODE()
    {
        n_relation = 0;
        relations = nullptr;
    }

    NODE::NODE(short n, RELATION rel, ...)
    {
        n_relation = n;
        relations = new RELATION[n];

        RELATION* p = &rel;
        for (short i = 0; i < n; i++)
            relations[i] = p[i];
    }

    FST::FST(char* s, short ns, NODE n, ...)
    {
        string = s;
        nstates = ns;
        position = 0;

        nodes = new NODE[ns];
        rstates = new short[ns];

        NODE* p = &n;
        for (short i = 0; i < ns; i++)
            nodes[i] = p[i];

        rstates[0] = 0;
        for (short i = 1; i < ns; i++)
            rstates[i] = -1;
    }

    bool execute(FST& fst)
    {
        short* rstates = new short[fst.nstates]; 
        short* rstates_next = new short[fst.nstates]; 

        rstates[0] = 0;
        for (short i = 1; i < fst.nstates; i++)
            rstates[i] = -1;

        fst.position = 0;
        char ch = fst.string[fst.position];

        while (ch != 0x00)
        {
            for (short i = 0; i < fst.nstates; i++)
                rstates_next[i] = -1;

            short k = 0; 

            for (short i = 0; i < fst.nstates; i++)
            {
                if (rstates[i] == -1) continue;

                short node = rstates[i];
                NODE& nd = fst.nodes[node];

                for (short j = 0; j < nd.n_relation; j++)
                {
                    if (nd.relations[j].symbol == ch)
                    {
                        short nn = nd.relations[j].nnode;
                        bool exists = false;
                        for (short t = 0; t < k; t++)
                            if (rstates_next[t] == nn) { exists = true; break; }
                        if (!exists)
                            rstates_next[k++] = nn;
                    }
                }
            }

            if (k == 0)
            {
                delete[] rstates;
                delete[] rstates_next;
                return false;
            }

            for (short i = 0; i < fst.nstates; i++)
                rstates[i] = rstates_next[i];

            fst.position++;
            ch = fst.string[fst.position];
        }

        for (short i = 0; i < fst.nstates; i++)
            fst.rstates[i] = rstates[i];
        
        bool ok = false;
        for (short i = 0; i < fst.nstates; i++)
            if (rstates[i] == fst.nstates - 1) { ok = true; break; }

        delete[] rstates;
        delete[] rstates_next;
        return ok;
    }
}