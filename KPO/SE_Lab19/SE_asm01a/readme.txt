SE_asm01a
MASM, x86, статическая библиотека.

int getmin(const int *pArr, int n);
int getmax(const int *pArr, int n);

Соглашение cdecl: в asm вызывать через getmin PROTO C :DWORD, :DWORD,
в C++ - extern "C". Результат в EAX.
