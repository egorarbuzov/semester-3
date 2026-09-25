#include "stdafx.h"
#include "LT.h"
#include "Error.h"

namespace LT
{
    // Создать таблицу лексем
    LexTable Create(int size)
    {
        if (size > LT_MAXSIZE) {
            ERROR_THROW(207);  // превышено максимальное количество лексем
        }

        LexTable lextable;
        lextable.maxsize = size;
        lextable.size = 0;
        lextable.table = new Entry[size];

        if (lextable.table == nullptr) {
            ERROR_THROW(2);  // ошибка выделения памяти
        }

        return lextable;
    }

    // Добавить строку в таблицу
    void Add(LexTable& lextable, Entry entry)
    {
        if (lextable.size >= lextable.maxsize) {
            ERROR_THROW(207);  // переполнение
        }

        lextable.table[lextable.size] = entry;
        lextable.size++;
    }

    // Получить строку по номеру
    Entry GetEntry(LexTable& lextable, int n)
    {
        if (n < 0 || n >= lextable.size) {
            ERROR_THROW(0);  // недопустимый код
        }

        return lextable.table[n];
    }

    // Удалить таблицу
    void Delete(LexTable& lextable)
    {
        if (lextable.table != nullptr) {
            delete[] lextable.table;
            lextable.table = nullptr;
        }
        lextable.size = 0;
        lextable.maxsize = 0;
    }
}