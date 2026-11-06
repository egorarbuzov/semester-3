# План выполнения лабораторной работы 19

## Задание

| Проект | Тип | Язык | Требование |
|--------|-----|------|------------|
| `SE_asm01` | консоль | MASM | массив 10 `int32`, локальная процедура `getmin(arr, n)`, показ в отладчике |
| `SE_asm01a` | библиотека | MASM | `getmin`, `getmax` |
| `SE_asm01b` | консоль | MASM | вызывает asm-библиотеку, печатает `getmax + getmin = xxxxx` |
| `SE_asm01c` | консоль | C++ | вызывает asm-библиотеку |
| `SE_asm01d` | библиотека | C++ | `getmin`, `getmax` |
| `SE_asm01e` | консоль | MASM | вызывает C++-библиотеку, печатает результат |

## Решения

1. Платформа только `Win32` (x86), конфигурации `Debug|Win32` и `Release|Win32`.
2. MASM подключён через build customization: `masm.props` / `masm.targets`, элементы `<MASM Include="*.asm"/>`.
3. Директивы модуля: `.586`, `.model flat, C`.
4. Ассемблерные exe без CRT: `EntryPointSymbol = _main`, `main PROC C`, зависимость только от `kernel32.lib`.
5. Внутри asm-модулей — `stdcall` (`getmin PROC STDCALL`), на границе с C++ — `cdecl`
   (`getmin PROC C` ↔ `extern "C" int __cdecl getmin(const int*, int)`), оба дают символ `_getmin`.
6. Число в строку Windows-1251: деление на 10, цифры `'0'..'9'`, знак `'-'`; ASCII совпадает с CP1251.
7. Вывод — `WriteConsoleA` (без CRT).
8. C++-библиотека собрана с `BasicRuntimeChecks = Default`, чтобы в Debug не появились `__RTC_*`.
9. Тестовые данные: `15, -3, 27, -8, 42, 0, -19, 33, 7, -25` → `getmin = -25`, `getmax = 42`, сумма `17`.

## Порядок

1. `SE_asm01.sln` + каталоги проектов.
2. `SE_asm01`: `main.asm` — `getmin`, `IntToStr`, `print`, `main`.
3. `SE_asm01a`: `minmax.asm` — `getmin`, `getmax`.
4. `SE_asm01b`: asm-exe + ссылка на `SE_asm01a`.
5. `SE_asm01c`: C++-exe + ссылка на `SE_asm01a`.
6. `SE_asm01d`: C++-библиотека.
7. `SE_asm01e`: asm-exe + ссылка на `SE_asm01d`.
8. `README.md` — сборка, вывод, работа в отладчике.
