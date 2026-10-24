.586P
.MODEL FLAT, STDCALL
includelib kernel32.lib
includelib user32.lib

ExitProcess PROTO : DWORD
MessageBoxA PROTO : DWORD, : DWORD, : DWORD, : DWORD

.STACK 4096

.DATA
MB_OK       EQU 0
A           DB  2
B           DB  7
SUM         DB  ?
CAPTION     DB "Результат", 0
PREFIX      DB "Результат сложения = ", 0
RESULT_STR  DB "Результат сложения = 0", 0  
DIGIT_POS   EQU 21                 

.CODE

main PROC
START :
    MOV     AL, A
    ADD     AL, B
    MOV     SUM, AL

    ADD     AL, '0'             
    MOV     RESULT_STR[DIGIT_POS], AL  

    PUSH    MB_OK
    PUSH    OFFSET CAPTION
    PUSH    OFFSET RESULT_STR
    PUSH    0
    CALL    MessageBoxA

    push - 1
    call ExitProcess
main ENDP

end main