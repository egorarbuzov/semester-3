#include "stdafx.h"
#include "IT.h"
#include "Error.h"
#include <cstring>

namespace IT
{
    IdTable Create(int size)
    {
        if (size > TI_MAXSIZE) {
            ERROR_THROW(206);
        }

        IdTable idtable;
        idtable.maxsize = size;
        idtable.size = 0;
        idtable.table = new Entry[size];

        if (idtable.table == nullptr) {
            ERROR_THROW(2);
        }

        return idtable;
    }

    void Add(IdTable& idtable, Entry entry)
    {
        if (idtable.size >= idtable.maxsize) {
            ERROR_THROW(206);
        }

        idtable.table[idtable.size] = entry;
        idtable.size++;
    }

    Entry GetEntry(IdTable& idtable, int n)
    {
        if (n < 0 || n >= idtable.size) {
            ERROR_THROW(0);
        }

        return idtable.table[n];
    }

    int IsId(IdTable& idtable, char id[ID_MAXSIZE])
    {
        for (int i = 0; i < idtable.size; i++) {
            if (strncmp(idtable.table[i].id, id, ID_MAXSIZE - 1) == 0)
                return i;
        }
        return TI_NULLIDX;
    }

    void Delete(IdTable& idtable)
    {
        if (idtable.table != nullptr) {
            delete[] idtable.table;
            idtable.table = nullptr;
        }
        idtable.size = 0;
        idtable.maxsize = 0;
    }
}