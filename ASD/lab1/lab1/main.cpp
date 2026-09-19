#include <iostream>
#include <cmath>
#include <clocale>

using namespace std;

int findFree(int from, int to, int blocked, int k) {
    for (int r = 1; r <= k; ++r) {
        if (r != from && r != to && r != blocked) return r;
    }
    return -1;
}

void hanoi(int n, int from, int to, int k, int blocked) {
    if (n == 0) return;

    if (n == 1) {
        cout << "Диск 1: " << from << " -> " << to << "\n";
        return;
    }

    int available = k - (blocked != 0 ? 1 : 0);
 
    if (available == 3) {
        int aux = findFree(from, to, blocked, k);
        hanoi(n - 1, from, aux, k, blocked);
        cout << "Диск " << n << ": " << from << " -> " << to << "\n";
        hanoi(n - 1, aux, to, k, blocked);
        return;
    }

    int r = n - (int)round(sqrt(2.0 * n - 1)) + 1;
    if (r < 1) r = 1;
    if (r >= n) r = n - 1;

    int aux = findFree(from, to, blocked, k);

    hanoi(r, from, aux, k, blocked);

    hanoi(n - r, from, to, k, aux);

    hanoi(r, aux, to, k, blocked);
}

int main() {
    setlocale(LC_ALL, "Russian");

    int n, k;

    cout << "Дисков: ";
    while (!(cin >> n) || n <= 0) {
        cout << "n > 0, повторите: ";
        cin.clear(); cin.ignore(10000, '\n');
    }

    cout << "Стержней (>= 3): ";
    while (!(cin >> k) || k < 3) {
        cout << "k >= 3, повторите: ";
        cin.clear(); cin.ignore(10000, '\n');
    }

    cout << "\n--- Ходы ---\n";
    hanoi(n, 1, k, k, 0);

    return 0;
}
