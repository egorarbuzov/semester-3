# Лабораторная работа 19. Введение в Ассемблер

Решение Visual Studio 2026: [`SE_asm01.sln`](SE_asm01.sln), MASM, платформа **Win32 (x86)**.
План работы — [`PLAN.md`](PLAN.md).

## Проекты

| Проект | Тип | Язык | Что делает |
|--------|-----|------|-----------|
| `SE_asm01` | exe | MASM | массив 10 `DWORD`, локальная процедура `getmin`, вывод результата |
| `SE_asm01a` | lib | MASM | `getmin`, `getmax` |
| `SE_asm01b` | exe | MASM | вызывает `getmin`/`getmax` из `SE_asm01a` |
| `SE_asm01c` | exe | C++ | вызывает `getmin`/`getmax` из `SE_asm01a` |
| `SE_asm01d` | lib | C++ | `getmin`, `getmax` |
| `SE_asm01e` | exe | MASM | вызывает `getmin`/`getmax` из `SE_asm01d` |

Связи заданы через `ProjectReference`.

## Сборка

1. Открыть `SE_asm01.sln`.
2. Конфигурация `Debug`, платформа `Win32`.
3. `Ctrl+Shift+B`, затем `Ctrl+F5` (проект — стартовый).

Набор инструментов в проектах: `v145` (VS 2026). Если он не установлен — выполнить
*Retarget Projects* или заменить `v145` на `v143` в файлах `*.vcxproj`.

## Ожидаемый вывод

`SE_asm01`: `getmin = -25`

`SE_asm01b`, `SE_asm01c`, `SE_asm01e`: `getmax + getmin = 17`

Массив: `15, -3, 27, -8, 42, 0, -19, 33, 7, -25`.

## Отладчик (п. 4 задания)

1. Сборка `Debug|Win32`, `F11` — остановка на `main`.
2. `Debug → Windows → Registers`, `Memory 1` (в поле Address — `arr`).
3. `F9` на `invoke getmin`, затем `F5`.
4. `F11` в `getmin`, `F10` по циклу: `EAX` — минимум, `ECX` — счётчик, `ESI` — указатель.
   Параметры лежат по `[EBP+8]` и `[EBP+C]`.
5. После `ret` в `EAX` значение `-25` (`0xFFFFFFE7`).

## Кратко о реализации

* `.586`, `.model flat, C`; MASM подключён как build customization (`masm.props`/`masm.targets`).
* Ассемблерные exe не используют CRT: `Linker → Entry Point = _main` (`main PROC C`), только `kernel32.lib`.
* Граница asm ↔ C++ — `__cdecl`: в MASM `getmin PROTO C :DWORD, :DWORD` (символ `_getmin`),
  в C++ `extern "C" int __cdecl getmin(const int*, int)`.
* Число в строку Windows-1251 переводится процедурой `IntToStr` (деление на 10, знак `'-'`);
  ASCII-символы в CP1251 совпадают, вывод — через `WriteConsoleA`.
* C++ библиотека `SE_asm01d` собрана с `BasicRuntimeChecks = Default`, чтобы в Debug
  не появились ссылки на CRT-инициализацию (`__RTC_*`).
