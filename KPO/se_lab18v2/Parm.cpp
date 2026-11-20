#include "stdafx.h"
#include "Parm.h"
#include "Error.h"

using namespace std;

namespace
{
    bool hasExtension(const wchar_t* path)
    {
        const wchar_t* dot = wcsrchr(path, L'.');
        if (dot == nullptr)
            return false;

        const wchar_t* slash1 = wcsrchr(path, L'\\');
        const wchar_t* slash2 = wcsrchr(path, L'/');
        const wchar_t* slash = (slash1 > slash2) ? slash1 : slash2;

        return slash == nullptr || dot > slash;
    }

    void replaceExtension(wchar_t* path, const wchar_t* extension)
    {
        wchar_t* dot = wcsrchr(path, L'.');
        wchar_t* slash1 = wcsrchr(path, L'\\');
        wchar_t* slash2 = wcsrchr(path, L'/');
        wchar_t* slash = (slash1 > slash2) ? slash1 : slash2;

        if (dot != nullptr && (slash == nullptr || dot > slash))
            *dot = L'\0';

        wcscat_s(path, PARM_MAX_SIZE, extension);
    }
}

Parm::PARM Parm::getparm(int argc, _TCHAR* argv[])
{
    Parm::PARM parm;
    parm.in[0] = L'\0';
    parm.out[0] = L'\0';
    parm.log[0] = L'\0';

    bool inFound = false;

    if (argc == 1)
        ERROR_THROW(8);

    for (int i = 1; i < argc; ++i)
    {
        wstring arg = argv[i];

        if (arg.find(PARM_IN) == 0)
        {
            wstring temp = arg.substr(wcslen(PARM_IN));
            if (temp.empty() || temp.length() >= PARM_MAX_SIZE)
                ERROR_THROW(104);
            wcscpy_s(parm.in, temp.c_str());
            inFound = true;
        }
        else if (arg.find(PARM_OUT) == 0)
        {
            wstring temp = arg.substr(wcslen(PARM_OUT));
            if (temp.empty() || temp.length() >= PARM_MAX_SIZE)
                ERROR_THROW(104);
            wcscpy_s(parm.out, temp.c_str());
        }
        else if (arg.find(PARM_LOG) == 0)
        {
            wstring temp = arg.substr(wcslen(PARM_LOG));
            if (temp.empty() || temp.length() >= PARM_MAX_SIZE)
                ERROR_THROW(104);
            wcscpy_s(parm.log, temp.c_str());
            if (!hasExtension(parm.log))
                wcscat_s(parm.log, PARM_LOG_DEFAULT_EXT);
        }
        else
        {
            ERROR_THROW(9);
        }
    }

    if (!inFound)
        ERROR_THROW(100);

    if (wcslen(parm.out) == 0)
    {
        wcscpy_s(parm.out, parm.in);
        replaceExtension(parm.out, PARM_OUT_DEFAULT_EXT);
    }

    if (wcslen(parm.log) == 0)
    {
        wcscpy_s(parm.log, parm.in);
        replaceExtension(parm.log, PARM_LOG_DEFAULT_EXT);
    }

    return parm;
}
