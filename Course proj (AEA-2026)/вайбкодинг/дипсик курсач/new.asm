; ============================================
; Сгенерировано компилятором IVA-2026
; Целевая платформа: Linux x64 (ELF64)
; ============================================

section .data
    v_a_0: dq 0
    str_0: db "Hello world!", 0
    newline: db 10, 0
    numbuf:  times 32 db 0
    concat_buf: times 1024 db 0

section .text
global _start

; --- print_str(rdi=ptr) ---
print_str:
    push rbp
    mov rbp, rsp
    push rdi
    call strLen
    mov rdx, rax
    pop rsi
    mov rax, 1
    mov rdi, 1
    syscall
    mov rsp, rbp
    pop rbp
    ret

; --- intToStr(rdi=num, rsi=buf, rdx=size) -> rax ---
intToStr:
    push rbp
    mov rbp, rsp
    push rbx
    mov rax, rdi
    mov r8, rsi
    mov rcx, rdx
    mov rbx, 10
    mov r9, r8
    add r9, rcx
    dec r9
    mov byte [r9], 0
    test rax, rax
    jnz .digit
    dec r9
    mov byte [r9], '0'
    jmp .done
.digit:
    xor rdx, rdx
    div rbx
    add dl, '0'
    dec r9
    mov [r9], dl
    test rax, rax
    jnz .digit
.done:
    mov rax, r9
    pop rbx
    mov rsp, rbp
    pop rbp
    ret

; --- strLen(rdi=ptr) -> rax ---
strLen:
    xor rax, rax
.lp:
    cmp byte [rdi+rax], 0
    je .done
    inc rax
    jmp .lp
.done:
    ret

; --- strcat2(rdi=dst, rsi=src) -> rax ---
strcat2:
    mov r8, rdi
.seek:
    cmp byte [rdi], 0
    je .copy
    inc rdi
    jmp .seek
.copy:
    mov al, [rsi]
    mov [rdi], al
    test al, al
    jz .done
    inc rdi
    inc rsi
    jmp .copy
.done:
    mov rax, r8
    ret

; --- strcopy(rdi=dst, rsi=src) -> rax ---
strcopy:
    mov rax, rdi
.cp:
    mov bl, [rsi]
    mov [rdi], bl
    test bl, bl
    jz .done
    inc rdi
    inc rsi
    jmp .cp
.done:
    ret

; ---------- точка входа ----------
_start:
    lea rax, [str_0]
    mov rdi, rax
    call print_str
    lea rdi, [newline]
    call print_str
    mov rax, 32
    mov [v_a_0], rax
    mov rax, [v_a_0]
    mov rdi, rax
    call print_str
    lea rdi, [newline]
    call print_str
    mov rax, 60
    xor rdi, rdi
    syscall

