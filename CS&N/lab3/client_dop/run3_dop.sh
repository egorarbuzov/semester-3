#!/bin/bash
#
# Дополнительное задание к лабораторной работе №3.
# Запускает несколько клиентов (result/client_dop_test) одновременно —
# по одному в окне Terminal.
#
# Использование:
#   ./run3_dop.sh                 — 3 интерактивных клиента (по умолчанию)
#   ./run3_dop.sh 2               — 2 интерактивных клиента
#   ./run3_dop.sh 3 LIST          — все 3 клиента сразу посылают LIST
#   ./run3_dop.sh 3 'LIST' 'ADD|Ivanov Ivan|IU7-31B|5000|5,4,5' 'DELETE|Petrov Petr Petrovich'
#                                 — каждому клиенту своя команда (команд может быть меньше, чем клиентов:
#                                   остальным окнам достанется интерактивный режим)
#
# Зачем несколько клиентов: обязательная часть только читает список,
# а доп. задание его изменяет. Одновременный доступ (и работу мьютекса)
# видно только тогда, когда клиентов несколько. Опыт из методички:
# поставь Sleep(5000) внутри критической секции и запусти двух клиентов —
# второй будет ждать пять секунд; вынеси Sleep за секцию — оба ответят сразу.

DIR="$(cd "$(dirname "$0")" && pwd)"
CLIENT="$(cd "$DIR/.." && pwd)/result/client_dop_test"

if [ ! -x "$CLIENT" ]; then
    echo "Ошибка: $CLIENT не найден или не исполняемый"
    echo "Собери клиент: clang++ -std=c++17 client_dop/main.cpp -o result/client_dop_test"
    exit 1
fi

N="${1:-3}"
shift

case "$N" in
    ''|*[!0-9]*|0)
        echo "Ошибка: количество клиентов должно быть положительным числом (получено '$N')"
        exit 1
        ;;
esac

if [ "$N" -gt 10 ]; then
    echo "Ошибка: слишком много клиентов ($N), максимум 10"
    exit 1
fi

# Сервер должен уже слушать порт
if ! nc -z 127.0.0.1 1280 2>/dev/null; then
    echo "Предупреждение: на порту 1280 никто не слушает."
    echo "Сначала запусти сервер: ./result/dop_server3"
fi

# Экранирование строки для AppleScript
esc() {
    printf '%s' "$1" | sed -e 's/\\/\\\\/g' -e 's/"/\\"/g'
}

CMDS=("$@")

{
    echo 'tell application "Terminal"'
    echo '    activate'
    for ((i = 0; i < N; i++)); do
        cmd="${CMDS[$i]:-}"
        if [ -n "$cmd" ]; then
            # Команды кладём в файл и подаём клиенту на stdin: так они не проходят
            # через AppleScript, и любые кавычки/апострофы в ФИО остаются целыми.
            f="$(mktemp "${TMPDIR:-/tmp}/run3_dop.XXXXXX")"
            printf '%s\n\n' "$cmd" > "$f"
            full="'$CLIENT' < '$f'; rm -f '$f'; echo; echo '--- готово, нажми Enter ---'; read -r _"
        else
            full="'$CLIENT'"
        fi
        echo "    do script \"$(esc "$full")\""
    done
    echo 'end tell'
} | osascript

echo "Запущено клиентов: $N"
if [ ${#CMDS[@]} -gt 0 ]; then
    echo "Автокоманды: ${CMDS[*]}"
fi
