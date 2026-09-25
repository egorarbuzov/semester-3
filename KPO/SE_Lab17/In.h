#pragma once

#define IN_MAX_LEN_TEXT   (1024*1024)
#define IN_CODE_ENDL      '\n'
#define IN_CODE_SEPARATOR   '|'

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
/* 0-15   */ In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::T, In::T, In::F, In::F, In::T, In::F, In::F, /* 9=\t, 10=\n, 13=\r */\
/* 16-31  */ In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F,\
/* 32-47  */ In::T, In::F, In::F, In::F, In::F, In::F, In::F, 39,    In::T, In::T, In::T, In::T, In::T, In::T, In::F, In::T, /* ' ( ) * + , - / */\
/* 48-63  */ In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::F, In::T, In::F, In::T, In::F, In::F, /* 0-9 ; = */\
/* 64-79  */ In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F,\
/* 80-95  */ In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F,\
/* 96-111 */ In::F, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, /* a-o */\
/* 112-127*/ In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::F, In::T, In::F, In::F, /* p-z { } */\
/* 128-143*/ In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F,\
/* 144-159*/ In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F,\
/* 160-175*/ In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F,\
/* 176-191*/ In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F, In::F,\
/* 192-207*/ In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, /* À-Ï */\
/* 208-223*/ In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, /* Ð-ß */\
/* 224-239*/ In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, /* à-ï */\
/* 240-255*/ In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T  /* ð-ÿ */\
}