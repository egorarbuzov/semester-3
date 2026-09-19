#include <iostream>
#include <cstring>
#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using std::cout;
using std::endl;
using std::string;

int main() {
    int s = ::socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) { perror("socket"); return 1; }

    sockaddr_in peer{};
    peer.sin_family = AF_INET;
    peer.sin_port   = htons(1280);
    inet_pton(AF_INET, "127.0.0.1", &peer.sin_addr);

    if (::connect(s, (sockaddr*)&peer, sizeof(peer)) < 0) {
        perror("connect");
        ::close(s);
        return 1;
    }

    string big(10000, 'a');

    int sent = ::send(s, big.c_str(), (int)big.size(), 0);
    cout << "send() returned: " << sent << endl;

    string reply;
    char tmp[4096];
    int n;
    while ((n = ::recv(s, tmp, sizeof(tmp), 0)) > 0) {
        reply.append(tmp, n);
    }

    cout << "recv() got total " << reply.size() << " bytes" << endl;
    cout << "First 20 chars: " << reply.substr(0, 20) << "..." << endl;
    cout << "(Expected total: 10000)" << endl;

    ::close(s);
    return 0;
}
