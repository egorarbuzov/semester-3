#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#include "Lex.h"
#include "Error.h"
#include "FST.h"
#include <cstring>

namespace Lex
{
    static FST::FST makeAutomat_integer(char* s)
    {
        return FST::FST(s, 8,
            FST::NODE(1, FST::RELATION('i', 1)),
            FST::NODE(1, FST::RELATION('n', 2)),
            FST::NODE(1, FST::RELATION('t', 3)),
            FST::NODE(1, FST::RELATION('e', 4)),
            FST::NODE(1, FST::RELATION('g', 5)),
            FST::NODE(1, FST::RELATION('e', 6)),
            FST::NODE(1, FST::RELATION('r', 7)),
            FST::NODE());
    }

    static FST::FST makeAutomat_string(char* s)
    {
        return FST::FST(s, 7,
            FST::NODE(1, FST::RELATION('s', 1)),
            FST::NODE(1, FST::RELATION('t', 2)),
            FST::NODE(1, FST::RELATION('r', 3)),
            FST::NODE(1, FST::RELATION('i', 4)),
            FST::NODE(1, FST::RELATION('n', 5)),
            FST::NODE(1, FST::RELATION('g', 6)),
            FST::NODE());
    }

    static FST::FST makeAutomat_function(char* s)
    {
        return FST::FST(s, 9,
            FST::NODE(1, FST::RELATION('f', 1)),
            FST::NODE(1, FST::RELATION('u', 2)),
            FST::NODE(1, FST::RELATION('n', 3)),
            FST::NODE(1, FST::RELATION('c', 4)),
            FST::NODE(1, FST::RELATION('t', 5)),
            FST::NODE(1, FST::RELATION('i', 6)),
            FST::NODE(1, FST::RELATION('o', 7)),
            FST::NODE(1, FST::RELATION('n', 8)),
            FST::NODE());
    }

    static FST::FST makeAutomat_declare(char* s)
    {
        return FST::FST(s, 8,
            FST::NODE(1, FST::RELATION('d', 1)),
            FST::NODE(1, FST::RELATION('e', 2)),
            FST::NODE(1, FST::RELATION('c', 3)),
            FST::NODE(1, FST::RELATION('l', 4)),
            FST::NODE(1, FST::RELATION('a', 5)),
            FST::NODE(1, FST::RELATION('r', 6)),
            FST::NODE(1, FST::RELATION('e', 7)),
            FST::NODE());
    }

    static FST::FST makeAutomat_return(char* s)
    {
        return FST::FST(s, 7,
            FST::NODE(1, FST::RELATION('r', 1)),
            FST::NODE(1, FST::RELATION('e', 2)),
            FST::NODE(1, FST::RELATION('t', 3)),
            FST::NODE(1, FST::RELATION('u', 4)),
            FST::NODE(1, FST::RELATION('r', 5)),
            FST::NODE(1, FST::RELATION('n', 6)),
            FST::NODE());
    }

    static FST::FST makeAutomat_print(char* s)
    {
        return FST::FST(s, 6,
            FST::NODE(1, FST::RELATION('p', 1)),
            FST::NODE(1, FST::RELATION('r', 2)),
            FST::NODE(1, FST::RELATION('i', 3)),
            FST::NODE(1, FST::RELATION('n', 4)),
            FST::NODE(1, FST::RELATION('t', 5)),
            FST::NODE());
    }

    static FST::FST makeAutomat_main(char* s)
    {
        return FST::FST(s, 5,
            FST::NODE(1, FST::RELATION('m', 1)),
            FST::NODE(1, FST::RELATION('a', 2)),
            FST::NODE(1, FST::RELATION('i', 3)),
            FST::NODE(1, FST::RELATION('n', 4)),
            FST::NODE());
    }

    static FST::FST makeAutomat_id5(char* s)
    {
        return FST::FST(s, 6,
            FST::NODE(26,
                FST::RELATION('a', 1), FST::RELATION('b', 1), FST::RELATION('c', 1),
                FST::RELATION('d', 1), FST::RELATION('e', 1), FST::RELATION('f', 1),
                FST::RELATION('g', 1), FST::RELATION('h', 1), FST::RELATION('i', 1),
                FST::RELATION('j', 1), FST::RELATION('k', 1), FST::RELATION('l', 1),
                FST::RELATION('m', 1), FST::RELATION('n', 1), FST::RELATION('o', 1),
                FST::RELATION('p', 1), FST::RELATION('q', 1), FST::RELATION('r', 1),
                FST::RELATION('s', 1), FST::RELATION('t', 1), FST::RELATION('u', 1),
                FST::RELATION('v', 1), FST::RELATION('w', 1), FST::RELATION('x', 1),
                FST::RELATION('y', 1), FST::RELATION('z', 1)),
            FST::NODE(26,
                FST::RELATION('a', 2), FST::RELATION('b', 2), FST::RELATION('c', 2),
                FST::RELATION('d', 2), FST::RELATION('e', 2), FST::RELATION('f', 2),
                FST::RELATION('g', 2), FST::RELATION('h', 2), FST::RELATION('i', 2),
                FST::RELATION('j', 2), FST::RELATION('k', 2), FST::RELATION('l', 2),
                FST::RELATION('m', 2), FST::RELATION('n', 2), FST::RELATION('o', 2),
                FST::RELATION('p', 2), FST::RELATION('q', 2), FST::RELATION('r', 2),
                FST::RELATION('s', 2), FST::RELATION('t', 2), FST::RELATION('u', 2),
                FST::RELATION('v', 2), FST::RELATION('w', 2), FST::RELATION('x', 2),
                FST::RELATION('y', 2), FST::RELATION('z', 2)),
            FST::NODE(26,
                FST::RELATION('a', 3), FST::RELATION('b', 3), FST::RELATION('c', 3),
                FST::RELATION('d', 3), FST::RELATION('e', 3), FST::RELATION('f', 3),
                FST::RELATION('g', 3), FST::RELATION('h', 3), FST::RELATION('i', 3),
                FST::RELATION('j', 3), FST::RELATION('k', 3), FST::RELATION('l', 3),
                FST::RELATION('m', 3), FST::RELATION('n', 3), FST::RELATION('o', 3),
                FST::RELATION('p', 3), FST::RELATION('q', 3), FST::RELATION('r', 3),
                FST::RELATION('s', 3), FST::RELATION('t', 3), FST::RELATION('u', 3),
                FST::RELATION('v', 3), FST::RELATION('w', 3), FST::RELATION('x', 3),
                FST::RELATION('y', 3), FST::RELATION('z', 3)),
            FST::NODE(26,
                FST::RELATION('a', 4), FST::RELATION('b', 4), FST::RELATION('c', 4),
                FST::RELATION('d', 4), FST::RELATION('e', 4), FST::RELATION('f', 4),
                FST::RELATION('g', 4), FST::RELATION('h', 4), FST::RELATION('i', 4),
                FST::RELATION('j', 4), FST::RELATION('k', 4), FST::RELATION('l', 4),
                FST::RELATION('m', 4), FST::RELATION('n', 4), FST::RELATION('o', 4),
                FST::RELATION('p', 4), FST::RELATION('q', 4), FST::RELATION('r', 4),
                FST::RELATION('s', 4), FST::RELATION('t', 4), FST::RELATION('u', 4),
                FST::RELATION('v', 4), FST::RELATION('w', 4), FST::RELATION('x', 4),
                FST::RELATION('y', 4), FST::RELATION('z', 4)),
            FST::NODE(26,
                FST::RELATION('a', 5), FST::RELATION('b', 5), FST::RELATION('c', 5),
                FST::RELATION('d', 5), FST::RELATION('e', 5), FST::RELATION('f', 5),
                FST::RELATION('g', 5), FST::RELATION('h', 5), FST::RELATION('i', 5),
                FST::RELATION('j', 5), FST::RELATION('k', 5), FST::RELATION('l', 5),
                FST::RELATION('m', 5), FST::RELATION('n', 5), FST::RELATION('o', 5),
                FST::RELATION('p', 5), FST::RELATION('q', 5), FST::RELATION('r', 5),
                FST::RELATION('s', 5), FST::RELATION('t', 5), FST::RELATION('u', 5),
                FST::RELATION('v', 5), FST::RELATION('w', 5), FST::RELATION('x', 5),
                FST::RELATION('y', 5), FST::RELATION('z', 5)),
            FST::NODE());
    }

    static FST::FST makeAutomat_number(char* s)
    {
        return FST::FST(s, 2,
            FST::NODE(10,
                FST::RELATION('0', 1), FST::RELATION('1', 1), FST::RELATION('2', 1),
                FST::RELATION('3', 1), FST::RELATION('4', 1), FST::RELATION('5', 1),
                FST::RELATION('6', 1), FST::RELATION('7', 1), FST::RELATION('8', 1),
                FST::RELATION('9', 1)),
            FST::NODE(10,
                FST::RELATION('0', 1), FST::RELATION('1', 1), FST::RELATION('2', 1),
                FST::RELATION('3', 1), FST::RELATION('4', 1), FST::RELATION('5', 1),
                FST::RELATION('6', 1), FST::RELATION('7', 1), FST::RELATION('8', 1),
                FST::RELATION('9', 1)));
    }

    static FST::FST makeAutomat_semicolon(char* s)
    {
        return FST::FST(s, 2, FST::NODE(1, FST::RELATION(';', 1)), FST::NODE());
    }
    static FST::FST makeAutomat_comma(char* s)
    {
        return FST::FST(s, 2, FST::NODE(1, FST::RELATION(',', 1)), FST::NODE());
    }
    static FST::FST makeAutomat_lbrace(char* s)
    {
        return FST::FST(s, 2, FST::NODE(1, FST::RELATION('{', 1)), FST::NODE());
    }
    static FST::FST makeAutomat_rbrace(char* s)
    {
        return FST::FST(s, 2, FST::NODE(1, FST::RELATION('}', 1)), FST::NODE());
    }
    static FST::FST makeAutomat_lthesis(char* s)
    {
        return FST::FST(s, 2, FST::NODE(1, FST::RELATION('(', 1)), FST::NODE());
    }
    static FST::FST makeAutomat_rthesis(char* s)
    {
        return FST::FST(s, 2, FST::NODE(1, FST::RELATION(')', 1)), FST::NODE());
    }
    static FST::FST makeAutomat_plus(char* s)
    {
        return FST::FST(s, 2, FST::NODE(1, FST::RELATION('+', 1)), FST::NODE());
    }
    static FST::FST makeAutomat_minus(char* s)
    {
        return FST::FST(s, 2, FST::NODE(1, FST::RELATION('-', 1)), FST::NODE());
    }
    static FST::FST makeAutomat_star(char* s)
    {
        return FST::FST(s, 2, FST::NODE(1, FST::RELATION('*', 1)), FST::NODE());
    }
    static FST::FST makeAutomat_slash(char* s)
    {
        return FST::FST(s, 2, FST::NODE(1, FST::RELATION('/', 1)), FST::NODE());
    }
    static FST::FST makeAutomat_assign(char* s)
    {
        return FST::FST(s, 2, FST::NODE(1, FST::RELATION('=', 1)), FST::NODE());
    }

    static bool isLetter(char c)
    {
        unsigned char u = static_cast<unsigned char>(c);
        return (c >= 'a' && c <= 'z') || (u == 0xB8) || (u >= 0xE0);
    }

    static bool isDigit(char c)
    {
        return c >= '0' && c <= '9';
    }

    static int readAllLetters(char* p)
    {
        int len = 0;
        while (p[len] != 0 && isLetter(p[len]))
            ++len;
        return len;
    }

    static bool isStdNameAt(const char* p, int len)
    {
        return len == 6 &&
               (strncmp(p, "strlen", 6) == 0 || strncmp(p, "substr", 6) == 0);
    }

    static bool checkIntRange(const char* p, int len)
    {
        if (len > 10) return false;

        char buf[32] = { 0 };
        strncpy(buf, p, len);

        if (len < 10) return true;
        return strcmp(buf, "2147483647") <= 0;
    }

    void LexicalAnalysis(In::IN& in, LT::LexTable& lextable, IT::IdTable& idtable)
    {
        int line = 1;
        int i = 0;

        IT::IDDATATYPE curDatatype = IT::INT;
        bool afterFunction = false;
        bool afterDeclare = false;
        int  parenDepth = 0;
        bool inParams = false;

        while (i < in.size)
        {
            char ch = (char)in.text[i];

            if (ch == '|')
            {
                ++line;
                ++i;
                continue;
            }
            if (ch == ' ' || ch == '\t')
            {
                ++i;
                continue;
            }

            // String literal.
            if (ch == '\'')
            {
                int start = i;
                ++i;
                int strLen = 0;
                while (i < in.size && in.text[i] != '\'')
                {
                    ++strLen;
                    ++i;
                }

                if (i >= in.size)
                    ERROR_THROW_IN(202, line, in.origCol[start]);
                if (strLen > 255)
                    ERROR_THROW_IN(203, line, in.origCol[start]);

                ++i;

                IT::Entry ie;
                memset(ie.id, 0, ID_MAXSIZE);
                strncpy(ie.id, "_lit", ID_MAXSIZE - 1);
                ie.sn = line;
                ie.col = in.origCol[start];
                ie.iddatatype = IT::STR;
                ie.idtype = IT::L;
                memset(ie.value, 0, VALUE_MAXSIZE);
                int copyLen = (strLen < VALUE_MAXSIZE - 1) ? strLen : VALUE_MAXSIZE - 1;
                strncpy(ie.value, (char*)in.text + start + 1, copyLen);
                ie.idxFirstLE = lextable.size;
                IT::Add(idtable, ie);
                int litIdx = idtable.size - 1;

                LT::Entry e;
                e.lexema[0] = LEX_LITERAL;
                e.sn = line;
                e.idxTI = litIdx;
                LT::Add(lextable, e);
                continue;
            }

            char buf[256];
            strncpy(buf, (char*)in.text + i, 255);
            buf[255] = 0;

            struct Rule
            {
                char type;
                FST::FST(*make)(char*);
            };

            static Rule rules[] = {
                { LEX_INTEGER,   makeAutomat_integer   },
                { LEX_STRING,    makeAutomat_string    },
                { LEX_FUNCTION,  makeAutomat_function  },
                { LEX_DECLARE,   makeAutomat_declare   },
                { LEX_RETURN,    makeAutomat_return    },
                { LEX_PRINT,     makeAutomat_print     },
                { LEX_MAIN,      makeAutomat_main      },
                { LEX_LITERAL,   makeAutomat_number    },
                { LEX_SEMICOLON, makeAutomat_semicolon },
                { LEX_COMMA,     makeAutomat_comma     },
                { LEX_LEFTBRACE, makeAutomat_lbrace    },
                { LEX_BRACELET,  makeAutomat_rbrace    },
                { LEX_LEFTHESIS, makeAutomat_lthesis   },
                { LEX_RIGHTHESIS,makeAutomat_rthesis   },
                { LEX_PLUS,      makeAutomat_plus      },
                { LEX_MINUS,     makeAutomat_minus     },
                { LEX_STAR,      makeAutomat_star      },
                { LEX_DIRSLASH,  makeAutomat_slash     },
                { LEX_ASSIGN,    makeAutomat_assign    },
                { LEX_ID,        makeAutomat_id5       },
            };

            char bestType = 0;
            int  bestLen = 0;

            for (int r = 0; r < (int)(sizeof(rules) / sizeof(rules[0])); ++r)
            {
                FST::FST fst = rules[r].make(buf);
                short len = FST::matchPrefix(fst);
                if (len > bestLen)
                {
                    bestLen = len;
                    bestType = rules[r].type;
                }
            }

            if (bestType != LEX_ID)
            {
                int idLen = readAllLetters(buf);
                if (idLen > 0)
                {
                    int capped = (idLen > ID_MAXSIZE - 1) ? ID_MAXSIZE - 1 : idLen;
                    if (capped > bestLen)
                    {
                        bestLen = capped;
                        bestType = LEX_ID;
                    }
                }
            }

            if (bestLen == 0)
                ERROR_THROW_IN(208, line, in.origCol[i]);

            // Numeric literal.
            if (bestType == LEX_LITERAL && isDigit((char)in.text[i]))
            {
                int numLen = 0;
                while (i + numLen < in.size && isDigit((char)in.text[i + numLen]))
                    ++numLen;

                if (!checkIntRange((char*)in.text + i, numLen))
                    ERROR_THROW_IN(204, line, in.origCol[i]);

                IT::Entry ie;
                memset(ie.id, 0, ID_MAXSIZE);
                strncpy(ie.id, "_lit", ID_MAXSIZE - 1);
                ie.sn = line;
                ie.col = in.origCol[i];
                ie.iddatatype = IT::INT;
                ie.idtype = IT::L;
                memset(ie.value, 0, VALUE_MAXSIZE);
                int copyLen = (numLen < VALUE_MAXSIZE - 1) ? numLen : VALUE_MAXSIZE - 1;
                strncpy(ie.value, (char*)in.text + i, copyLen);
                ie.idxFirstLE = lextable.size;
                IT::Add(idtable, ie);
                int litIdx = idtable.size - 1;

                LT::Entry e;
                e.lexema[0] = LEX_LITERAL;
                e.sn = line;
                e.idxTI = litIdx;
                LT::Add(lextable, e);

                i += numLen;
                continue;
            }

            LT::Entry e;
            e.lexema[0] = bestType;
            e.sn = line;
            e.idxTI = LT_TI_NULLIDX;

            if (bestType == LEX_INTEGER || bestType == LEX_STRING)
            {
                if (strncmp((char*)in.text + i, "string", 6) == 0)
                    curDatatype = IT::STR;
                else
                    curDatatype = IT::INT;
            }
            else if (bestType == LEX_FUNCTION)
            {
                afterFunction = true;
                afterDeclare = false;
                inParams = false;
            }
            else if (bestType == LEX_DECLARE)
            {
                afterDeclare = true;
            }
            else if (bestType == LEX_LEFTHESIS)
            {
                ++parenDepth;
            }
            else if (bestType == LEX_RIGHTHESIS)
            {
                if (parenDepth > 0) --parenDepth;
                if (parenDepth == 0) inParams = false;
            }
            else if (bestType == LEX_LEFTBRACE)
            {
                inParams = false;
                afterFunction = false;
                afterDeclare = false;
                parenDepth = 0;
            }

            if (bestType == LEX_ID)
            {
                int fullLen = readAllLetters((char*)in.text + i);

                if (fullLen > 5 && !isStdNameAt((char*)in.text + i, fullLen))
                    ERROR_THROW_IN(209, line, in.origCol[i]);

                int idLen = (fullLen > ID_MAXSIZE - 1) ? ID_MAXSIZE - 1 : fullLen;
                char id[ID_MAXSIZE] = { 0 };
                strncpy(id, (char*)in.text + i, idLen);

                IT::IDTYPE idType = IT::V;
                IT::IDDATATYPE idDt = curDatatype;
                bool isParam = false;

                if (afterFunction)
                {
                    idType = IT::F;
                    afterFunction = false;
                    inParams = true;
                }
                else if (inParams && parenDepth > 0)
                {
                    idType = IT::P;
                    isParam = true;
                }
                else if (afterDeclare)
                {
                    idType = IT::V;
                    afterDeclare = false;
                }

                int existing = IT::IsId(idtable, id);

                if (existing != TI_NULLIDX)
                {
                    if (idType == IT::F)
                    {
                        idtable.table[existing].idtype = IT::F;
                        idtable.table[existing].iddatatype = idDt;
                    }
                    else if (!isParam)
                    {
                        idType = idtable.table[existing].idtype;
                        idDt = idtable.table[existing].iddatatype;
                    }
                }
                else
                {
                    if (strcmp(id, "strlen") == 0)
                        idDt = IT::INT;
                    else if (strcmp(id, "substr") == 0)
                        idDt = IT::STR;
                }

                int idx = existing;
                if (idx == TI_NULLIDX)
                {
                    IT::Entry ie;
                    memset(ie.id, 0, ID_MAXSIZE);
                    strncpy(ie.id, id, idLen);
                    ie.sn = line;
                    ie.col = in.origCol[i];
                    ie.iddatatype = idDt;
                    ie.idtype = idType;
                    memset(ie.value, 0, VALUE_MAXSIZE);
                    if (idDt == IT::INT)
                        strcpy_s(ie.value, VALUE_MAXSIZE, "0");
                    else
                        ie.value[0] = 0;
                    ie.idxFirstLE = lextable.size;
                    IT::Add(idtable, ie);
                    idx = idtable.size - 1;
                }

                e.idxTI = idx;
                bestLen = fullLen;
            }

            LT::Add(lextable, e);
            i += bestLen;
        }

        // End sentinel. It is appended exactly once.
        LT::Entry endEntry;
        endEntry.lexema[0] = '$';
        endEntry.sn = line;
        endEntry.idxTI = LT_TI_NULLIDX;
        LT::Add(lextable, endEntry);
    }
}
