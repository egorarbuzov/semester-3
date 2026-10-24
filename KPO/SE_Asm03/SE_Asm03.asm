.586P
.MODEL FLAT, STDCALL
includelib kernel32.lib
includelib user32.lib

ExitProcess PROTO : DWORD
MessageBoxA PROTO : DWORD, : DWORD, : DWORD, : DWORD

.STACK 4096

.DATA
myBytes     BYTE 10h, 20h, 30h, 40h
myWords     WORD 8Ah, 3Bh, 44h, 5Fh, 99h
myDoubles   DWORD 1, 2, 3, 4, 5, 6
myPointer   DWORD myDoubles

array       DWORD 4, 2, 7, 0, 5, 1, 3
ARRAY_SIZE  EQU 7

MB_OK       EQU 0
CAPTION     DB "Арбузов Егор Андреевич, 2 курс, 8 группа", 0
MSG_ZERO    DB "В массиве есть нулевой элемент", 0
MSG_NO_ZERO DB "В массиве нет нулевого элемента", 0

.CODE

main PROC
START :
    MOV     ESI, OFFSET myBytes
    MOV     AH, [ESI]
    MOV     AL, [ESI + 3]

    MOV     ECX, ARRAY_SIZE
    LEA     ESI, array
    XOR     EAX, EAX

sum_loop:
    ADD     EAX, [ESI]
    ADD     ESI, 4
    LOOP    sum_loop

    MOV     ECX, ARRAY_SIZE
    LEA     ESI, array
    MOV     EBX, 1

check_loop:
    CMP     DWORD PTR [ESI], 0
    JE      found_zero
    ADD     ESI, 4
    LOOP    check_loop
    JMP     check_done

found_zero:
    MOV     EBX, 0

check_done:
    CMP     EBX, 0
    JE      show_zero_msg

    PUSH    MB_OK
    PUSH    OFFSET CAPTION
    PUSH    OFFSET MSG_NO_ZERO
    PUSH    0
    CALL    MessageBoxA
    JMP     exit_program

show_zero_msg:
    PUSH    MB_OK
    PUSH    OFFSET CAPTION
    PUSH    OFFSET MSG_ZERO
    PUSH    0
    CALL    MessageBoxA

exit_program:
    push - 1
    call ExitProcess
main ENDP

end main