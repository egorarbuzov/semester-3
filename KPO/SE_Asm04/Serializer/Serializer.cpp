#include <iostream>
#include <fstream>
#include <string>

const char TYPE_INT = 0x01;
const char TYPE_STR = 0x02;
const size_t MAX_STR_LEN = 127;

void writeInt(std::ofstream& out, int value) {
    out.put(TYPE_INT);
    out.write((char*)&value, 4);
}

void writeStr(std::ofstream& out, const std::string& str) {
    if (str.length() > MAX_STR_LEN) {
        std::cout << "Ошибка: строка длиннее 127 байт не будет записана!\n";
        return;
    }
    out.put(TYPE_STR);
    out.put((char)str.length());
    out.write(str.c_str(), str.length());
}

int main() {
    setlocale(LC_ALL, "ru");
    std::ofstream out("../Deserializer/data.bin", std::ios::binary);
    if (!out) return 1;

    writeInt(out, 123);
    writeStr(out, "Hell!");

    writeInt(out, 1);
    writeStr(out, "");

    writeInt(out, -1);

    std::string longStr(128, 'A');
    writeStr(out, longStr);

    writeStr(out, "X");

    std::cout << "Сериализация завершена, создан data.bin\n";
    return 0;
}