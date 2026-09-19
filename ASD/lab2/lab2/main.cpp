#include <iostream>
using namespace std;

const int N = 11;
const int M = 11;

struct Edge { int u, v; };
Edge edgeList[M] = {
    {1,2}, {1,5}, {2,7}, {2,8}, {3,8},
    {4,6}, {5,6}, {7,8}, {4,9}, {6,9}, {9,10}
};

int adjMatrix[N][N];

void buildMatrix() {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            adjMatrix[i][j] = 0;

    for (int i = 0; i < M; ++i) {
        int u = edgeList[i].u;
        int v = edgeList[i].v;
        adjMatrix[u][v] = 1;
        adjMatrix[v][u] = 1;
    }
}

int adjList[N][N];
int deg[N];

void buildAdjList() {
    for (int i = 0; i < N; ++i) deg[i] = 0;

    for (int i = 0; i < M; ++i) {
        int u = edgeList[i].u;
        int v = edgeList[i].v;
        adjList[u][deg[u]++] = v;
        adjList[v][deg[v]++] = u;
    }

    for (int v = 1; v < N; ++v) {
        for (int i = 0; i < deg[v] - 1; ++i)
            for (int j = 0; j < deg[v] - 1 - i; ++j)
                if (adjList[v][j] > adjList[v][j + 1]) {
                    int t = adjList[v][j];
                    adjList[v][j] = adjList[v][j + 1];
                    adjList[v][j + 1] = t;
                }
    }
}

void bfs(int start) {
    bool visited[N] = {false};
    int queue[N];
    int head = 0, tail = 0;

    visited[start] = true;
    queue[tail++] = start;

    cout << "BFS: ";
    while (head < tail) {
        int v = queue[head++];
        cout << v << " ";
        for (int k = 0; k < deg[v]; ++k) {
            int nb = adjList[v][k];
            if (!visited[nb]) {
                visited[nb] = true;
                queue[tail++] = nb;
            }
        }
    }
    cout << "\n";
}

void dfs(int v, bool visited[]) {
    visited[v] = true;
    cout << v << " ";
    for (int k = 0; k < deg[v]; ++k) {
        int nb = adjList[v][k];
        if (!visited[nb]) dfs(nb, visited);
    }
}

void printEdgeList() {
    cout << "=== Список рёбер ===\n";
    for (int i = 0; i < M; ++i)
        cout << "(" << edgeList[i].u << ", " << edgeList[i].v << ") ";
    cout << "\n\n";
}

void printMatrix() {
    cout << "=== Матрица смежности ===\n    ";
    for (int j = 1; j < N; ++j) cout << j << " ";
    cout << "\n";
    for (int i = 1; i < N; ++i) {
        cout << i << ":  ";
        for (int j = 1; j < N; ++j)
            cout << adjMatrix[i][j] << " ";
        cout << "\n";
    }
    cout << "\n";
}

void printAdjList() {
    cout << "=== Список смежности ===\n";
    for (int v = 1; v < N; ++v) {
        cout << v << ": ";
        for (int k = 0; k < deg[v]; ++k)
            cout << adjList[v][k] << " ";
        cout << "\n";
    }
    cout << "\n";
}

int main() {
    buildMatrix();
    buildAdjList();

    printEdgeList();
    printMatrix();
    printAdjList();

    int start = 1;

    bfs(start);

    bool visited[N] = {false};
    cout << "DFS: ";
    dfs(start, visited);
    cout << "\n";

    return 0;
}
