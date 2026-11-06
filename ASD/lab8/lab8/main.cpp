#include <iostream>

using namespace std;

const int MAX_ITEMS = 100;
const int MAX_CAPACITY = 10000;

struct Item {
    char name[64];
    int weight;
    int cost;
};

int main() {
    int N;
    int n;

    cout << "Введите вместимость рюкзака N: ";
    cin >> N;
    cout << "Введите количество предметов: ";
    cin >> n;

    if (n > MAX_ITEMS || N > MAX_CAPACITY || n <= 0 || N < 0) {
        cout << "Некорректные входные данные." << endl;
        return 1;
    }

    Item items[MAX_ITEMS];

    for (int i = 0; i < n; i++) {
        cout << "Предмет #" << i + 1 << ":" << endl;
        cout << "  Название: ";
        cin >> items[i].name;
        cout << "  Вес: ";
        cin >> items[i].weight;
        cout << "  Стоимость: ";
        cin >> items[i].cost;
    }
    static int dp[MAX_ITEMS + 1][MAX_CAPACITY + 1];

    for (int i = 0; i <= n; i++)
        for (int w = 0; w <= N; w++)
            dp[i][w] = 0;

    for (int i = 1; i <= n; i++) {
        int wi = items[i - 1].weight;
        int ci = items[i - 1].cost;
        for (int w = 0; w <= N; w++) {
            dp[i][w] = dp[i - 1][w];
            if (wi <= w) {
                int take = dp[i - 1][w - wi] + ci;
                if (take > dp[i][w])
                    dp[i][w] = take;
            }
        }
    }

    bool taken[MAX_ITEMS];
    for (int i = 0; i < n; i++) taken[i] = false;

    int w = N;
    for (int i = n; i > 0; i--) {
        if (dp[i][w] != dp[i - 1][w]) {
            taken[i - 1] = true;
            w -= items[i - 1].weight;
        }
    }

    cout << "Максимальная стоимость: " << dp[n][N] << endl;
    cout << "Выбранные предметы:" << endl;

    bool any = false;
    int totalWeight = 0;
    for (int i = 0; i < n; i++) {
        if (taken[i]) {
            cout << "  - " << items[i].name
                 << " (вес: " << items[i].weight
                 << ", стоимость: " << items[i].cost << ")" << endl;
            totalWeight += items[i].weight;
            any = true;
        }
    }
    if (!any) cout << "  (ни один предмет не поместился)" << endl;
    cout << "Итоговый вес: " << totalWeight << " / " << N << endl;

    return 0;
}
