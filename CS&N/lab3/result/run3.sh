
DIR="$(cd "$(dirname "$0")" && pwd)"
CLIENT="$DIR/client_3"

if [ ! -x "$CLIENT" ]; then
    echo "Ошибка: $CLIENT не найден или не исполняемый"
    echo "Собери клиент: clang++ -std=c++17 -Wall -pthread client_v1.cpp -o client_v1"
    exit 1
fi

# Открываем три окна Terminal, в каждом — клиент
osascript <<EOF
tell application "Terminal"
    activate
    do script "cd '$DIR' && ./client_3"
    do script "cd '$DIR' && ./client_3"
    do script "cd '$DIR' && ./client_3"
end tell
EOF

echo "Запущены 3 клиента. Введи букву в каждом окне."