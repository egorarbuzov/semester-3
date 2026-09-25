#include "stdafx.h"
#include "Parm.h"
#include "Error.h"

using namespace std;

Parm::PARM Parm::getparm(int argc, _TCHAR* argv[]) {
    Parm::PARM parm;
    parm.in[0] = L'\0';
    parm.out[0] = L'\0';
    parm.log[0] = L'\0';

    bool inFound = false;

    if (argc == 1) {
        ERROR_THROW(8);
    }

    for (int i = 1; i < argc; i++) {
        wstring arg = argv[i];

        if (arg.find(PARM_IN) == 0) {
            wstring temp = arg.substr(wcslen(PARM_IN));
            if (temp.empty()) { ERROR_THROW(104); }
            if (temp.length() >= PARM_MAX_SIZE) { ERROR_THROW(104); }
            wcscpy_s(parm.in, temp.c_str());
            inFound = true;
            wcout << L"Found -in: " << parm.in << endl;
        }
        else if (arg.find(PARM_OUT) == 0) {
            wstring temp = arg.substr(wcslen(PARM_OUT));
            if (temp.empty()) { ERROR_THROW(104); }
            if (temp.length() >= PARM_MAX_SIZE) { ERROR_THROW(104); }
            wcscpy_s(parm.out, temp.c_str());
            wcout << L"Found -out: " << parm.out << endl;
        }
        else if (arg.find(PARM_LOG) == 0) {
            wstring temp = arg.substr(wcslen(PARM_LOG));
            if (temp.empty()) { ERROR_THROW(104); }
            if (temp.length() >= PARM_MAX_SIZE) { ERROR_THROW(104); }
            wcscpy_s(parm.log, temp.c_str());
            wcscat_s(parm.log, L".log");
            wcout << L"Found -log: " << parm.log << endl;
        }
        else {
            ERROR_THROW(9);  
        }
    }

    if (!inFound) {
        ERROR_THROW(100); 
    }

    if (wcslen(parm.out) == 0) {
        wcscpy_s(parm.out, parm.in);
        wchar_t* dot = wcsrchr(parm.out, L'.');
        if (dot) {
            *dot = L'\0';
        }
        wcscat_s(parm.out, PARM_OUT_DEFAULT_EXT);
        wcout << L"out: " << parm.out << endl;
    }

    if (wcslen(parm.log) == 0) {
        wcscpy_s(parm.log, parm.in);
        wchar_t* dot = wcsrchr(parm.log, L'.');
        if (dot) {
            *dot = L'\0';
        }
        wcscat_s(parm.log, PARM_LOG_DEFAULT_EXT);
        wcout << L"log: " << parm.log << endl;
    }

    return parm;
}