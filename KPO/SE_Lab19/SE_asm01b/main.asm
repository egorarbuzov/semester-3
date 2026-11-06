.586
.model flat, C

GetStdHandle   PROTO STDCALL :DWORD
WriteConsoleA  PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ExitProcess    PROTO STDCALL :DWORD

getmin         PROTO C :DWORD, :DWORD
getmax         PROTO C :DWORD, :DWORD

.data
arr     DWORD 15, -3, 27, -8, 42, 0, -19, 33, 7, -25
mn      DWORD 0
mx      DWORD 0
sum     DWORD 0
buf     DB 16 DUP(0)
msg     DB 'getmax + getmin = '
msgLen  EQU $ - msg
crlf    DB 13, 10

.code

; IntToStr(val, pBuf) -> EAX = длина строки (Windows-1251)
IntToStr PROC STDCALL USES ebx edi, val:DWORD, pBuf:DWORD
    mov  edi, pBuf
    mov  eax, val
    test eax, eax
    jns  short conv
    mov  byte ptr [edi], '-'
    inc  edi
    neg  eax
conv:
    mov  ebx, 10
    xor  ecx, ecx
digits:
    xor  edx, edx
    div  ebx
    push edx
    inc  ecx
    test eax, eax
    jnz  short digits
store:
    pop  eax
    add  al, '0'
    mov  [edi], al
    inc  edi
    loop store
    mov  byte ptr [edi], 0
    mov  eax, edi
    sub  eax, pBuf
    ret
IntToStr ENDP

; print(pStr, len)
print PROC STDCALL USES esi, pStr:DWORD, len:DWORD
    LOCAL wr:DWORD
    invoke GetStdHandle, -11
    invoke WriteConsoleA, eax, pStr, len, ADDR wr, 0
    ret
print ENDP

PUBLIC main
main PROC C
    invoke getmin, OFFSET arr, 10
    mov  mn, eax
    invoke getmax, OFFSET arr, 10
    mov  mx, eax
    add  eax, mn
    mov  sum, eax

    invoke print, OFFSET msg, msgLen
    invoke IntToStr, sum, OFFSET buf
    mov  ebx, eax
    invoke print, OFFSET buf, ebx
    invoke print, OFFSET crlf, 2
    invoke ExitProcess, 0
main ENDP

END main
