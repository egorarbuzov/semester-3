#include "minmax.h"

int __cdecl getmin(const int* pArr, int n)
{
    int m = pArr[0];
    for (int i = 1; i < n; i++)
        if (pArr[i] < m) m = pArr[i];
    return m;
}

int __cdecl getmax(const int* pArr, int n)
{
    int m = pArr[0];
    for (int i = 1; i < n; i++)
        if (pArr[i] > m) m = pArr[i];
    return m;
}
