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

    int* a = new int[n];
    int* dp = new int[n];
    int* prev = new int[n];

    cout << "Введите " << n << " элементов: ";
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    for (int i = 0; i < n; ++i) {
        dp[i]   = 1;
        prev[i] = -1;

        for (int j = 0; j < i; ++j) {
            if (a[j] < a[i] && dp[j] + 1 > dp[i]) {
                dp[i] = dp[j] + 1;
                prev[i] = j;
            }
        }
    }

    int bestEnd = 0;
    for (int i = 1; i < n; ++i) {
        if (dp[i] > dp[bestEnd]) {
            bestEnd = i;
        }
    }

    cout << "Длина максимальной возрастающей подпоследовательности: "
         << dp[bestEnd] << endl;

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

    
    delete[] a;
    delete[] dp;
    delete[] prev;
    delete[] seq;

    return 0;
}
