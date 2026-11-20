.586
.model flat, C

PUBLIC getmin
PUBLIC getmax

.code

getmin PROC C USES esi ecx, pArr:DWORD, n:DWORD
    mov  esi, pArr
    mov  ecx, n
    mov  eax, [esi]
    dec  ecx
    jz   short gm_done
gm_loop:
    add  esi, 4
    mov  edx, [esi]
    cmp  edx, eax
    jge  short gm_next
    mov  eax, edx
gm_next:
    loop gm_loop
gm_done:
    ret
getmin ENDP

getmax PROC C USES esi ecx, pArr:DWORD, n:DWORD
    mov  esi, pArr
    mov  ecx, n
    mov  eax, [esi]
    dec  ecx
    jz   short gx_done
gx_loop:
    add  esi, 4
    mov  edx, [esi]
    cmp  edx, eax
    jle  short gx_next
    mov  eax, edx
gx_next:
    loop gx_loop
gx_done:
    ret
getmax ENDP

END
