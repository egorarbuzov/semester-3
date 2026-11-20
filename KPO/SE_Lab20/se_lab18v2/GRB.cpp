#include "stdafx.h"
#include "GRB.h"
#include <cstring>

namespace GRB
{
    Rule::Chain::Chain(short psize, GRBALPHABET s, ...)
    {
        size = psize;
        nt = new GRBALPHABET[size > 0 ? size : 1];
        if (size > 0) nt[0] = s;

        va_list args;
        va_start(args, s);
        for (short i = 1; i < size; ++i)
            nt[i] = (GRBALPHABET)va_arg(args, int);
        va_end(args);
    }

    char* Rule::Chain::getCChain(char* b)
    {
        if (b == nullptr) return b;
        for (short i = 0; i < size; ++i)
            b[i] = alphabet_to_char(nt[i]);
        b[size] = 0;
        return b;
    }

    Rule::Rule(GRBALPHABET pnn, int piderror, short psize, Chain c, ...)
    {
        nn = pnn;
        iderror = piderror;
        size = psize;
        chains = new Chain[size > 0 ? size : 1];
        if (size > 0) chains[0] = c;

        va_list args;
        va_start(args, c);
        for (short i = 1; i < size; ++i)
            chains[i] = va_arg(args, Chain);
        va_end(args);
    }

    char* Rule::getCRule(char* b, short nchain)
    {
        if (b == nullptr) return b;
        if (nchain < 0 || nchain >= size)
        {
            strcpy_s(b, GRB_MAX_RULE_STRING, "?->?");
            return b;
        }

        char buf[GRB_MAX_RULE_STRING] = { 0 };
        b[0] = Chain::alphabet_to_char(nn);
        b[1] = '-';
        b[2] = '>';
        b[3] = 0;
        chains[nchain].getCChain(buf);
        strcat_s(b, GRB_MAX_RULE_STRING, buf);
        return b;
    }

    short Rule::getNextChain(GRBALPHABET t, Chain& pchain, short j)
    {
        if (j < 0) j = 0;
        while (j < size && chains[j].size > 0 && chains[j].nt[0] != t)
            ++j;
        if (j < size)
        {
            pchain = chains[j];
            return j;
        }
        return -1;
    }

    Greibach::Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottomT, short psize, Rule r, ...)
    {
        startN = pstartN;
        stbottomT = pstbottomT;
        size = psize;
        rules = new Rule[size > 0 ? size : 1];
        if (size > 0) rules[0] = r;

        va_list args;
        va_start(args, r);
        for (short i = 1; i < size; ++i)
            rules[i] = va_arg(args, Rule);
        va_end(args);
    }

    short Greibach::getRule(GRBALPHABET pnn, Rule& prule)
    {
        for (short k = 0; k < size; ++k)
        {
            if (rules[k].nn == pnn)
            {
                prule = rules[k];
                return k;
            }
        }
        return -1;
    }

    Rule Greibach::getRule(short n)
    {
        if (n < 0 || n >= size) return Rule();
        return rules[n];
    }

    Greibach getGreibach()
    {
        return Greibach(
            Rule::Chain::N('S'), Rule::Chain::T('$'), 6,

            Rule(Rule::Chain::N('S'), GRB_ERROR_SERIES + 0, 5,
                Rule::Chain(10, Rule::Chain::T('t'), Rule::Chain::T('f'), Rule::Chain::T('i'), Rule::Chain::T('('), Rule::Chain::N('F'), Rule::Chain::T(')'), Rule::Chain::T('{'), Rule::Chain::N('N'), Rule::Chain::T(';'), Rule::Chain::N('S')), // Функция с параметрами, точка с запятой после }, продолжение программы
                Rule::Chain(9, Rule::Chain::T('t'), Rule::Chain::T('f'), Rule::Chain::T('i'), Rule::Chain::T('('), Rule::Chain::T(')'), Rule::Chain::T('{'), Rule::Chain::N('N'), Rule::Chain::T(';'), Rule::Chain::N('S')), // Функция без параметров, точка с запятой после }, продолжение программы
                Rule::Chain(5, Rule::Chain::T('m'), Rule::Chain::T('{'), Rule::Chain::N('N'), Rule::Chain::T(';'), Rule::Chain::N('S')), // Главная функция
                Rule::Chain(4, Rule::Chain::T('m'), Rule::Chain::T('{'), Rule::Chain::N('N'), Rule::Chain::N('S')), // Главная функция
                Rule::Chain(1, Rule::Chain::T('$')) // Конец программы
            ),

            Rule(Rule::Chain::N('N'), GRB_ERROR_SERIES + 1, 6,
                Rule::Chain(5, Rule::Chain::T('d'), Rule::Chain::T('t'), Rule::Chain::T('i'), Rule::Chain::T(';'), Rule::Chain::N('N')), // Объявление переменной, продолжение блока
                Rule::Chain(9, Rule::Chain::T('d'), Rule::Chain::T('t'), Rule::Chain::T('f'), Rule::Chain::T('i'), Rule::Chain::T('('), Rule::Chain::N('F'), Rule::Chain::T(')'), Rule::Chain::T(';'), Rule::Chain::N('N')), // Объявление функции с параметрами, продолжение блока
                Rule::Chain(5, Rule::Chain::T('i'), Rule::Chain::T('='), Rule::Chain::N('E'), Rule::Chain::T(';'), Rule::Chain::N('N')), // Присваивание, продолжение блока
                Rule::Chain(4, Rule::Chain::T('p'), Rule::Chain::N('E'), Rule::Chain::T(';'), Rule::Chain::N('N')), // Вывод
                Rule::Chain(4, Rule::Chain::T('r'), Rule::Chain::N('E'), Rule::Chain::T(';'), Rule::Chain::N('N')), // Возврат
                Rule::Chain(1, Rule::Chain::T('}')) // Конец
            ),

            Rule(Rule::Chain::N('E'), GRB_ERROR_SERIES + 2, 8,
                Rule::Chain(1, Rule::Chain::T('i')), // Переменная
                Rule::Chain(1, Rule::Chain::T('l')), // Литерал
                Rule::Chain(4, Rule::Chain::T('i'), Rule::Chain::T('('), Rule::Chain::N('W'), Rule::Chain::T(')')), // Вызов функции
                Rule::Chain(3, Rule::Chain::T('('), Rule::Chain::N('E'), Rule::Chain::T(')')), // Скобки
                Rule::Chain(2, Rule::Chain::T('i'), Rule::Chain::N('M')), // Переменная с операцией
                Rule::Chain(2, Rule::Chain::T('l'), Rule::Chain::N('M')), // Литерал с операцией
                Rule::Chain(5, Rule::Chain::T('i'), Rule::Chain::T('('), Rule::Chain::N('W'), Rule::Chain::T(')'), Rule::Chain::N('M')), // Вызов функции с операцией
                Rule::Chain(4, Rule::Chain::T('('), Rule::Chain::N('E'), Rule::Chain::T(')'), Rule::Chain::N('M')) // Скобки с операцией
            ),

            Rule(Rule::Chain::N('M'), GRB_ERROR_SERIES + 4, 4,
                Rule::Chain(2, Rule::Chain::T('+'), Rule::Chain::N('E')), // Сложение
                Rule::Chain(2, Rule::Chain::T('-'), Rule::Chain::N('E')), // Вычитание
                Rule::Chain(2, Rule::Chain::T('*'), Rule::Chain::N('E')), // Умножение
                Rule::Chain(2, Rule::Chain::T('/'), Rule::Chain::N('E'))  // Деление
            ),

            Rule(Rule::Chain::N('F'), GRB_ERROR_SERIES + 3, 2,
                Rule::Chain(2, Rule::Chain::T('t'), Rule::Chain::T('i')), // Один параметр
                Rule::Chain(4, Rule::Chain::T('t'), Rule::Chain::T('i'), Rule::Chain::T(','), Rule::Chain::N('F')) // Несколько параметров
            ),

            Rule(Rule::Chain::N('W'), GRB_ERROR_SERIES + 5, 4,
                Rule::Chain(1, Rule::Chain::T('i')), // Аргумент-переменная
                Rule::Chain(1, Rule::Chain::T('l')), // Аргумент-литерал
                Rule::Chain(3, Rule::Chain::T('i'), Rule::Chain::T(','), Rule::Chain::N('W')), // Переменная и следующие аргументы
                Rule::Chain(3, Rule::Chain::T('l'), Rule::Chain::T(','), Rule::Chain::N('W'))  // Литерал и следующие аргументы
            )
        );
    }
}