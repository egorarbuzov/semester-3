#pragma once

#define ID_MAXSIZE  5              // максимальная длина идентификатора
#define TI_MAXSIZE  4096           // максимальное количество строк
#define TI_NULLIDX  0xffffffff     // "не найдено"

namespace IT
{
    // Строка таблицы идентификаторов
    struct Entry
    {
        char id[ID_MAXSIZE];  // идентификатор (без нуль-терминатора)
        int  sn;              // номер строки первого вхождения
    };

    // Экземпляр таблицы идентификаторов
    struct IdTable
    {
        int    maxsize;   // ёмкость
        int    size;      // текущее количество строк
        Entry* table;     // массив строк
    };

    // Функции
    IdTable Create(int size);                             // создать таблицу
    void    Add(IdTable& idtable, Entry entry);           // добавить идентификатор
    Entry   GetEntry(IdTable& idtable, int n);            // получить по номеру
    int     IsId(IdTable& idtable, char id[ID_MAXSIZE]);  // поиск: вернуть номер или TI_NULLIDX
    void    Delete(IdTable& idtable);                     // удалить
}