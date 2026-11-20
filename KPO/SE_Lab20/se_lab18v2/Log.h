#pragma once
#include <fstream>
#include "In.h"
#include "LT.h"
#include "IT.h"
#include "Parm.h"
#include "Error.h"

namespace Log 
{
    struct LOG      
    {
        wchar_t logfile[PARM_MAX_SIZE]; 
        std::ofstream* stream;     
    };

    static const LOG INITLOG{ L"", NULL };  

    LOG getlog(wchar_t logfile[]);              // сформировать структуру 
    void WriteLine(LOG log, char* c, ...);   
    void WriteLine(LOG log, wchar_t* c, ...);   // вывести в протокол
    void WriteLog(LOG log);                     // вывести в протокол 
    void WriteParm(LOG log, Parm::PARM parm);   // вывести в протокол
    void WriteIn(LOG log, In::IN in);           // вывести в протокол 
    void WriteError(LOG log, Error::Error error); // вывести в протокол
    void Close(LOG log);                        // закрыть поток
    void WriteLexTable(LOG log, LT::LexTable lextable);
    void WriteIdTable(LOG log, IT::IdTable idtable);
};