#include "FST.h"
#include <cstdarg>
#include <cstring>

namespace FST
{
    // ---------- RELATION ----------
    RELATION::RELATION(char c, short ns)
        : symbol(c), nnode(ns)
    {
    }

    // ---------- NODE ----------
    // Конструктор по умолчанию — вершина без исходящих ребер
    // (используется для конечного/финального состояния автомата)
    NODE::NODE()
        : n_relation(0), relations(nullptr)
    {
    }

    // Вершина с n исходящими ребрами, заданными списком переменной длины
    NODE::NODE(short n, RELATION rel, ...)
        : n_relation(n), relations(nullptr)
    {
        if (n <= 0) return;

        relations = new RELATION[n];
        relations[0] = rel;                 // первое ребро пришло именованным параметром

        va_list args;
        va_start(args, rel);
        for (short i = 1; i < n; i++)
            relations[i] = va_arg(args, RELATION);
        va_end(args);
    }

    // ---------- FST ----------
    // ns состояний, первое из которых передано именованным параметром n,
    // остальные ns-1 — через список переменной длины
    FST::FST(char* s, short ns, NODE n, ...)
        : string(s), position(0), nstates(ns), nodes(nullptr), rstates(nullptr)
    {
        nodes = new NODE[ns];
        if (ns > 0) nodes[0] = n;

        va_list args;
        va_start(args, n);
        for (short i = 1; i < ns; i++)
            nodes[i] = va_arg(args, NODE);
        va_end(args);

        rstates = new short[ns];
        for (short i = 0; i < ns; i++)
            rstates[i] = -1;                // -1 -> состояние ещё не достигнуто
    }

    // ---------- execute ----------
    // Алгоритм разбора цепочки, основанный на двух массивах (лекция 10).
    //
    // Идея: вместо привычного булева "активно / не активно" в массиве
    // хранится НОМЕР ПОЗИЦИИ (число уже разобранных символов), на которой
    // состояние стало достижимым. Значение -1 означает, что состояние
    // недостижимо.
    //
    //   cur[s]  == p   -> состояние s достигнуто после разбора p символов
    //   cur[s]  == -1  -> состояние s недостижимо
    //
    // На каждом такте (символ string[position]) по массиву cur строится
    // новый массив next: если состояние s было активно РОВНО на текущей
    // позиции (cur[s] == position, т.е. "дошли досюда, дальше не разбирали"),
    // то для каждого исходящего ребра (s -> t) по символу string[position]
    // помечаем next[t] = position + 1.
    //
    // После обработки всей цепочки автомат "разобрал" цепочку, если
    // финальное состояние (последний элемент массива) равно длине строки —
    // то есть путь в него был построен, использовав ВСЕ значащие символы
    // входной цепочки.
    bool execute(FST& fst)
    {
        short n = fst.nstates;
        if (n <= 0) return false;

        short* cur  = new short[n];
        short* next = new short[n];

        for (short i = 0; i < n; i++)
            cur[i] = -1;
        cur[0] = 0;                 // стартовое состояние достижимо после 0 символов

        fst.position = 0;

        while (fst.string[fst.position] != 0x00)
        {
            char symbol = fst.string[fst.position];

            for (short i = 0; i < n; i++)
                next[i] = -1;

            bool anyActive = false;

            for (short s = 0; s < n; s++)
            {
                // состояние s активно ИМЕННО на текущей позиции разбора
                if (cur[s] == fst.position)
                {
                    NODE& node = fst.nodes[s];
                    for (short r = 0; r < node.n_relation; r++)
                    {
                        if (node.relations[r].symbol == symbol)
                        {
                            short target = node.relations[r].nnode;
                            next[target] = fst.position + 1;
                            anyActive = true;
                        }
                    }
                }
            }

            for (short i = 0; i < n; i++)
                cur[i] = next[i];

            fst.position++;

            // ни одно состояние не достижимо дальше -> разбор бессмысленно продолжать
            // (демонстрирует случай "разбор завершается, не перебрав все символы")
            if (!anyActive)
                break;
        }

        // сохраняем итоговый массив состояний в структуре FST — как и
        // предусмотрено полем rstates, для последующей демонстрации/анализа
        for (short i = 0; i < n; i++)
            fst.rstates[i] = cur[i];

        delete[] next;

        bool accepted = (cur[n - 1] == static_cast<short>(strlen(fst.string)));

        delete[] cur;
        return accepted;
    }
}
