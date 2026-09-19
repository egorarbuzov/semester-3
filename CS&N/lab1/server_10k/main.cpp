#include <iostream>
#include <cstring>
#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using std::cout;
using std::endl;

int main() {
    int s = ::socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("socket");
        return 1;
    }

    int reuse = 1;
    if (::setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("setsockopt");
        ::close(s);
        return 1;
    }

    sockaddr_in local{};
    local.sin_family      = AF_INET;
    local.sin_port        = htons(1280);
    local.sin_addr.s_addr = htonl(INADDR_ANY);

    if (::bind(s, (sockaddr*)&local, sizeof(local)) < 0) {
        perror("bind");
        ::close(s);
        return 1;
    }

    if (::listen(s, SOMAXCONN) < 0) {
        perror("listen");
        ::close(s);
        return 1;
    }

    cout << "Echo server on port 1280. Waiting for clients..." << endl;

    while (true) {
        sockaddr_in remote{};
        socklen_t size = sizeof(remote);

        int s2 = ::accept(s, (sockaddr*)&remote, &size);
        if (s2 < 0) {
            perror("accept");
            continue;
        }

        cout << "Client connected: " << inet_ntoa(remote.sin_addr)
             << ":" << ntohs(remote.sin_port) << endl;

        char buf[255];
        int n;
        int recvCount = 0;
        int totalBytes = 0;

        while ((n = ::recv(s2, buf, sizeof(buf) - 1, 0)) > 0) {
            recvCount++;
            totalBytes += n;

            cout << "recv() #" << recvCount
                 << " got " << n << " bytes"
                 << " (total " << totalBytes << ")" << endl;

            int sent = 0;
            while (sent < n) {
                int r = ::send(s2, buf + sent, n - sent, 0);
                if (r <= 0) {
                    perror("send");
                    break;
                }
                sent += r;
            }
        }

        if (n < 0) perror("recv");

        cout << "Client disconnected. recv() called "
             << recvCount << " times, total "
             << totalBytes << " bytes." << endl;

        ::close(s2);
    }

    ::close(s);
    return 0;
}
