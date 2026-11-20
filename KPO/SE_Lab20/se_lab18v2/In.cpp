#include "stdafx.h"
#include "In.h"
#include "Error.h"
#include <fstream>

#undef IN

namespace In
{
    static int table[256] = IN_CODE_TABLE;

    void WriteResultToOut(wchar_t outfile[], In::IN in)
    {
        std::ofstream outFile(outfile, std::ios::binary);
        if (!outFile.is_open())
            ERROR_THROW(113);

        for (int i = 0; i < in.size; ++i)
            outFile.put((char)in.text[i]);

        outFile.close();
    }

    IN getin(wchar_t infile[])
    {
        IN result = { 0, 0, 0, nullptr, nullptr, { 0 } };

        std::ifstream file(infile, std::ios::binary);
        if (!file.is_open())
            ERROR_THROW(110);

        for (int i = 0; i < 256; ++i)
            result.code[i] = table[i];

        result.text = new unsigned char[IN_MAX_LEN_TEXT];
        result.origCol = new int[IN_MAX_LEN_TEXT];

        if (result.text == nullptr || result.origCol == nullptr)
        {
            delete[] result.text;
            delete[] result.origCol;
            file.close();
            ERROR_THROW(2);
        }

        int idx = 0;
        int col = 0;
        int line = 1;
        char ch = 0;

        bool lastWasSpace = false;
        bool lastWasSeparator = false;
        bool atStart = true;
        bool inString = false;

        while (file.get(ch))
        {
            unsigned char uch = static_cast<unsigned char>(ch);
            int rule = result.code[uch];

            if (ch == '\r')
                continue;

            // String contents may contain any byte except a line break.
            if (inString)
            {
                if (ch == '\n')
                {
                    delete[] result.text;
                    delete[] result.origCol;
                    file.close();
                    ERROR_THROW_IN(202, line, col);
                }

                if (idx >= IN_MAX_LEN_TEXT - 1)
                {
                    delete[] result.text;
                    delete[] result.origCol;
                    file.close();
                    ERROR_THROW(7);
                }

                result.text[idx] = uch;
                result.origCol[idx] = col;
                ++idx;
                ++col;

                if (ch == '\'')
                    inString = false;

                lastWasSpace = false;
                lastWasSeparator = false;
                atStart = false;
                continue;
            }

            if (rule == In::F)
            {
                delete[] result.text;
                delete[] result.origCol;
                file.close();
                ERROR_THROW_IN(111, line, col);
            }

            if (idx >= IN_MAX_LEN_TEXT - 1)
            {
                delete[] result.text;
                delete[] result.origCol;
                file.close();
                ERROR_THROW(7);
            }

            // Opening quote of a string literal.
            if (ch == '\'')
            {
                inString = true;
                result.text[idx] = uch;
                result.origCol[idx] = col;
                ++idx;
                ++col;
                lastWasSpace = false;
                lastWasSeparator = false;
                atStart = false;
                continue;
            }

            if (ch == '\n')
            {
                if (atStart && idx == 0)
                {
                    ++line;
                    col = 0;
                    lastWasSpace = false;
                    lastWasSeparator = true;
                    continue;
                }

                if (lastWasSeparator)
                {
                    ++result.ignor;
                    ++line;
                    col = 0;
                    lastWasSpace = false;
                    continue;
                }

                result.text[idx] = IN_CODE_SEPARATOR;
                result.origCol[idx] = col;
                ++idx;
                ++result.lines;
                ++line;
                col = 0;
                lastWasSpace = false;
                lastWasSeparator = true;
                atStart = false;
                continue;
            }

            if (ch == ' ' || ch == '\t')
            {
                if (lastWasSpace || lastWasSeparator || atStart)
                {
                    ++result.ignor;
                    ++col;
                    continue;
                }

                result.text[idx] = ' ';
                result.origCol[idx] = col;
                ++idx;
                ++col;
                lastWasSpace = true;
                lastWasSeparator = false;
                atStart = false;
                continue;
            }

            result.text[idx] = uch;
            result.origCol[idx] = col;
            ++idx;
            ++col;

            lastWasSpace = false;
            lastWasSeparator = false;
            atStart = false;
        }

        if (inString)
        {
            delete[] result.text;
            delete[] result.origCol;
            file.close();
            ERROR_THROW_IN(202, line, col);
        }

        if (idx > 0 && result.text[idx - 1] != IN_CODE_SEPARATOR)
        {
            if (idx < IN_MAX_LEN_TEXT - 1)
            {
                result.text[idx] = IN_CODE_SEPARATOR;
                result.origCol[idx] = col;
                ++idx;
                ++result.lines;
            }
        }

        result.size = idx;
        result.text[idx] = '\0';
        result.origCol[idx] = -1;

        file.close();
        return result;
    }
}
