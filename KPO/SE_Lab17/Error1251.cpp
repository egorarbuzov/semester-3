#include "stdafx.h"
#include "Error.h"
#include <cstring>

namespace Error
{
    Error errors[ERROR_MAX_ENTRY] = {
        ERROR_ENTRY(0, "Недопустимый код ошибки"),
        ERROR_ENTRY(1, "Системный сбой"),
        ERROR_ENTRY(2, "Ошибка выделения памяти"),
        ERROR_ENTRY(3, "Ошибка открытия файла"),
        ERROR_ENTRY(4, "Ошибка чтения файла"),
        ERROR_ENTRY(5, "Ошибка записи в файл"),
        ERROR_ENTRY(6, "Неверный формат данных"),
        ERROR_ENTRY(7, "Превышен максимальный размер"),
        ERROR_ENTRY(8, "Недостаточно аргументов командной строки"),
        ERROR_ENTRY(9, "Неверный параметр командной строки"),
        ERROR_ENTRY_NODEF10(10),
        ERROR_ENTRY_NODEF10(20),
        ERROR_ENTRY_NODEF10(30),
        ERROR_ENTRY_NODEF10(40),
        ERROR_ENTRY_NODEF10(50),
        ERROR_ENTRY_NODEF10(60),
        ERROR_ENTRY_NODEF10(70),
        ERROR_ENTRY_NODEF10(80),
        ERROR_ENTRY_NODEF10(90),
        ERROR_ENTRY(100, "Параметр -in должен быть задан"),
        ERROR_ENTRY_NODEF(101),
        ERROR_ENTRY_NODEF(102),
        ERROR_ENTRY_NODEF(103),
        ERROR_ENTRY(104, "Превышена длина входного параметра"),
        ERROR_ENTRY_NODEF(105),
        ERROR_ENTRY_NODEF(106),
        ERROR_ENTRY_NODEF(107),
        ERROR_ENTRY_NODEF(108),
        ERROR_ENTRY_NODEF(109),
        ERROR_ENTRY(110, "Ошибка при открытии файла с исходным кодом (-in)"),
        ERROR_ENTRY(111, "Недопустимый символ в исходном файле (-in)"),
        ERROR_ENTRY(112, "Ошибка при создании файла протокола (-log)"),
        ERROR_ENTRY_NODEF(113),
        ERROR_ENTRY_NODEF(114),
        ERROR_ENTRY_NODEF(115),
        ERROR_ENTRY_NODEF(116),
        ERROR_ENTRY_NODEF(117),
        ERROR_ENTRY_NODEF(118),
        ERROR_ENTRY_NODEF(119),
        ERROR_ENTRY_NODEF(120),
        ERROR_ENTRY_NODEF10(130),
        ERROR_ENTRY_NODEF10(140),
        ERROR_ENTRY_NODEF10(150),
        ERROR_ENTRY_NODEF10(160),
        ERROR_ENTRY_NODEF10(170),
        ERROR_ENTRY_NODEF10(180),
        ERROR_ENTRY_NODEF10(190),
        ERROR_ENTRY_NODEF100(300),
        ERROR_ENTRY_NODEF100(400),
        ERROR_ENTRY_NODEF100(500),
        ERROR_ENTRY_NODEF100(600),
        ERROR_ENTRY_NODEF100(700),
        ERROR_ENTRY_NODEF100(800),
        ERROR_ENTRY_NODEF100(900),
        ERROR_ENTRY(200, "Идентификатор длиннее 5 символов"),
        ERROR_ENTRY(201, "Идентификатор совпадает с ключевым словом"),
        ERROR_ENTRY(202, "Незакрытая строка (нет закрывающей ')"),
        ERROR_ENTRY(203, "Недопустимый символ в идентификаторе"),
        ERROR_ENTRY(204, "Литерал вне диапазона"),
        ERROR_ENTRY(205, "Пустой литерал"),
        ERROR_ENTRY(206, "Превышено максимальное количество идентификаторов"),
        ERROR_ENTRY(207, "Превышено максимальное количество лексем"),
        ERROR_ENTRY(208, "Неизвестная лексема"),
        ERROR_ENTRY(209, "Ошибка лексического анализа"),
    };

    Error geterror(int id) {
        Error err;

        if (id < 0 || id >= ERROR_MAX_ENTRY) {
            err.id = 0;
            strcpy_s(err.message, ERROR_MAXSIZE_MESSAGE, errors[0].message);
            err.inext.line = -1;
            err.inext.col = -1;
            return err;
        }

        err.id = errors[id].id;
        strcpy_s(err.message, ERROR_MAXSIZE_MESSAGE, errors[id].message);
        err.inext.line = -1;
        err.inext.col = -1;

        return err;
    }

    Error geterrorin(int id, int line, int col) {
        Error err = geterror(id);
        err.inext.line = line;
        err.inext.col = col;
        return err;
    }
};