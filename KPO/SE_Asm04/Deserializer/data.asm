.586P
.MODEL FLAT, STDCALL
includelib kernel32.lib
ExitProcess PROTO : DWORD
.DATA
int1 DWORD 2026
str1 DB 048h, 065h, 06Ch, 06Ch, 06Fh, 02Ch, 020h, 061h, 073h, 073h, 065h, 06Dh, 062h, 06Ch, 065h, 072h, 0
int2 DWORD -1
str2 DB 0

.CODE
main PROC
    MOV EAX, int1
    MOV EAX, int2
    LEA ESI, str1
    LEA ESI, str2
    PUSH 0
    CALL ExitProcess
main ENDP
end main
