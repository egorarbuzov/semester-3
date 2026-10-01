#pragma once

#define IN_MAX_LEN_TEXT   (1024*1024)
#define IN_CODE_ENDL      '\n'

#undef IN 

namespace In
{
    enum CodeType { T = 1024, F = 2048, I = 4096 };

    struct IN
    {
        int size;
        int lines;
        int ignor;
        unsigned char* text;
        int code[256];
    };

    IN getin(wchar_t infile[]);
    void WriteResultToOut(wchar_t outfile[], In::IN in);
}

#define IN_CODE_TABLE {\
/* 0-15 */    In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, '>', In::F, In::F, In::T, In::F, In::F, /* 10 - '\n', 13 - '\r' */\
/* 16-31 */   In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F,\
/* 32-47 */   In::T, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, /* 32 - Пробел */\
/* 48-63 */   In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F,\
/* 64-79 */   In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F,\
/* 80-95 */   In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::I, In::F, In::F, In::F, In::F, In::F, In::F, In::F, /* 88 - Английская 'X' (In::I - Игнорировать) */\
/* 96-111 */  In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F,\
/* 112-127 */ In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F,\
/* 128-143 */ In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F,\
/* 144-159 */ In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F,\
/* 160-175 */ In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F,\
/* 176-191 */ In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F,\
/* 192-207 */  '-',  In::F, In::F, In::F, In::F, In::T, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, '!', In::F, /* 192 - Русская 'А' (Замена на '-'), 197 - 'Е' */\
/* 208-223 */ In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F,\
/* 224-239 */ In::F, In::T, In::T, In::T, In::F, In::F, In::F, In::T, In::F, In::F, In::F, In::F, In::F, In::F, In::T, In::F, /* 225 - 'б', 226 - 'в', 227 - 'г', 231 - 'з', 238 - 'о' */\
/* 240-255 */ In::T, In::F, In::F, In::T, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F  /* 240 - 'р', 243 - 'у' */\
}