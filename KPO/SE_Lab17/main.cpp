#include "stdafx.h"
#include "Parm.h"
#include "Error.h"
#include "In.h"
#include "Log.h"
#include "LT.h"
#include "IT.h"
#include "Lex.h"
#include <fstream>

using namespace std;
using namespace Parm;

// ===== Запись ошибки в выходной файл =====
void WriteErrorToOut(wchar_t outfile[], Error::Error error) {
    std::ofstream outFile(outfile, std::ios::app);
    if (!outFile.is_open()) {
        return;
    }

    outFile << std::endl;
    outFile << "---- ОБНАРУЖЕНА ОШИБКА ----" << std::endl;
    outFile << "Код ошибки: " << error.id << std::endl;
    outFile << "Сообщение:  " << error.message << std::endl;
    if (error.inext.line >= 0) {
        outFile << "Строка: " << error.inext.line + 1
            << ", позиция: " << error.inext.col + 1 << std::endl;
    }
    outFile << std::endl;

    outFile.close();
}

int _tmain(int argc, _TCHAR* argv[]) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");

    system("chcp 1251 > nul");

    PARM parms;
    Log::LOG log = Log::INITLOG;

    // Таблицы лексем и идентификаторов
    LT::LexTable lextable;
    IT::IdTable  idtable;

    try {
        parms = getparm(argc, argv);
        cout << "Параметры определены успешно." << endl;

        log = Log::getlog(parms.log);
        Log::WriteLog(log);
        Log::WriteParm(log, parms);

        // Создаём пустой выходной файл
        std::ofstream createFile(parms.out, std::ios::binary);
        createFile.close();

        // ============ ЭТАП 1: Подготовка текста ============
        In::IN input_data = In::getin(parms.in);
        Log::WriteIn(log, input_data);

        // Запись преобразованного текста в -out (убрали тестовую WriteLine)
        WriteResultToOut(parms.out, input_data);

        // ============ ЭТАП 2: Лексический анализ ============
        lextable = LT::Create(LT_MAXSIZE);
        idtable = IT::Create(TI_MAXSIZE);

        Lex::LexicalAnalysis(input_data, lextable, idtable);

        // ============ ЭТАП 3: Вывод таблиц в лог ============
        Log::WriteLexTable(log, lextable);
        Log::WriteIdTable(log, idtable);

        cout << "Лексический анализ выполнен успешно." << endl;
        cout << "Лексем:          " << lextable.size << endl;
        cout << "Идентификаторов: " << idtable.size << endl;
        cout << "Программа успешно завершила работу. Лог сформирован." << endl;

        // ============ ОЧИСТКА ============
        LT::Delete(lextable);
        IT::Delete(idtable);
        delete[] input_data.text;
        Log::Close(log);
    }
    catch (Error::Error e) {
        // Запись ошибки в -out
        if (e.id >= 111 && e.id <= 209) {
            WriteErrorToOut(parms.out, e);
        }

        // Запись ошибки в лог
        Log::WriteError(log, e);
        Log::Close(log);

        // Вывод в консоль
        cout << "Ошибка: " << e.id << ": " << e.message << endl;
        if (e.inext.line >= 0) {
            cout << "Строка: " << e.inext.line + 1
                << ", позиция: " << e.inext.col + 1 << endl;
        }

        return e.id;
    }
    catch (...) {
        cout << "Неизвестная ошибка" << endl;
        Log::Close(log);
        return 1;
    }

    return 0;
}