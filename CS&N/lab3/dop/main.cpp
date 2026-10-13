#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>
#include <algorithm>

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
    string fio;
    string group;
    double stipend;
    vector<int> grades;
};

vector<Student> students = {
    {"Ivanov Ivan Ivanovich",     "IU7-31B", 5000.0, {5, 4, 5, 4, 5}},
    {"Petrov Petr Petrovich",     "IU7-32B", 4500.0, {4, 4, 4, 3, 5}},
    {"Sidorov Alexey Sergeevich", "IU7-31B", 5500.0, {5, 5, 5, 5, 5}},
    {"Alekseeva Maria Ivanovna",  "IU7-33B", 4000.0, {3, 4, 3, 4, 3}},
    {"Andreev Andrey Andreevich", "IU7-32B", 5000.0, {5, 4, 5, 4, 4}},
    {"Smirnova Olga Pavlovna",    "IU7-33B", 6000.0, {5, 5, 4, 5, 5}},
};

pthread_mutex_t studentsMutex = PTHREAD_MUTEX_INITIALIZER;

char getFirstLetter(const string& fio) {
    if (fio.empty()) return '\0';
    return (char)std::toupper((unsigned char)fio[0]);
}

string studentToString(const Student& s) {
    ostringstream out;
    out << "Name: "    << s.fio     << "\n"
        << "Group: "   << s.group   << "\n"
        << "Stipend: " << s.stipend << "\n"
        << "Grades: ";
    for (size_t i = 0; i < s.grades.size(); ++i) {
        if (i) out << ", ";
        out << s.grades[i];
    }
    out << "\n";
    return out.str();
}

string findStudentsByLetter(char letter) {
    letter = (char)std::toupper((unsigned char)letter);

    pthread_mutex_lock(&studentsMutex);   // <<< ЗАХВАТ

    ostringstream out;
    int count = 0;
    for (const auto& s : students) {
        if (getFirstLetter(s.fio) == letter) {
            out << studentToString(s) << "---\n";
            ++count;
        }
    }

    pthread_mutex_unlock(&studentsMutex); // <<< ОСВОБОЖДЕНИЕ

    if (count == 0) {
        return "No students with last name starting with '"
               + string(1, letter) + "'.\n";
    }
    ostringstream header;
    header << "Found students: " << count << "\n\n";
    return header.str() + out.str();
}

string addStudent(const string& args) {
    vector<string> parts;
    string token;
    std::istringstream iss(args);
    while (std::getline(iss, token, '|')) {
        parts.push_back(token);
    }

    if (parts.size() != 4) {
        return "Error ADD: expected FIO|GROUP|STIPEND|GRADES\n";
    }

    Student s;
    s.fio   = parts[0];
    s.group = parts[1];
    try {
        s.stipend = std::stod(parts[2]);
    } catch (...) {
        return "Error ADD: stipend is not a number\n";
    }

    // Разбираем оценки по запятым
    std::istringstream giss(parts[3]);
    string g;
    while (std::getline(giss, g, ',')) {
        try {
            s.grades.push_back(std::stoi(g));
        } catch (...) {
            return "Error ADD: grade is not a number\n";
        }
    }

    if (s.fio.empty()) {
        return "Error ADD: empty FIO\n";
    }

    pthread_mutex_lock(&studentsMutex);   // <<< ЗАХВАТ

    // Проверка на дубликат
    for (const auto& existing : students) {
        if (existing.fio == s.fio) {
            pthread_mutex_unlock(&studentsMutex);
            return "Error ADD: student with this FIO already exists\n";
        }
    }
    students.push_back(s);

    pthread_mutex_unlock(&studentsMutex); // <<< ОСВОБОЖДЕНИЕ

    return "Added: " + s.fio + "\n";
}

string deleteStudent(const string& fio) {
    if (fio.empty()) {
        return "Error DELETE: empty FIO\n";
    }

    pthread_mutex_lock(&studentsMutex);   // <<< ЗАХВАТ

    auto it = std::find_if(students.begin(), students.end(),
                           [&](const Student& s) { return s.fio == fio; });
    if (it == students.end()) {
        pthread_mutex_unlock(&studentsMutex);
        return "Error DELETE: student not found: " + fio + "\n";
    }
    students.erase(it);

    pthread_mutex_unlock(&studentsMutex); // <<< ОСВОБОЖДЕНИЕ

    return "Deleted: " + fio + "\n";
}

string listAll() {
    pthread_mutex_lock(&studentsMutex);
    sleep(5);
    ostringstream out;
    out << "Total students: " << students.size() << "\n\n";
    for (const auto& s : students) {
        out << studentToString(s) << "---\n";
    }
    pthread_mutex_unlock(&studentsMutex);
    return out.str();
}

void* threadFunc(void* param) {
    int s2 = *(int*)param;
    delete (int*)param;

    cout << "[thread " << s2 << "] started" << endl;

    char buf[2048];
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

        // --- Разбор команды ---
        if (request == "LIST") {
            response = listAll();
        } else if (request.rfind("ADD|", 0) == 0) {
            response = addStudent(request.substr(4));
        } else if (request.rfind("DELETE|", 0) == 0) {
            response = deleteStudent(request.substr(7));
        } else if (request.size() == 1 &&
                   std::isalpha((unsigned char)request[0])) {
            response = findStudentsByLetter(request[0]);
        } else if (request.empty()) {
            response = "Error: empty request.\n";
        } else {
            response =
                "Commands:\n"
                "  <letter>                  - find students by first letter\n"
                "  ADD|FIO|GROUP|STIPEND|G1,G2,... - add student\n"
                "  DELETE|FIO                - delete student by FIO\n"
                "  LIST                      - show all students\n";
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
    if (s < 0) { perror("socket"); return 1; }

    int reuse = 1;
    if (::setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("setsockopt"); ::close(s); return 1;
    }

    sockaddr_in local{};
    local.sin_family      = AF_INET;
    local.sin_port        = htons(1280);
    local.sin_addr.s_addr = htonl(INADDR_ANY);

    if (::bind(s, (sockaddr*)&local, sizeof(local)) < 0) {
        perror("bind"); ::close(s); return 1;
    }

    if (::listen(s, SOMAXCONN) < 0) {
        perror("listen"); ::close(s); return 1;
    }

    cout << "Multithreaded TCP server on port 1280" << endl;
    cout << "Variant 1 (extra): find by letter + ADD + DELETE + LIST" << endl;
    cout << "Total students in DB: " << students.size() << endl;

    while (true) {
        sockaddr_in remote{};
        socklen_t size = sizeof(remote);

        int s2 = ::accept(s, (sockaddr*)&remote, &size);
        if (s2 < 0) { perror("accept"); continue; }

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

    pthread_mutex_destroy(&studentsMutex);
    ::close(s);
    return 0;
}