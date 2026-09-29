#include <iostream>
#include <cstring>
#include <string>
#include <cctype>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using std::cout;
using std::endl;
using std::string;

string removeMultiplesOfThree(const string& s) {
    string result;
    for (char c : s) {
        if (c == '0' || c == '3' || c == '6' || c == '9') {
            continue;
        }
        result += c;
    }
    return result;
}

int main() {
    int s = ::socket(AF_INET, SOCK_DGRAM, 0);
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
    local.sin_port        = htons(1024);
    local.sin_addr.s_addr = htonl(INADDR_ANY);

    // 4. bind
    if (::bind(s, (sockaddr*)&local, sizeof(local)) < 0) {
        perror("bind");
        ::close(s);
        return 1;
    }

    cout << "UDP server started on port 1024. Waiting for datagrams..." << endl;

    char buf[1024];
    while (true) {
        sockaddr_in from{};
        socklen_t fromlen = sizeof(from);

        int rv = ::recvfrom(s, buf, sizeof(buf) - 1, 0, (sockaddr*)&from, &fromlen);
        if (rv < 0) {
            perror("recvfrom");
            continue;
        }

        buf[rv] = '\0';
        string received(buf);
        cout << "Received from " << inet_ntoa(from.sin_addr)
             << ":" << ntohs(from.sin_port)
             << " -> \"" << received << "\" (len=" << rv << ")" << endl;

        string response;
        if (received.size() % 3 == 0) {
            response = removeMultiplesOfThree(received);
            cout << "Length is multiple of 3 -> removing 0,3,6,9" << endl;
        } else {
            response = received;
            cout << "Length is NOT multiple of 3 -> unchanged" << endl;
        }

        if (::sendto(s, response.c_str(), response.size(), 0,
                     (sockaddr*)&from, fromlen) < 0) {
            perror("sendto");
        } else {
            cout << "Sent back: \"" << response << "\"" << endl;
        }
    }

    ::close(s);
    return 0;
}
