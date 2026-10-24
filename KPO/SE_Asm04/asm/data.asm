.586P
.MODEL FLAT, STDCALL
includelib kernel32.lib
ExitProcess PROTO : DWORD
.DATA
int1_type DB 01h
int1      DWORD 123
str1_type DB 02h
str1_len  DB 5
str1      DB 048h, 065h, 06Ch, 06Ch, 021h, 0
int2_type DB 01h
int2      DWORD 1
str2_type DB 02h
str2_len  DB 0
str2      DB 0
int3_type DB 01h
int3      DWORD -1
str3_type DB 02h
str3_len  DB 127
str3      DB 127 DUP(041h), 0
str4_type DB 02h
str4_len  DB 1
str4      DB 058h, 0

.CODE
main PROC
    MOV EAX, int1
    MOV EAX, int2
    MOV EAX, int3
    LEA ESI, str1
    LEA ESI, str2
    LEA ESI, str3
    LEA ESI, str4
    PUSH 0
    CALL ExitProcess
main ENDP
end main
