#include "stdafx.h"
#include "Log.h"
#include "Error.h"
#include "Parm.h"
#include "In.h"
#include "LT.h"
#include "IT.h"
#include <cstdarg>
#include <ctime>
#include <iostream>

namespace Log
{
    LOG getlog(wchar_t logfile[])
    {
        LOG log = INITLOG;
        wcscpy_s(log.logfile, logfile);

        log.stream = new std::ofstream(logfile, std::ios::out);

        if (!log.stream->is_open())
        {
            delete log.stream;
            log.stream = NULL;
            ERROR_THROW(112);
        }

        return log;
    }

    void WriteLine(LOG log, char* c, ...)
    {
        if (log.stream == NULL || !log.stream->is_open()) return;

        *log.stream << c;

        va_list args;
        va_start(args, c);

        char* ptr = va_arg(args, char*);
        while (ptr != NULL) 
        {
            *log.stream << ptr;
            ptr = va_arg(args, char*);
        }

        va_end(args);
        *log.stream << std::endl; 
    }

    void WriteLine(LOG log, wchar_t* c, ...)
    {
        if (log.stream == NULL || !log.stream->is_open()) return;

        int i = 0;
        while (c[i] != L'\0') *log.stream << (char)c[i++];

        va_list args;
        va_start(args, c);

        wchar_t* ptr = va_arg(args, wchar_t*);
        while (ptr != NULL)
        {
            i = 0;
            while (ptr[i] != L'\0') *log.stream << (char)ptr[i++];
            ptr = va_arg(args, wchar_t*);
        }

        va_end(args);
        *log.stream << std::endl;
    }

    void WriteLog(LOG log)
    {
        if (log.stream == NULL || !log.stream->is_open()) return;

        time_t t = time(NULL);
        tm t_str;
        localtime_s(&t_str, &t);

        char time_buf[100];
        strftime(time_buf, sizeof(time_buf), "%d.%m.%Y %H:%M:%S", &t_str);

        *log.stream << "---- Протокол компиляции ----" << std::endl;
        *log.stream << "Дата и время создания: " << time_buf << std::endl << std::endl;
    }

    void WriteParm(LOG log, Parm::PARM parm)
    {
        if (log.stream == NULL || !log.stream->is_open()) return;

        *log.stream << "---- Параметры запуска ----" << std::endl;

        *log.stream << "-in:  ";
        int i = 0; while (parm.in[i] != L'\0') *log.stream << (char)parm.in[i++];
        *log.stream << std::endl;

        *log.stream << "-out: ";
        i = 0; while (parm.out[i] != L'\0') *log.stream << (char)parm.out[i++];
        *log.stream << std::endl;

        *log.stream << "-log: ";
        i = 0; while (parm.log[i] != L'\0') *log.stream << (char)parm.log[i++];
        *log.stream << std::endl << std::endl;
    }

    void WriteIn(LOG log, In::IN in)
    {
        if (log.stream == NULL || !log.stream->is_open()) return;

        *log.stream << "---- Результаты анализа входного файла ----" << std::endl;
        *log.stream << "Успешно обработано символов (size):  " << in.size << std::endl;
        *log.stream << "Всего строк (lines):                 " << in.lines << std::endl;
        *log.stream << "Пропущено символов (ignor):          " << in.ignor << std::endl << std::endl;
    }

    void WriteError(LOG log, Error::Error error)
    {
        if (log.stream != NULL && log.stream->is_open())
        {
            *log.stream << "---- ОБНАРУЖЕНА ОШИБКА ----" << std::endl;
            *log.stream << "Код ошибки: " << error.id << std::endl;
            *log.stream << "Сообщение:  " << error.message << std::endl;
            if (error.inext.line >= 0)
            {
                *log.stream << "Строка: " << error.inext.line + 1 << ", позиция: " << error.inext.col + 1 << std::endl;
            }
            *log.stream << std::endl;
        }
        else
        {
            std::cout << "Ошибка " << error.id << ": " << error.message << std::endl;
        }
    }

    void Close(LOG log)
    {
        if (log.stream != NULL)
        {
            if (log.stream->is_open())
            {
                log.stream->close();
            }
            delete log.stream; 
        }
    }

    void WriteLexTable(LOG log, LT::LexTable lextable)
    {
        if (log.stream == NULL || !log.stream->is_open()) return;

        *log.stream << "---- Таблица лексем ----" << std::endl;

        int curLine = -1;

        for (int i = 0; i < lextable.size; i++) {
            LT::Entry e = lextable.table[i];

            // Разбивка по строкам
            if (e.sn != curLine) {
                curLine = e.sn;
                *log.stream << std::endl;
                // Номер строки с ведущим нулём
                if (curLine < 10) *log.stream << "0";
                *log.stream << curLine << "  ";
            }

            *log.stream << e.lexema[0];
        }

        *log.stream << std::endl << std::endl;
    }

    void WriteIdTable(LOG log, IT::IdTable idtable)
    {
        if (log.stream == NULL || !log.stream->is_open()) return;

        *log.stream << "---- Таблица идентификаторов ----" << std::endl;

        for (int i = 0; i < idtable.size; i++) {
            IT::Entry e = idtable.table[i];

            *log.stream << i << ": ";
            for (int j = 0; j < ID_MAXSIZE && e.id[j] != 0; j++) {
                *log.stream << e.id[j];
            }
            *log.stream << "  (sn=" << e.sn << ")" << std::endl;
        }

        *log.stream << std::endl;
    }
}