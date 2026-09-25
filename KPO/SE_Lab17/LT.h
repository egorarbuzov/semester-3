#pragma once

#define LEXEMA_FIXSIZE 1              // размер лексемы (1 символ)
#define LT_MAXSIZE     4096           // максимальное количество строк
#define LT_TI_NULLIDX  0xffffffff     // "нет ссылки" на таблицу идентификаторов

// Коды лексем
#define LEX_INTEGER    't'            // integer / string
#define LEX_STRING     't'
#define LEX_ID         'i'            // идентификатор
#define LEX_LITERAL    'l'            // литерал
#define LEX_FUNCTION   'f'            // function
#define LEX_DECLARE    'd'            // declare
#define LEX_RETURN     'r'            // return
#define LEX_PRINT      'p'            // print
#define LEX_MAIN       'm'            // main
#define LEX_SEMICOLON  ';'            // ;
#define LEX_COMMA      ','            // ,
#define LEX_LEFTBRACE  '{'            // {
#define LEX_BRACELET   '}'            // }
#define LEX_LEFTHESIS  '('            // (
#define LEX_RIGHTHESIS ')'            // )
#define LEX_PLUS       'v'            // + - * /
#define LEX_MINUS      'v'
#define LEX_STAR       'v'
#define LEX_DIRSLASH   'v'

namespace LT
{
    // Строка таблицы лексем
    struct Entry
    {
        char lexema[LEXEMA_FIXSIZE];  // сама лексема (1 символ)
        int  sn;                       // номер строки в исходном тексте
        int  idxTI;                    // индекс в ТИ или LT_TI_NULLIDX
    };

    // Экземпляр таблицы лексем
    struct LexTable
    {
        int    maxsize;   // ёмкость
        int    size;      // текущее количество строк
        Entry* table;     // массив строк
    };

    // Функции
    LexTable Create(int size);                       // создать таблицу
    void     Add(LexTable& lextable, Entry entry);   // добавить строку
    Entry    GetEntry(LexTable& lextable, int n);    // получить строку по номеру
    void     Delete(LexTable& lextable);             // удалить (освободить память)
}