SE_asm01
MASM, x86. Массив из 10 int32, локальная процедура getmin.

Вывод:
getmin = -25

Отладчик (Debug|Win32): F11 до main, в Memory 1 набрать arr,
F9 на строке invoke getmin, затем F10 по циклу:
EAX - минимум, ECX - счётчик, ESI - указатель на элемент.
