#include "stdafx.h"
#include "Parm.h"
#include "Error.h"
#include "In.h"
#include "Log.h" 

using namespace std;
using namespace Parm;

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
        outFile << "Строка: " << error.inext.line + 1 << ", позиция: " << error.inext.col + 1 << std::endl;
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

    try {
        parms = getparm(argc, argv);

        cout << "Параметры определены успешно." << endl;

        log = Log::getlog(parms.log);

        Log::WriteLog(log);
        Log::WriteParm(log, parms);

        std::ofstream createFile(parms.out, std::ios::binary);
        createFile.close();

        In::IN input_data = In::getin(parms.in);

        Log::WriteIn(log, input_data);

        Log::WriteLine(log, (char*)"Тестирование функции WriteLine: ", (char*)"успешно ", (char*)"завершено.", NULL);
        WriteResultToOut(parms.out, input_data);
        cout << "Программа успешно завершила работу. Лог сформирован." << endl;

        delete[] input_data.text;
        Log::Close(log);
    }
    catch (Error::Error e) {
        if (e.id == 111) {
            WriteErrorToOut(parms.out, e);
        }
        Log::WriteError(log, e);
        Log::Close(log);

        cout << "Ошибка: " << e.id << ": " << e.message << endl;
        if (e.inext.line >= 0) {
            cout << "Строка: " << e.inext.line + 1 << " Позиция: " << e.inext.col + 1 << endl;
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