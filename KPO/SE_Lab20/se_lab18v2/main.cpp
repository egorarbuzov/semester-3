#include "stdafx.h"
#include "Error.h"
#include "Parm.h"
#include "In.h"
#include "Lex.h"
#include "Log.h"
#include "LT.h"
#include "IT.h"
#include "GRB.h"
#include "MFST.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
    setlocale(LC_ALL, "rus");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Log::LOG log = Log::INITLOG;
    In::IN in = { 0, 0, 0, nullptr, nullptr, { 0 } };
    LT::LexTable lextable = { 0, 0, nullptr };
    IT::IdTable idtable = { 0, 0, nullptr };

    auto cleanup = [&]()
    {
        if (in.text != nullptr)
        {
            delete[] in.text;
            in.text = nullptr;
        }
        if (in.origCol != nullptr)
        {
            delete[] in.origCol;
            in.origCol = nullptr;
        }
        if (lextable.table != nullptr)
            LT::Delete(lextable);
        if (idtable.table != nullptr)
            IT::Delete(idtable);
    };

    try
    {
        Parm::PARM parm = Parm::getparm(argc, argv);
        log = Log::getlog(parm.log);

        Log::WriteLog(log);
        Log::WriteParm(log, parm);

        in = In::getin(parm.in);
        Log::WriteIn(log, in);
        In::WriteResultToOut(parm.out, in);

        lextable = LT::Create(LT_MAXSIZE);
        idtable = IT::Create(TI_MAXSIZE);

        Lex::LexicalAnalysis(in, lextable, idtable);

        Log::WriteLexTable(log, lextable);
        Log::WriteIdTable(log, idtable);

        cout << "--- Syntax analysis ---" << endl;
        GRB::Greibach greibach = GRB::getGreibach();
        LEX::LEX lex(lextable, idtable);
        MFST::Mfst mfst(lex, greibach);
        bool ok = mfst.start();

        if (!ok)
        {
            char dbuf[MFST_DIAGN_MAXSIZE];
            for (short di = 0; di < MFST_DIAGN_NUMBER; ++di)
            {
                mfst.getDiagnosis(di, dbuf);
                if (dbuf[0] != 0)
                    Log::WriteLine(log, dbuf, (char*)NULL);
            }
        }

        cleanup();
        Log::Close(log);
        return ok ? 0 : 1;
    }
    catch (Error::Error e)
    {
        cout << "Error " << e.id << ": " << e.message << endl;
        if (e.inext.line >= 0)
            cout << "line: " << e.inext.line
                 << ", column: " << e.inext.col + 1 << endl;

        if (log.stream != nullptr)
        {
            Log::WriteError(log, e);
            Log::Close(log);
        }

        cleanup();
        return 1;
    }
    catch (const std::exception& e)
    {
        cout << "Fatal error: " << e.what() << endl;
        if (log.stream != nullptr)
            Log::Close(log);
        cleanup();
        return 1;
    }
}
