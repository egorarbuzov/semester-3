#include <iostream>
#include <cctype>
#include <climits>
#include <iomanip>

using namespace std;

const int MAX_V = 9;

int graph[MAX_V][MAX_V];

char indexToChar(int i) { return (char)('A' + i); }
int charToIndex(char c)  { return toupper(c) - 'A'; }

void initGraph() {
    for (int i = 0; i < MAX_V; i++)
        for (int j = 0; j < MAX_V; j++)
            graph[i][j] = 0;

    auto addEdge = [](int u, int v, int w) {
        graph[u][v] = w;
        graph[v][u] = w;
    };

    addEdge(0, 1, 7);   // A - B
    addEdge(0, 2, 10);  // A - C
    addEdge(1, 6, 27);  // B - G
    addEdge(1, 5, 9);   // B - F
    addEdge(2, 5, 8);   // C - F
    addEdge(2, 4, 31);  // C - E
    addEdge(4, 3, 32);  // E - D
    addEdge(5, 7, 11);  // F - H
    addEdge(6, 8, 15);  // G - I
    addEdge(7, 3, 17);  // H - D
    addEdge(7, 8, 15);  // H - I
    addEdge(3, 8, 21);  // D - I
}

int main() {
    setlocale(LC_ALL, "Russian");
    initGraph();

    char startChar;
    cout << "Введите стартовую вершину (A-I): ";
    cin >> startChar;
    startChar = toupper(startChar);

    if (startChar < 'A' || startChar > 'I') {
        cout << "Ошибка: неверная вершина." << endl;
        return 1;
    }

    int start = charToIndex(startChar);

    int dist[MAX_V];
    bool visited[MAX_V];

    for (int i = 0; i < MAX_V; i++) {
        dist[i] = INT_MAX;
        visited[i] = false;
    }
    dist[start] = 0;

    for (int count = 0; count < MAX_V; count++) {
        int u = -1;
        int minDist = INT_MAX;
        for (int i = 0; i < MAX_V; i++) {
            if (!visited[i] && dist[i] < minDist) {
                minDist = dist[i];
                u = i;
            }
        }

        if (u == -1) break;
        visited[u] = true;

        for (int v = 0; v < MAX_V; v++) {
            if (graph[u][v] != 0 && !visited[v] &&
                dist[u] != INT_MAX &&
                dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    cout << "\nКратчайшие расстояния от вершины " << startChar << ":\n";
    cout << setw(10) << "Вершина" << " | " << setw(10) << "Расстояние" << "\n";

    for (int i = 0; i < MAX_V; i++) {
        cout << setw(10) << indexToChar(i) << " | ";
        if (dist[i] == INT_MAX)
            cout << setw(10) << "Недостижима";
        else
            cout << setw(10) << dist[i];
        cout << "\n";
    }

    return 0;
}
