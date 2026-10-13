#include <iostream>
#include <cstring>
#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::getline;

int main() {
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

    cout << "Connected to server. Enter a letter (empty line to quit):" << endl;

    string line;
    while (true) {
        cout << "> ";
        if (!getline(cin, line)) break;
        if (line.empty()) break;

        if (::send(s, line.c_str(), line.size(), 0) < 0) {
            perror("send");
            break;
        }

        string response;
        char buf[1024];
        int n;
        while (true) {
            n = ::recv(s, buf, sizeof(buf) - 1, 0);
            if (n <= 0) break;
            buf[n] = '\0';
            response.append(buf, n);

            if (response.size() >= 4 &&
                (response.rfind("---\n") == response.size() - 4 ||
                 response.rfind("не найдено.\n") != string::npos)) {
                break;
            }
            if (response.rfind("Ошибка:") == 0 &&
                response.back() == '\n') {
                break;
            }
        }

        if (n < 0) {
            perror("recv");
            break;
        }

        cout << "--- Server response ---" << endl;
        cout << response;
        cout << "-----------------------" << endl;
    }

    ::shutdown(s, SHUT_WR);
    ::close(s);
    return 0;
}
