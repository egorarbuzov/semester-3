#!/bin/bash
#
# Запуск двух клиентов доп. задания — по одному в окне Terminal.
# В каждом окне введи команду, например LIST, и нажми Enter.
#
# Запуск:  ./run2_dop.sh

DIR="$(cd "$(dirname "$0")" && pwd)"
CLIENT="$DIR/client_dop_test"

if [ ! -x "$CLIENT" ]; then
    echo "Ошибка: нет $CLIENT"
    echo "Собери: clang++ -std=c++17 client_dop/main.cpp -o result/client_dop_test"
    exit 1
fi

osascript <<EOF
tell application "Terminal"
    activate
    do script "'$CLIENT'"
    do script "'$CLIENT'"
end tell
EOF
