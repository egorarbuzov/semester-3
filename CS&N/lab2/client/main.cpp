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
    int s = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) {
        perror("socket");
        return 1;
    }

    sockaddr_in server{};
    server.sin_family      = AF_INET;
    server.sin_port        = htons(1024);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    timeval timeout{};
    timeout.tv_sec  = 3;
    timeout.tv_usec = 0;
    if (::setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        perror("setsockopt");
        ::close(s);
        return 1;
    }

    cout << "UDP client. Enter a string (empty line to quit):" << endl;

    string line;
    while (true) {
        cout << "> ";
        if (!getline(cin, line)) break;
        if (line.empty()) break;

        if (::sendto(s, line.c_str(), line.size(), 0,
                     (sockaddr*)&server, sizeof(server)) < 0) {
            perror("sendto");
            break;
        }

        char buf[1024];
        socklen_t serverLen = sizeof(server);
        int rv = ::recvfrom(s, buf, sizeof(buf) - 1, 0,
                            (sockaddr*)&server, &serverLen);

        if (rv < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                cout << "Ответ не получен. Сервер запущен?" << endl;
            } else {
                perror("recvfrom");
            }
            continue;
        }

        buf[rv] = '\0';
        cout << "Result: \"" << buf << "\"" << endl;
    }

    ::close(s);
    return 0;
}
