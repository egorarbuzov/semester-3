#include <iostream>

using namespace std;

int main() {
    int n;
    cout << "Введите N: ";
    cin >> n;

    if (n <= 0) {
        cout << "Некорректное N" << endl;
        return 0;
    }

    // Выделяем массивы динамически (vector запрещён условием)
    int* a    = new int[n];   // исходная последовательность
    int* dp   = new int[n];   // длины подпоследовательностей
    int* prev = new int[n];   // предки для восстановления

    cout << "Введите " << n << " элементов: ";
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    // ---- Динамическое программирование ----
    for (int i = 0; i < n; ++i) {
        dp[i]   = 1;    // минимум — сам элемент
        prev[i] = -1;   // нет предыдущего

        for (int j = 0; j < i; ++j) {
            if (a[j] < a[i] && dp[j] + 1 > dp[i]) {
                dp[i]   = dp[j] + 1;
                prev[i] = j;
            }
        }
    }

    // ---- Ищем конец лучшей подпоследовательности ----
    int bestEnd = 0;
    for (int i = 1; i < n; ++i) {
        if (dp[i] > dp[bestEnd]) {
            bestEnd = i;
        }
    }

    cout << "Длина максимальной возрастающей подпоследовательности: "
         << dp[bestEnd] << endl;

    // ---- Восстанавливаем подпоследовательность ----
    // Идём от конца назад по prev, складываем индексы в массив,
    // затем выводим в обратном порядке.
    int* seq = new int[dp[bestEnd]];
    int len = 0;
    for (int k = bestEnd; k != -1; k = prev[k]) {
        seq[len++] = a[k];
    }

    cout << "Подпоследовательность: ";
    for (int i = len - 1; i >= 0; --i) {
        cout << seq[i];
        if (i > 0) cout << ", ";
    }
    cout << endl;

    // ---- Освобождаем память ----
    delete[] a;
    delete[] dp;
    delete[] prev;
    delete[] seq;

    return 0;
}
