#include <iostream>
#include <cstring>
#include <cstdlib>
#include <string>
#include <sstream>

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

    int reuse = 1;
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("setsockopt");
        close(s);
        return 1;
    }

    sockaddr_in local{};
    local.sin_family = AF_INET;
    local.sin_port = htons(1280);
    local.sin_addr.s_addr = htonl(INADDR_ANY);

    if (::bind(s, (sockaddr*)&local, sizeof(local)) < 0) {
        perror("bind");
        close(s);
        return 1;
    }

    if (listen(s, SOMAXCONN) < 0) {
        perror("listen");
        close(s);
        return 1;
    }

    cout << "Server started on port 1280. Waiting for clients..." << endl;

    while (true) {
        sockaddr_in remote{};
        socklen_t size = sizeof(remote);

        int s2 = accept(s, (sockaddr*)&remote, &size);
        if (s2 < 0) {
            perror("accept");
            continue;
        }

        cout << "Client connected: " << inet_ntoa(remote.sin_addr) << ":" << ntohs(remote.sin_port) << endl;

        char buf[255];
        int n;
        while ((n = recv(s2, buf, sizeof(buf) - 1, 0)) > 0) {
            buf[n] = '\0';
            cout << "Received: " << buf << endl;

            istringstream iss(buf);
            double a, b;
            char op;
            string result;

            if (!(iss >> a >> op >> b)) {
                result = "Error: expected format '<num> <op> <num>'";
            } else {
                double r = 0;
                bool ok = true;
                switch (op) {
                    case '+': r = a + b; break;
                    case '-': r = a - b; break;
                    case '*': r = a * b; break;
                    case '/':
                        if (b == 0) { result = "Error: division by zero"; ok = false; }
                        else r = a / b;
                        break;
                    default:
                        result = "Error: unknown operator";
                        ok = false;
                }
                if (ok) {
                    ostringstream oss;
                    oss << r;
                    result = oss.str();
                }
            }

            if (send(s2, result.c_str(), result.size(), 0) < 0) {
                perror("send");
                break;
            }
        }

        if (n < 0) perror("recv");

        close(s2);
        cout << "Client disconnected." << endl;
    }

    close(s);
    return 0;
}
