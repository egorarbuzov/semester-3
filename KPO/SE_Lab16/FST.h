#pragma once

namespace FST
{
    struct RELATION      // ребро: символ -> вершина графа переходов КА
    {
        char  symbol;    // символ перехода
        short nnode;     // номер смежной вершины
        RELATION(char c = 0x00, short ns = -1);
    };

    struct NODE          // вершина графа переходов
    {
        short n_relation;        // количество инцидентных ребер
        RELATION* relations;     // инцидентные ребра
        NODE();
        NODE(short n, RELATION rel, ...);
    };

    struct FST           // недетерминированный конечный автомат
    {
        char* string;    // цепочка (строка, завершается 0x00)
        short  position;  // текущая позиция в цепочке
        short  nstates;   // количество состояний автомата
        NODE* nodes;     // граф переходов: [0] - начальное, [nstates-1] - конечное
        short* rstates;   // возможные состояния автомата на данной позиции
        FST(char* s, short ns, NODE n, ...);
    };

    bool execute(FST& fst);  // выполнить распознавание цепочки
};