#include "stdafx.h"
#include "In.h"
#include "Error.h"
#include <fstream>

#undef IN

namespace In
{
    static int table[256] = IN_CODE_TABLE;

    void WriteResultToOut(wchar_t outfile[], In::IN in) {
        std::ofstream outFile(outfile, std::ios::binary);
        if (!outFile.is_open()) {
            return;
        }
        for (int i = 0; i < in.size; i++) {
            outFile << in.text[i];
        }
        outFile.close();
    }

    IN getin(wchar_t infile[]) {
        IN result = { 0, 0, 0, nullptr, {0} };

        std::ifstream file(infile, std::ios::binary);
        if (!file.is_open()) {
            ERROR_THROW(110);
        }

        for (int i = 0; i < 256; ++i) {
            result.code[i] = table[i];
        }

        result.text = new unsigned char[IN_MAX_LEN_TEXT];
        if (result.text == nullptr) {
            file.close();
            ERROR_THROW(2);
        }

        int idx = 0;
        int col = 0;
        int line = 0;
        char ch = 0;

        // НОВОЕ: флаги для удаления лишних пробелов и пустых строк
        bool lastWasSpace = false;
        bool lastWasSeparator = false;
        // КОНЕЦ НОВОГО

        while (file.get(ch)) {
            unsigned char uch = static_cast<unsigned char>(ch);
            int rule = result.code[uch];

            // Проверка на недопустимый символ
            if (rule == In::F) {
                delete[] result.text;
                file.close();
                ERROR_THROW_IN(111, line, col);
            }

            if (idx >= IN_MAX_LEN_TEXT - 1) {
                delete[] result.text;
                file.close();
                ERROR_THROW(7);
            }

            // Игнорируем \r
            if (ch == '\r') {
                continue;
            }

            // НОВОЕ: обработка \n → сепаратор |
            if (ch == '\n') {
                // Если предыдущий тоже был сепаратором — пустая строка
                if (lastWasSeparator) {
                    ++result.ignor;   // считаем как игнорируемый
                    ++line;           // номер строки всё равно растёт
                    col = 0;
                    lastWasSpace = false;
                    continue;         // пропускаем второй |
                }
                result.text[idx++] = '|';
                ++result.lines;
                ++line;
                col = 0;
                lastWasSpace = false;
                lastWasSeparator = true;
                continue;
            }
            // КОНЕЦ НОВОГО

            // НОВОЕ: удаление лишних пробелов
            if (ch == ' ') {
                if (lastWasSpace) {
                    // Предыдущий был пробелом → лишний
                    ++result.ignor;
                    ++col;
                    continue;
                }
                result.text[idx++] = ' ';
                ++col;
                lastWasSpace = true;
                lastWasSeparator = false;
                continue;
            }
            // КОНЕЦ НОВОГО

            // Обработка символов через таблицу
            if (rule >= 0 && rule < 256) {
                // Замена символа (например, ' → 39)
                result.text[idx++] = static_cast<unsigned char>(rule);
                ++col;
            }
            else if (rule == In::T) {
                result.text[idx++] = uch;
                ++col;
            }
            else if (rule == In::I) {
                ++result.ignor;
                ++col;
            }

            // НОВОЕ: сброс флагов после обычного символа
            lastWasSpace = false;
            lastWasSeparator = false;
            // КОНЕЦ НОВОГО
        }

        // Добавляем | в конце, если его нет
        if (idx > 0 && result.text[idx - 1] != IN_CODE_SEPARATOR) {
            if (idx < IN_MAX_LEN_TEXT - 1) {
                result.text[idx++] = IN_CODE_SEPARATOR;
                ++result.lines;
            }
        }

        result.size = idx;
        result.text[idx] = '\0';

        file.close();
        return result;
    }
}