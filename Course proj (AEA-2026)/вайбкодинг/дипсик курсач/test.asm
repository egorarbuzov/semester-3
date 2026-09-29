; ============================================
; Сгенерировано компилятором IVA-2026
; Целевая платформа: Linux x64 (ELF64)
; ============================================

section .data
    v_a_0: dq 0
    v_b_1: dq 0
    v_c_6: dq 0
    v_d_7: dq 0
    v_e_8: dq 0
    v_flag_2: dq 0
    v_i_9: dq 0
    v_msg_3: dq 0
    v_x_4: dq 0
    v_y_5: dq 0
    str_0: db "result", 0
    str_1: db "a | b = ", 0
    str_2: db "a & b = ", 0
    str_3: db "~a    = ", 0
    str_4: db "iter", 0
    str_5: db "flag is true", 0
    str_6: db "flag is false", 0
    str_7: db "msg length:", 0
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

; ---------- func bitOr ----------
bitOr:
    push rbp
    mov rbp, rsp
    sub rsp, 40
    mov [v_x_4], rdi
    mov [v_y_5], rsi
    mov rax, [v_x_4]
    push rax
    mov rax, [v_y_5]
    mov rbx, rax
    pop rax
    or  rax, rbx
    and rax, 0xFF
    mov rsp, rbp
    pop rbp
    ret

; ---------- func bitAnd ----------
bitAnd:
    push rbp
    mov rbp, rsp
    sub rsp, 40
    mov [v_x_4], rdi
    mov [v_y_5], rsi
    mov rax, [v_x_4]
    push rax
    mov rax, [v_y_5]
    mov rbx, rax
    pop rax
    and rax, rbx
    and rax, 0xFF
    mov rsp, rbp
    pop rbp
    ret

; ---------- func bitNot ----------
bitNot:
    push rbp
    mov rbp, rsp
    sub rsp, 40
    mov [v_x_4], rdi
    mov rax, [v_x_4]
    not rax
    and rax, 0xFF
    mov rsp, rbp
    pop rbp
    ret

; ---------- точка входа ----------
_start:
    mov rax, 5
    mov [v_a_0], rax
    mov rax, 3
    mov [v_b_1], rax
    mov rax, 1
    mov [v_flag_2], rax
    lea rax, [str_0]
    mov [v_msg_3], rax
    mov rax, [v_a_0]
    push rax
    mov rax, [v_b_1]
    push rax
    pop rsi
    pop rdi
    call bitOr
    mov [v_c_6], rax
    lea rax, [str_1]
    mov rdi, rax
    call print_str
    lea rdi, [newline]
    call print_str
    mov rdi, [v_c_6]
    lea rsi, [numbuf]
    mov rdx, 32
    call intToStr
    mov rdi, rax
    call print_str
    lea rdi, [newline]
    call print_str
    mov rax, [v_a_0]
    push rax
    mov rax, [v_b_1]
    push rax
    pop rsi
    pop rdi
    call bitAnd
    mov [v_d_7], rax
    lea rax, [str_2]
    mov rdi, rax
    call print_str
    lea rdi, [newline]
    call print_str
    mov rdi, [v_d_7]
    lea rsi, [numbuf]
    mov rdx, 32
    call intToStr
    mov rdi, rax
    call print_str
    lea rdi, [newline]
    call print_str
    mov rax, [v_a_0]
    push rax
    pop rdi
    call bitNot
    mov [v_e_8], rax
    lea rax, [str_3]
    mov rdi, rax
    call print_str
    lea rdi, [newline]
    call print_str
    mov rdi, [v_e_8]
    lea rsi, [numbuf]
    mov rdx, 32
    call intToStr
    mov rdi, rax
    call print_str
    lea rdi, [newline]
    call print_str
    mov rax, 0
    mov [v_i_9], rax
.Ldo_10:
    lea rax, [str_4]
    mov rdi, rax
    call print_str
    lea rdi, [newline]
    call print_str
    mov rdi, [v_i_9]
    lea rsi, [numbuf]
    mov rdx, 32
    call intToStr
    mov rdi, rax
    call print_str
    lea rdi, [newline]
    call print_str
    mov rax, [v_i_9]
    push rax
    mov rax, 1
    mov rbx, rax
    pop rax
    add rax, rbx
    mov [v_i_9], rax
    mov rax, [v_i_9]
    push rax
    mov rax, 3
    mov rbx, rax
    pop rax
    cmp rax, rbx
    setl al
    movzx rax, al
    test rax, rax
    jnz .Ldo_10
    mov rax, [v_flag_2]
    test rax, rax
    jz .Lelse_11
    lea rax, [str_5]
    mov rdi, rax
    call print_str
    lea rdi, [newline]
    call print_str
    jmp .Lend_12
.Lelse_11:
    lea rax, [str_6]
    mov rdi, rax
    call print_str
    lea rdi, [newline]
    call print_str
.Lend_12:
    lea rax, [str_7]
    mov rdi, rax
    call print_str
    lea rdi, [newline]
    call print_str
    mov rdi, [v_msg_3]
    call strLen
    mov rdi, rax
    lea rsi, [numbuf]
    mov rdx, 32
    call intToStr
    mov rdi, rax
    call print_str
    lea rdi, [newline]
    call print_str
    mov rax, 60
    xor rdi, rdi
    syscall

