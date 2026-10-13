#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <cerrno>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::ostringstream;

struct Student {
    string fio;           // ФИО
    string group;         // номер группы
    double stipend;       // стипендия
    vector<int> grades;   // оценки по N предметам
};

const vector<Student> students = {
    {"Ivanov Ivan Ivanovich",     "IU7-31B", 5000.0, {5, 4, 5, 4, 5}},
    {"Petrov Petr Petrovich",     "IU7-32B", 4500.0, {4, 4, 4, 3, 5}},
    {"Sidorov Alexey Sergeevich", "IU7-31B", 5500.0, {5, 5, 5, 5, 5}},
    {"Alekseeva Maria Ivanovna",  "IU7-33B", 4000.0, {3, 4, 3, 4, 3}},
    {"Andreev Andrey Andreevich", "IU7-32B", 5000.0, {5, 4, 5, 4, 4}},
    {"Smirnova Olga Pavlovna",    "IU7-33B", 6000.0, {5, 5, 4, 5, 5}},
};

char getFirstLetter(const string& fio) {
    if (fio.empty()) return '\0';
    return (char)std::toupper((unsigned char)fio[0]);
}

string findStudentsByLetter(char letter) {
    letter = (char)std::toupper((unsigned char)letter);
    ostringstream out;
    int count = 0;

    for (const auto& s : students) {
        if (getFirstLetter(s.fio) == letter) {
            out << "ФИО: "       << s.fio      << "\n"
                << "Группа: "    << s.group    << "\n"
                << "Стипендия: " << s.stipend  << "\n"
                << "Оценки: ";
            for (size_t i = 0; i < s.grades.size(); ++i) {
                if (i) out << ", ";
                out << s.grades[i];
            }
            out << "\n---\n";
            ++count;
        }
    }

    if (count == 0) {
        return "Студентов с фамилией на букву '" + string(1, letter) + "' не найдено.\n";
    }

    ostringstream header;
    header << "Найдено студентов: " << count << "\n\n";
    return header.str() + out.str();
}

void* threadFunc(void* param) {
    int s2 = *(int*)param;
    delete (int*)param;

    cout << "[thread " << s2 << "] started" << endl;

    char buf[1024];
    int n;
    while ((n = ::recv(s2, buf, sizeof(buf) - 1, 0)) > 0) {
        buf[n] = '\0';
        string request(buf, n);

        while (!request.empty() &&
               (request.back() == '\n' || request.back() == '\r' || request.back() == ' ')) {
            request.pop_back();
        }

        cout << "[thread " << s2 << "] request: \"" << request << "\"" << endl;

        string response;
        if (request.empty()) {
            response = "Ошибка: пустой запрос.\n";
        } else if (request.size() > 1) {
            response = "Ошибка: нужно ввести одну букву.\n";
        } else if (!std::isalpha((unsigned char)request[0])) {
            response = "Ошибка: введите букву алфавита.\n";
        } else {
            response = findStudentsByLetter(request[0]);
        }

        if (::send(s2, response.c_str(), response.size(), 0) < 0) {
            perror("send");
            break;
        }
    }

    if (n < 0) perror("recv");
    ::close(s2);
    cout << "[thread " << s2 << "] finished" << endl;
    return nullptr;
}

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

    cout << "Multithreaded TCP server on port 1280" << endl;
    cout << "Variant 1: list of students, filter by first letter" << endl;
    cout << "Total students in DB: " << students.size() << endl;

    while (true) {
        sockaddr_in remote{};
        socklen_t size = sizeof(remote);

        int s2 = ::accept(s, (sockaddr*)&remote, &size);
        if (s2 < 0) {
            perror("accept");
            continue;
        }

        cout << "Accepted client: " << inet_ntoa(remote.sin_addr)
             << ":" << ntohs(remote.sin_port)
             << " (socket " << s2 << ")" << endl;

        int* arg = new int(s2);
        pthread_t tid;
        int rc = pthread_create(&tid, nullptr, threadFunc, arg);
        if (rc != 0) {
            perror("pthread_create");
            ::close(s2);
            delete arg;
        } else {
            pthread_detach(tid);
        }
    }

    ::close(s);
    return 0;
}
