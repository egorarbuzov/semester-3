#include <iostream>
#include <cstring>
#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cerrno>
#include <sys/time.h>

using std::cout;
using std::endl;
using std::string;

int main() {
    int s = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) { perror("socket"); return 1; }

    sockaddr_in server{};
    server.sin_family      = AF_INET;
    server.sin_port        = htons(1024);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    timeval timeout{};
    timeout.tv_sec = 3;
    ::setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    string big(9000, 'a');
    cout << "Sending " << big.size() << " bytes in ONE datagram..." << endl;

    int sent = ::sendto(s, big.c_str(), big.size(), 0,
                        (sockaddr*)&server, sizeof(server));
    if (sent < 0) {
        perror("sendto");
        cout << "errno = " << errno << endl;
    }
    cout << "sendto() returned: " << sent << endl;

    char buf[20000];
    socklen_t serverLen = sizeof(server);
    int rv = ::recvfrom(s, buf, sizeof(buf) - 1, 0,
                        (sockaddr*)&server, &serverLen);

    if (rv < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            cout << "Ответ не получен. Сервер запущен?" << endl;
        } else {
            perror("recvfrom");
        }
    } else {
        buf[rv] = '\0';
        cout << "recvfrom() got total " << rv << " bytes" << endl;
        cout << "First 20: " << string(buf, rv < 20 ? rv : 20) << "..." << endl;
        cout << "(Expected total: 9000)" << endl;
    }

    ::close(s);
    return 0;
}
