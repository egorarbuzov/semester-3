#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

int main() {
    setlocale(LC_ALL, "ru");
    std::ifstream in("data.bin", std::ios::binary);
    std::ofstream asmFile("../asm/data.asm");

    if (!in || !asmFile) {
        std::cout << "Ошибка: Невозможно открыть файлы!\n";
        return 1;
    }

    asmFile << ".586P\n.MODEL FLAT, STDCALL\nincludelib kernel32.lib\n"
        << "ExitProcess PROTO : DWORD\n"
        << ".DATA\n";

    char type;
    int intCount = 0;
    int strCount = 0;

    while (in.read(&type, 1)) {
        if (type == 1) {
            int val;
            in.read((char*)&val, 4);
            intCount++;

            asmFile << "int" << intCount << "_type DB 01h\n";
            asmFile << "int" << intCount << "      DWORD " << std::dec << val << "\n";
        }

        else if (type == 2) {
            char len;
            in.read(&len, 1);

            std::string str(len, '\0');
            in.read(&str[0], len);

            strCount++;

            asmFile << "str" << strCount << "_type DB 02h\n";
            asmFile << "str" << strCount << "_len  DB " << (int)(unsigned char)len << "\n";

            if (str.empty()) {
                asmFile << "str" << strCount << "      DB 0\n";
                continue;
            }

            asmFile << "str" << strCount << "      DB ";

            for (size_t i = 0; i < str.size(); i++) {
                size_t count = 1;
                while (i + count < str.size() && str[i] == str[i + count]) {
                    count++;
                }

                if (count >= 3) {
                    asmFile << std::dec << count << " DUP(0" << std::hex << std::uppercase << (int)(unsigned char)str[i] << "h), ";
                    i += count - 1;
                }
                else {
                    for (size_t j = 0; j < count; j++) {
                        asmFile << "0" << std::hex << std::uppercase << (int)(unsigned char)str[i] << "h, ";
                    }
                    i += count - 1;
                }
            }

            asmFile.seekp(-2, std::ios::cur);
            asmFile << ", 0\n";
        }
    }

    asmFile << "\n.CODE\nmain PROC\n";

    for (int i = 1; i <= intCount; i++) {
        asmFile << "    MOV EAX, int" << i << "\n";
    }
    for (int i = 1; i <= strCount; i++) {
        asmFile << "    LEA ESI, str" << i << "\n";
    }

    asmFile << "    PUSH 0\n    CALL ExitProcess\n"
        << "main ENDP\nend main\n";

    std::cout << "Десериализация завершена, создан data.asm\n";
    return 0;
}