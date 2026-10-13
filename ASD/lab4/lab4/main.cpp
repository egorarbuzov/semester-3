#include <iostream>
#include <iomanip>

using namespace std;

const int V = 6;
const int INF = 1e9;

void floydWarshall(int graph[V][V]) {
    int dist[V][V];
    int S[V][V];

    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            dist[i][j] = graph[i][j];
            if (i == j) S[i][j] = 0;
            else S[i][j] = j + 1;
        }
    }

    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF && dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    S[i][j] = k + 1;
                }
            }
        }
    }

    cout << "Матрица кратчайших расстояний D:" << endl;
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (dist[i][j] == INF) cout << "INF\t";
            else cout << dist[i][j] << "\t";
        }
        cout << endl;
    }

    cout << "\nМатрица последовательности S:" << endl;
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            cout << S[i][j] << "\t";
        }
        cout << endl;
    }
}

int main() {
    int graph[V][V] = {
        {  0, 28, 21, 59, 12, 27 },
        {  7,  0, 24, INF, 21,  9 },
        {  9, 32,  0, 13, 11, INF },
        {  8, INF,  5,  0, 16, INF },
        { 14, 13, 15, INF,  0, 22 },
        { 15, 18, INF, INF,  6,  0 }
    };

    floydWarshall(graph);

    return 0;
}
