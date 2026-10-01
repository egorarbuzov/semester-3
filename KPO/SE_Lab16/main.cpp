#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <clocale>
#include <cstring>
#include "FST.h"

FST::FST makeAutomat_lab13(char* s)
{
        return FST::FST(
            s,
            23,

            FST::NODE(1, FST::RELATION('s', 1)),

            FST::NODE(1, FST::RELATION('t', 2)),

            FST::NODE(1, FST::RELATION('a', 3)),

            FST::NODE(1, FST::RELATION('r', 4)),

            FST::NODE(1, FST::RELATION('t', 5)),

            FST::NODE(2, FST::RELATION(' ', 5), FST::RELATION(' ', 6)),

            FST::NODE(5,
                FST::RELATION(' ', 6),
                FST::RELATION('s', 7),
                FST::RELATION('w', 10),
                FST::RELATION('s', 17),
                FST::RELATION('k', 21)  
            ),

            FST::NODE(2, FST::RELATION('e', 8), FST::RELATION('h', 14)),

            FST::NODE(1, FST::RELATION('n', 9)),

            FST::NODE(1, FST::RELATION('d', 11)),

            FST::NODE(1, FST::RELATION('a', 12)),

            FST::NODE(2, FST::RELATION(' ', 11), FST::RELATION(' ', 6)),

            FST::NODE(1, FST::RELATION('i', 13)),

            FST::NODE(1, FST::RELATION('t', 15)),

            FST::NODE(1, FST::RELATION('o', 16)),

            FST::NODE(2, FST::RELATION(' ', 15), FST::RELATION(' ', 6)),

            FST::NODE(1, FST::RELATION('w', 19)),

            FST::NODE(1, FST::RELATION('t', 18)),

            FST::NODE(1, FST::RELATION('o', 20)),

            FST::NODE(2, FST::RELATION(' ', 19), FST::RELATION(' ', 6)),

            FST::NODE(1, FST::RELATION('p', 22)),

            FST::NODE(2, FST::RELATION(' ', 21), FST::RELATION(' ', 6)),

            FST::NODE()
        );

}

int main()
{
    setlocale(LC_ALL, "rus");

    const char* tests7[] = {
        "start  stop",                        // 1
        "start   stop",                       // 2
        "start send stop",                    // 3
        "start wait stop",                    // 4
        "start show stop",                    // 5
        "start send wait show stop",          // 6
        "start  send   wait  show   stop",   // 7
        "start send k k k stop"
    };

    for (int i = 0; i < 8; i++)
    {
        char buf[128];
        strcpy(buf, tests7[i]);
        FST::FST fst = makeAutomat_lab13(buf);

        if (FST::execute(fst))
            std::cout << "Цепочка \"" << fst.string << "\" распознана\n";
        else
            std::cout << "Цепочка \"" << fst.string << "\" не распознана\n";
    }

    std::cout << "\n2 цепочки, не распознаваемые автоматом:\n";

    {
        char s[] = "start send";
        FST::FST fst = makeAutomat_lab13(s);
        std::cout << "Цепочка \"" << fst.string << "\" "
            << (FST::execute(fst) ? "распознана" : "не распознана") << "\n";
    }
    {
        char s[] = "send  stop";
        FST::FST fst = makeAutomat_lab13(s);
        std::cout << "Цепочка \"" << fst.string << "\" "
            << (FST::execute(fst) ? "распознана" : "не распознана") << "\n";
    }

    std::cout << "\nВсе символы пройдены, но не распознана\n";
    {
        char s[] = "start  sto";   // дошли до конца, но 'p' нет
        FST::FST fst = makeAutomat_lab13(s);
        bool res = FST::execute(fst);
        std::cout << "Цепочка \"" << fst.string << "\" пройдена до конца (position="
            << fst.position << "), результат: "
            << (res ? "распознана" : "не распознана") << "\n";
    }


    std::cout << "\nРазбор прерван, не все символы перебраны\n";
    {
        char s[] = "start  recv  stop"; // на 'r' нет перехода
        FST::FST fst = makeAutomat_lab13(s);
        bool res = FST::execute(fst);
        std::cout << "Цепочка \"" << fst.string << "\" остановлена на позиции "
            << fst.position << ", результат: "
            << (res ? "распознана" : "не распознана") << "\n";
    }

    system("pause");
    return 0;
}