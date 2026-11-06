#include <iostream>

extern "C" int getmin(const int* pArr, int n);
extern "C" int getmax(const int* pArr, int n);

int main()
{
    int arr[10] = { 15, -3, 27, -8, 42, 0, -19, 33, 7, -25 };
    int sum = getmin(arr, 10) + getmax(arr, 10);

    std::cout << "getmax + getmin = " << sum << std::endl;
    return 0;
}
