#include <iostream>
#include <cstring>
#include <string>
#include <chrono>      

#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::getline;

int recvResponse(int s, string& response) {
    response.clear();

    char buf[1024];
    bool gotAny = false;

    while (true) {
        if (gotAny) {
            fd_set rd;
            FD_ZERO(&rd);
            FD_SET(s, &rd);

            timeval tv{};
            tv.tv_sec  = 0;
            tv.tv_usec = 200000;   //0.2 с — пауза между частями одного ответа

            if (::select(s + 1, &rd, nullptr, nullptr, &tv) <= 0) break;
        }

        int n = ::recv(s, buf, sizeof(buf) - 1, 0);
        if (n <= 0) return n;      //0 — сервер закрыл соединение, -1 — ошибка
        buf[n] = '\0';
        response.append(buf, n);
        gotAny = true;
    }

    return (int)response.size();
}

//[ТЕСТ КРИТИЧЕСКОЙ СЕКЦИИ] здесь изменена сигнатура (в исходном клиенте было int main())
int main(int argc, char** argv) {
    int s = ::socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("socket");
        return 1;
    }

    sockaddr_in peer{};
    peer.sin_family      = AF_INET;
    peer.sin_port        = htons(1280);
    peer.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (::connect(s, (sockaddr*)&peer, sizeof(peer)) < 0) {
        perror("connect");
        ::close(s);
        return 1;
    }

    cout << "Connected to server. Extra task: find, add, delete, list" << endl;
    cout << "Commands:" << endl;
    cout << "  <letter>                        - find students by first letter" << endl;
    cout << "  ADD|FIO|GROUP|STIPEND|G1,G2,... - add student" << endl;
    cout << "  DELETE|FIO                      - delete student by FIO" << endl;
    cout << "  LIST                            - show all students" << endl;
    cout << "Empty line to quit." << endl;

    string argvCommand;
    for (int i = 1; i < argc; ++i) {
        if (i > 1) argvCommand += ' ';
        argvCommand += argv[i];
    }

    string line;
    while (true) {
        if (!argvCommand.empty()) { line = argvCommand; } else
        {
            cout << "> ";
            if (!getline(cin, line)) break;
            if (line.empty()) break;
        }

        const auto t0 = std::chrono::steady_clock::now();   // старт замера

        if (::send(s, line.c_str(), line.size(), 0) < 0) {
            perror("send");
            break;
        }

        string response;
        int n = recvResponse(s, response);

        if (n < 0) {
            perror("recv");
            break;
        }
        if (n == 0) {
            cout << "Server closed connection." << endl;
            break;
        }

        cout << "--- Server response ---" << endl;
        cout << response;
        if (response.back() != '\n') cout << endl;
        cout << "-----------------------" << endl;

        const double waited =
            std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();
        cout << "[time] ответ получен за " << waited << " с" << endl;
        if (!argvCommand.empty()) break;  
    }

    ::shutdown(s, SHUT_WR);
    ::close(s);
    return 0;
}
