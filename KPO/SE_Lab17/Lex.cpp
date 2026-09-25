#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#include "Lex.h"
#include "Error.h"
#include <cstring>

namespace Lex
{
    // Проверить: является ли символ буквой
    bool isLetter(char c) {
        return (c >= 'a' && c <= 'z');
    }

    // Проверить: является ли символ цифрой
    bool isDigit(char c) {
        return (c >= '0' && c <= '9');
    }

    // Сравнить строку с ключевым словом
    bool isKeyword(const char* buf, int len, const char* keyword) {
        if ((int)strlen(keyword) != len) return false;
        return strncmp(buf, keyword, len) == 0;
    }

    // Главная функция
    void LexicalAnalysis(In::IN& in, LT::LexTable& lextable, IT::IdTable& idtable)
    {
        int line = 1;                 // текущая строка
        int i = 0;                    // позиция в тексте

        while (i < in.size) {
            char ch = (char)in.text[i];

            // === СЕПАРАТОР СТРОКИ ===
            if (ch == '|') {
                line++;
                i++;
                continue;
            }

            // === ПРОБЕЛ ===
            if (ch == ' ') {
                i++;
                continue;
            }

            // === ОДНОСИМВОЛЬНЫЕ ЛЕКСЕМЫ ===
            if (ch == ';' || ch == ',' || ch == '{' || ch == '}' ||
                ch == '(' || ch == ')') {
                LT::Entry entry;
                entry.lexema[0] = ch;
                entry.sn = line;
                entry.idxTI = LT_TI_NULLIDX;
                LT::Add(lextable, entry);
                i++;
                continue;
            }

            // === ОПЕРАТОРЫ + - * / ===
            if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
                LT::Entry entry;
                entry.lexema[0] = LEX_PLUS;  // все 'v'
                entry.sn = line;
                entry.idxTI = LT_TI_NULLIDX;
                LT::Add(lextable, entry);
                i++;
                continue;
            }

            // === ЗНАК ПРИСВАИВАНИЯ = ===
            if (ch == '=') {
                LT::Entry entry;
                entry.lexema[0] = '=';
                entry.sn = line;
                entry.idxTI = LT_TI_NULLIDX;
                LT::Add(lextable, entry);
                i++;
                continue;
            }

            // === СТРОКОВЫЙ ЛИТЕРАЛ '...' ===
            if (ch == '\'') {
                i++;  // пропускаем открывающую '
                char lit[256];
                int litLen = 0;

                while (i < in.size && in.text[i] != '\'') {
                    if (litLen < 255) lit[litLen++] = in.text[i];
                    i++;
                }

                if (i >= in.size) {
                    ERROR_THROW_IN(202, line, i);  // незакрытая строка
                }
                i++;  // пропускаем закрывающую '

                LT::Entry entry;
                entry.lexema[0] = LEX_LITERAL;
                entry.sn = line;
                entry.idxTI = LT_TI_NULLIDX;
                LT::Add(lextable, entry);
                continue;
            }

            // === ЧИСЛОВОЙ ЛИТЕРАЛ ===
            if (isDigit(ch)) {
                while (i < in.size && isDigit((char)in.text[i])) i++;

                LT::Entry entry;
                entry.lexema[0] = LEX_LITERAL;
                entry.sn = line;
                entry.idxTI = LT_TI_NULLIDX;
                LT::Add(lextable, entry);
                continue;
            }

            // === ИДЕНТИФИКАТОР ИЛИ КЛЮЧЕВОЕ СЛОВО ===
            if (isLetter(ch)) {
                char buf[16] = { 0 };
                int len = 0;

                while (i < in.size && isLetter((char)in.text[i]) && len < 15) {
                    buf[len++] = in.text[i];
                    i++;
                }

                // Проверяем ключевые слова
                if (isKeyword(buf, len, "integer") || isKeyword(buf, len, "string")) {
                    LT::Entry entry;
                    entry.lexema[0] = LEX_INTEGER;
                    entry.sn = line;
                    entry.idxTI = LT_TI_NULLIDX;
                    LT::Add(lextable, entry);
                }
                else if (isKeyword(buf, len, "function")) {
                    LT::Entry entry;
                    entry.lexema[0] = LEX_FUNCTION;
                    entry.sn = line;
                    entry.idxTI = LT_TI_NULLIDX;
                    LT::Add(lextable, entry);
                }
                else if (isKeyword(buf, len, "declare")) {
                    LT::Entry entry;
                    entry.lexema[0] = LEX_DECLARE;
                    entry.sn = line;
                    entry.idxTI = LT_TI_NULLIDX;
                    LT::Add(lextable, entry);
                }
                else if (isKeyword(buf, len, "return")) {
                    LT::Entry entry;
                    entry.lexema[0] = LEX_RETURN;
                    entry.sn = line;
                    entry.idxTI = LT_TI_NULLIDX;
                    LT::Add(lextable, entry);
                }
                else if (isKeyword(buf, len, "print")) {
                    LT::Entry entry;
                    entry.lexema[0] = LEX_PRINT;
                    entry.sn = line;
                    entry.idxTI = LT_TI_NULLIDX;
                    LT::Add(lextable, entry);
                }
                else if (isKeyword(buf, len, "main")) {
                    LT::Entry entry;
                    entry.lexema[0] = LEX_MAIN;
                    entry.sn = line;
                    entry.idxTI = LT_TI_NULLIDX;
                    LT::Add(lextable, entry);
                }
                else {
                    // Это идентификатор!
                    if (len > ID_MAXSIZE) {
                        ERROR_THROW_IN(200, line, i - len);  // слишком длинный
                    }

                    // Ищем в таблице идентификаторов
                    char id[ID_MAXSIZE] = { 0 };
                    strncpy(id, buf, ID_MAXSIZE);

                    int idx = IT::IsId(idtable, id);
                    if (idx == TI_NULLIDX) {
                        // Новый идентификатор
                        IT::Entry idEntry;
                        strncpy(idEntry.id, buf, ID_MAXSIZE);
                        idEntry.sn = line;
                        IT::Add(idtable, idEntry);
                        idx = idtable.size - 1;
                    }

                    // Добавляем лексему
                    LT::Entry entry;
                    entry.lexema[0] = LEX_ID;
                    entry.sn = line;
                    entry.idxTI = idx;
                    LT::Add(lextable, entry);
                }
                continue;
            }

            // === НЕИЗВЕСТНЫЙ СИМВОЛ ===
            ERROR_THROW_IN(208, line, i);  // неизвестная лексема
        }
    }
}