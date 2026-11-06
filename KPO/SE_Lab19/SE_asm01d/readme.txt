SE_asm01d
C++ статическая библиотека.

int __cdecl getmin(const int *pArr, int n);
int __cdecl getmax(const int *pArr, int n);

extern "C" + cdecl, поэтому символы _getmin и _getmax вызываются
из ассемблера (getmin PROTO C :DWORD, :DWORD).
