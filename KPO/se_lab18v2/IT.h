#pragma once

#define ID_MAXSIZE    16
#define TI_MAXSIZE    4096
#define TI_NULLIDX    0xffffffff
#define VALUE_MAXSIZE 256

namespace IT
{
    enum IDDATATYPE { INT = 1, STR = 2 };
    enum IDTYPE { V = 1, F = 2, P = 3, L = 4 };

    struct Entry
    {
        char        id[ID_MAXSIZE];
        int         sn;
        int         col;
        IDDATATYPE  iddatatype;
        IDTYPE      idtype;
        char        value[VALUE_MAXSIZE];
        int         idxFirstLE;
    };

    struct IdTable
    {
        int    maxsize;
        int    size;
        Entry* table;
    };

    IdTable Create(int size);
    void    Add(IdTable& idtable, Entry entry);
    Entry   GetEntry(IdTable& idtable, int n);
    int     IsId(IdTable& idtable, char id[ID_MAXSIZE]);
    void    Delete(IdTable& idtable);
}
