#include "helpers.h"
#include <stdio.h>

// Generate print_number helper function in assembly
void generate_print_number_helper(FILE* out) {
    fprintf(out, "    ; Helper function: print_number\n");
    fprintf(out, "    ; Input: rax = number to print\n");
    fprintf(out, "print_number:\n");
    fprintf(out, "    push rbp\n");
    fprintf(out, "    mov rbp, rsp\n");
    fprintf(out, "    push rbx\n");
    fprintf(out, "    push r12\n");
    fprintf(out, "    sub rsp, 32\n");
    fprintf(out, "    mov rbx, 10\n");
    fprintf(out, "    xor r12, r12\n");
    fprintf(out, "    test rax, rax\n");
    fprintf(out, "    jns .positive\n");
    fprintf(out, "    neg rax\n");
    fprintf(out, "    push rax\n");
    fprintf(out, "    mov rax, 1\n");
    fprintf(out, "    mov rdi, 1\n");
    fprintf(out, "    lea rsi, [_print_minus]\n");
    fprintf(out, "    mov rdx, 1\n");
    fprintf(out, "    syscall\n");
    fprintf(out, "    pop rax\n");
    fprintf(out, ".positive:\n");
    fprintf(out, ".convert_loop:\n");
    fprintf(out, "    xor rdx, rdx\n");
    fprintf(out, "    div rbx\n");
    fprintf(out, "    add rdx, '0'\n");
    fprintf(out, "    mov [rbp - 48 + r12], dl\n");
    fprintf(out, "    inc r12\n");
    fprintf(out, "    test rax, rax\n");
    fprintf(out, "    jnz .convert_loop\n");
    fprintf(out, "    mov rax, 1\n");
    fprintf(out, "    mov rdi, 1\n");
    fprintf(out, "    lea rsi, [rbp - 48]\n");
    fprintf(out, "    mov r8, 0\n");
    fprintf(out, "    mov r9, r12\n");
    fprintf(out, "    dec r9\n");
    fprintf(out, ".reverse_loop:\n");
    fprintf(out, "    cmp r8, r9\n");
    fprintf(out, "    jge .reverse_done\n");
    fprintf(out, "    movzx r10, byte [rbp - 48 + r8]\n");
    fprintf(out, "    movzx r11, byte [rbp - 48 + r9]\n");
    fprintf(out, "    mov [rbp - 48 + r8], r11b\n");
    fprintf(out, "    mov [rbp - 48 + r9], r10b\n");
    fprintf(out, "    inc r8\n");
    fprintf(out, "    dec r9\n");
    fprintf(out, "    jmp .reverse_loop\n");
    fprintf(out, ".reverse_done:\n");
    fprintf(out, "    mov rdx, r12\n");
    fprintf(out, "    syscall\n");
    fprintf(out, "    mov rax, 1\n");
    fprintf(out, "    mov rdi, 1\n");
    fprintf(out, "    lea rsi, [_print_newline]\n");
    fprintf(out, "    mov rdx, 1\n");
    fprintf(out, "    syscall\n");
    fprintf(out, "    add rsp, 32\n");
    fprintf(out, "    pop r12\n");
    fprintf(out, "    pop rbx\n");
    fprintf(out, "    pop rbp\n");
    fprintf(out, "    ret\n\n");
}
