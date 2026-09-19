#include <iostream>
#include <cstring>
#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

int main() {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("socket");
        return 1;
    }

    sockaddr_in peer{};
    peer.sin_family = AF_INET;
    peer.sin_port = htons(1280);
    peer.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(s, (sockaddr*)&peer, sizeof(peer)) < 0) {
        perror("connect");
        close(s);
        return 1;
    }

    cout << "Connected to server. Enter expression like '5 * 3' (empty line to quit):" << endl;

    string line;
    while (true) {
        cout << "> ";
        if (!getline(cin, line)) break;
        if (line.empty()) break;

        if (send(s, line.c_str(), line.size(), 0) < 0) {
            perror("send");
            break;
        }

        char b[255];
        int n = recv(s, b, sizeof(b) - 1, 0);
        if (n <= 0) {
            cout << "Server closed connection." << endl;
            break;
        }
        b[n] = '\0';
        cout << "Result: " << b << endl;
    }

    close(s);
    return 0;
}
