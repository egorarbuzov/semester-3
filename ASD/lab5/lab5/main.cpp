#include <iostream>
#include <climits>
#include <clocale>

using namespace std;

const int N = 8;
const int MAXN = 100;

struct Edge {
    int from;
    int to;
    int weight;
};

Edge sourceEdges[] = {
    {1, 2, 2},
    {2, 3, 3},
    {1, 4, 8},
    {4, 6, 3},
    {2, 5, 5},
    {5, 7, 4},
    {3, 5, 12},
    {5, 8, 8},
    {3, 8, 7},
    {6, 7, 6},
    {7, 8, 9},
    {1, 5, 10},
    {2, 4, 2},
    {4, 5, 14},
    {4, 7, 1},
    {6, 5, 11}
};

const int M = sizeof(sourceEdges) / sizeof(sourceEdges[0]);

int parentSet[MAXN];
int rankSet[MAXN];

void dsuInit(int n) {
    for (int i = 1; i <= n; i++) {
        parentSet[i] = i;
        rankSet[i] = 0;
    }
}

int dsuFind(int x) {
    if (parentSet[x] != x) {
        parentSet[x] = dsuFind(parentSet[x]);
    }
    return parentSet[x];
}

bool dsuUnite(int x, int y) {
    int rx = dsuFind(x);
    int ry = dsuFind(y);

    if (rx == ry) {
        return false;
    }

    if (rankSet[rx] < rankSet[ry]) {
        int tmp = rx;
        rx = ry;
        ry = tmp;
    }
    parentSet[ry] = rx;
    if (rankSet[rx] == rankSet[ry]) {
        rankSet[rx]++;
    }
    return true;
}

void sortEdges(Edge edges[], int m) {
    for (int i = 0; i < m - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < m; j++) {
            if (edges[j].weight < edges[minIndex].weight) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            Edge tmp = edges[i];
            edges[i] = edges[minIndex];
            edges[minIndex] = tmp;
        }
    }
}

void printMST(Edge mst[], int count) {
    int total = 0;
    for (int i = 0; i < count; i++) {
        cout << "v" << mst[i].from << " -- v" << mst[i].to
             << "  вес: " << mst[i].weight << endl;
        total += mst[i].weight;
    }
    cout << "суммарный вес остовного дерева: " << total << endl;
}

int primMST(int graph[MAXN][MAXN], int n, Edge result[]) {
    bool inMST[MAXN];
    int minEdge[MAXN];
    int treeParent[MAXN];

    for (int i = 1; i <= n; i++) {
        inMST[i] = false;
        minEdge[i] = INT_MAX;
        treeParent[i] = -1;
    }

    minEdge[1] = 0;

    int edgeCount = 0;
    for (int count = 1; count <= n; count++) {
        int u = -1;
        for (int v = 1; v <= n; v++) {
            if (!inMST[v] && (u == -1 || minEdge[v] < minEdge[u])) {
                u = v;
            }
        }

        if (u == -1 || minEdge[u] == INT_MAX) {
            continue;
        }

        inMST[u] = true;

        if (treeParent[u] != -1) {
            result[edgeCount].from = treeParent[u];
            result[edgeCount].to = u;
            result[edgeCount].weight = minEdge[u];
            edgeCount++;
        }

        for (int v = 1; v <= n; v++) {
            int w = graph[u][v];
            if (w != 0 && !inMST[v] && w < minEdge[v]) {
                minEdge[v] = w;
                treeParent[v] = u;
            }
        }
    }

    return edgeCount;
}

int kruskalMST(Edge edges[], int m, int n, Edge result[]) {
    static Edge sorted[MAXN * MAXN];

    for (int i = 0; i < m; i++) {
        sorted[i] = edges[i];
    }

    dsuInit(n);
    sortEdges(sorted, m);

    int edgeCount = 0;
    for (int i = 0; i < m; i++) {
        if (dsuUnite(sorted[i].from, sorted[i].to)) {
            result[edgeCount] = sorted[i];
            edgeCount++;
        }
        if (edgeCount == n - 1) {
            break;
        }
    }

    return edgeCount;
}

int main() {
    setlocale(LC_ALL, "ru");
    static int graph[MAXN][MAXN];
    static Edge primResult[MAXN];
    static Edge kruskalResult[MAXN];

    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            graph[i][j] = 0;
        }
    }

    for (int i = 0; i < M; i++) {
        int u = sourceEdges[i].from;
        int v = sourceEdges[i].to;
        int w = sourceEdges[i].weight;
        graph[u][v] = w;
        graph[v][u] = w;
    }

    cout << "граф: " << N << " вершин, " << M << " ребер" << endl;

    cout << endl << "минимальное остовное дерево, алгоритм Прима:" << endl;
    int primCount = primMST(graph, N, primResult);
    printMST(primResult, primCount);
    if (primCount < N - 1) {
        cout << "внимание: граф несвязный, дерево построено не для всех вершин" << endl;
    }

    cout << endl << "минимальное остовное дерево, алгоритм Краскала:" << endl;
    int kruskalCount = kruskalMST(sourceEdges, M, N, kruskalResult);
    printMST(kruskalResult, kruskalCount);
    if (kruskalCount < N - 1) {
        cout << "внимание: граф несвязный, дерево построено не для всех вершин" << endl;
    }

    return 0;
}
