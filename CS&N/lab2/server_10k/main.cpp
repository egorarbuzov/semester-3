#include <iostream>
#include <cstring>
#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cerrno>

using std::cout;
using std::endl;
using std::string;

int main() {
    int s = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) { perror("socket"); return 1; }

    int reuse = 1;
    ::setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    sockaddr_in local{};
    local.sin_family      = AF_INET;
    local.sin_port        = htons(1024);
    local.sin_addr.s_addr = htonl(INADDR_ANY);

    if (::bind(s, (sockaddr*)&local, sizeof(local)) < 0) {
        perror("bind"); ::close(s); return 1;
    }

    char buf[512];
    while (true) {
        sockaddr_in from{};
        socklen_t fromlen = sizeof(from);

        int rv = ::recvfrom(s, buf, sizeof(buf) - 1, 0,
                            (sockaddr*)&from, &fromlen);

        if (rv < 0) {
            if (errno == EMSGSIZE) {
                cout << "recvfrom: EMSGSIZE — datagram too big, "
                     << "rest LOST FOREVER" << endl;
            } else {
                perror("recvfrom");
            }
            continue;
        }

        buf[rv] = '\0';
        cout << "recvfrom() #1 got " << rv << " bytes (buffer = "
             << sizeof(buf) << ")" << endl;
        cout << "First 20: " << string(buf, rv < 20 ? rv : 20) << "..." << endl;

        string response(buf, rv);
        ::sendto(s, response.c_str(), response.size(), 0,
                 (sockaddr*)&from, fromlen);
    }

    ::close(s);
    return 0;
}
