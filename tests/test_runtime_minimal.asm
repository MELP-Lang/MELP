extern printf
extern malloc
extern free
extern strlen
extern strcmp
extern strstr
extern tyd_substr
extern tyd_strcat
extern dosya_ac
extern dosya_oku
extern dosya_yaz
extern dosya_kapat
extern string_karakter_al
extern string_alt
extern karakter_kodu
extern kodu_karaktere
extern runtime_dizin_al
extern runtime_dizin_al
extern tyd_fix_cwd
extern promise_create
extern promise_resolve
extern promise_reject
extern promise_then
extern promise_is_resolved
extern promise_get_value
extern promise_free
extern event_loop_create
extern event_loop_push_task
extern event_loop_pop_task
extern event_loop_run
extern event_loop_stop
extern async_sleep
extern async_read_file
extern async_write_file
extern async_http_get
extern promise_all
extern promise_all_simple
extern string_birlestir
extern string_karsilastir
extern string_uzunluk
extern string_esit_mi
extern string_karakter_al
extern string_alt
extern string_length
extern string_substring
extern string_concat
extern string_char_at
extern char_code
extern code_to_char
extern read_input
extern read_line
extern read_int
extern num
extern str
extern mlp_get_type
extern gui_window_create
extern gui_window_show
extern gui_window_hide
extern gui_window_destroy
extern gui_button_create
extern gui_label_create
extern gui_label_set_text
extern gui_poll_event
extern gui_get_clicked_button
extern gui_canvas_create
extern gui_canvas_clear
extern gui_canvas_set_color
extern gui_canvas_draw_rect
extern gui_canvas_draw_circle
extern gui_canvas_render
extern int_to_string
extern string_to_int
extern mlp_set_args
extern mlp_get_argc
extern mlp_get_argv
extern mlp_get_args_list
extern exit_with_code
extern panic
extern mlp_assert
extern get_error_code
extern set_error_code
extern mlp_malloc
extern mlp_free
extern mlp_realloc
extern mlp_calloc
extern get_allocated_bytes
extern check_memory_leaks
extern get_env
extern current_timestamp
extern sleep_ms
extern read_binary
extern write_binary
extern get_file_info
extern copy_file
extern list_directory
extern create_directory
extern remove_directory
extern directory_exists
extern get_current_dir
extern change_directory
extern execute_command
extern get_command_output
extern get_process_id
extern get_parent_process_id
extern format_timestamp
extern parse_timestamp
extern get_milliseconds
extern get_time_string
extern join_path
extern get_file_extension
extern get_file_name
extern get_directory
extern mlp_list_create
extern mlp_list_add
extern mlp_list_get
extern mlp_list_set
extern mlp_list_remove
extern mlp_list_insert
extern mlp_list_clear
extern mlp_list_length
extern mlp_list_free
extern hashmap_create
extern hashmap_put
extern hashmap_get
extern hashmap_remove
extern hashmap_contains
extern hashmap_size
extern hashmap_clear
extern hashmap_destroy
section .data
    format_sayi db "%ld", 10, 0
    format_metin db "%s", 10, 0
    global_event_loop_ptr: dq 0
    global_async_state_ptr: dq 0  ; PHASE 8.7: Current AsyncState for continuations
str_0: db "=== Test: List Create ===", 0
str_1: db "Created list: ", 0
str_2: db 226,156,147,32,76,105,115,116,32,116,101,115,116,32,112,97,115,115,101,100,33,0
str_3: db "", 0
str_4: db "=== Test: String Operations ===", 0
str_5: db "Hello", 0
str_6: db " World", 0
str_7: db "Concatenated: ", 0
str_8: db "Length: ", 0
str_9: db 226,156,147,32,83,116,114,105,110,103,32,116,101,115,116,32,112,97,115,115,101,100,33,0
str_10: db "", 0
str_11: db "=== Test: Math Operations ===", 0
str_12: db "min(42, 17) = ", 0
str_13: db "max(42, 17) = ", 0
str_14: db 226,156,147,32,77,97,116,104,32,116,101,115,116,32,112,97,115,115,101,100,33,0
str_15: db "", 0
str_16: db "=== Test: File I/O ===", 0
str_17: db "/tmp/mlp_runtime_test.txt", 0
str_18: db "Hello from MLP!", 0
str_19: db "Writing to file...", 0
str_20: db "Write result: ", 0
str_21: db "Reading from file...", 0
str_22: db "Content: ", 0
str_23: db 226,156,147,32,70,105,108,101,32,73,47,79,32,116,101,115,116,32,112,97,115,115,101,100,33,0
str_24: db "", 0
str_25: db "", 0
str_26: db "###############################################", 0
str_27: db "# MLP Runtime Bindings - Minimal Test", 0
str_28: db "###############################################", 0
str_29: db "", 0
str_30: db "###############################################", 0
str_31: db "# All Tests Complete!", 0
str_32: db "###############################################", 0
str_33: db "", 0

section .text
global main
main:
    push rbp
    mov rbp, rsp
    sub rsp, 256
    call mlp_set_args
    call tyd_fix_cwd
    ; --- Ana Program Akışı ---
    call mlp_main
    ; --- Program Sonu ---
    xor rax, rax
    mov rsp, rbp
    pop rbp
    ret

; === Struct Tanımları ===

; === Fonksiyon Tanımları ===
test_list:
    ; --- Islec Tanimlama: test_list ---
    push rbp
    mov rbp, rsp
    sub rsp, 256
    mov rax, str_0
    push rax
    pop rdi
    call mlp_println_string
    ; --- DegiskenTanimlama: list ---
    mov rax, 8
    push rax
    pop rdi
    call melp_list_create
    mov [rbp-8], rax
    mov rax, str_1
    push rax
    pop rdi
    call mlp_print_string
    mov rax, [rbp-8]
    push rax
    mov rax, 1
    push rax
    pop rsi
    pop rdi
    call mlp_println_numeric
    mov rax, [rbp-8]
    push rax
    pop rdi
    call melp_list_free
    mov rax, str_2
    push rax
    pop rdi
    call mlp_println_string
    mov rax, str_3
    push rax
    pop rdi
    call mlp_println_string
    ; --- Implicit Return (Function Epilog) ---
    mov rsp, rbp
    pop rbp
    ret
test_string:
    ; --- Islec Tanimlama: test_string ---
    push rbp
    mov rbp, rsp
    sub rsp, 256
    mov rax, str_4
    push rax
    pop rdi
    call mlp_println_string
    ; --- DegiskenTanimlama: str1 ---
    mov rax, str_5
    mov [rbp-8], rax
    ; --- DegiskenTanimlama: str2 ---
    mov rax, str_6
    mov [rbp-16], rax
    ; --- DegiskenTanimlama: result ---
    mov rax, [rbp-8]
    push rax
    mov rax, [rbp-16]
    push rax
    pop rsi
    pop rdi
    call mlp_string_concat
    mov [rbp-24], rax
    mov rax, str_7
    push rax
    pop rdi
    call mlp_print_string
    mov rax, [rbp-24]
    push rax
    pop rdi
    call mlp_println_string
    ; --- DegiskenTanimlama: len ---
    mov rax, [rbp-24]
    push rax
    pop rdi
    call mlp_string_length
    mov [rbp-32], rax
    mov rax, str_8
    push rax
    pop rdi
    call mlp_print_string
    mov rax, [rbp-32]
    push rax
    mov rax, 1
    push rax
    pop rsi
    pop rdi
    call mlp_println_numeric
    mov rax, str_9
    push rax
    pop rdi
    call mlp_println_string
    mov rax, str_10
    push rax
    pop rdi
    call mlp_println_string
    ; --- Implicit Return (Function Epilog) ---
    mov rsp, rbp
    pop rbp
    ret
test_math:
    ; --- Islec Tanimlama: test_math ---
    push rbp
    mov rbp, rsp
    sub rsp, 256
    mov rax, str_11
    push rax
    pop rdi
    call mlp_println_string
    ; --- DegiskenTanimlama: a ---
    mov rax, 42
    mov [rbp-8], rax
    ; --- DegiskenTanimlama: b ---
    mov rax, 17
    mov [rbp-16], rax
    ; --- DegiskenTanimlama: min_val ---
    mov rax, [rbp-8]
    push rax
    mov rax, [rbp-16]
    push rax
    pop rsi
    pop rdi
    call min
    mov [rbp-24], rax
    ; --- DegiskenTanimlama: max_val ---
    mov rax, [rbp-8]
    push rax
    mov rax, [rbp-16]
    push rax
    pop rsi
    pop rdi
    call max
    mov [rbp-32], rax
    mov rax, str_12
    push rax
    pop rdi
    call mlp_print_string
    mov rax, [rbp-24]
    push rax
    mov rax, 1
    push rax
    pop rsi
    pop rdi
    call mlp_println_numeric
    mov rax, str_13
    push rax
    pop rdi
    call mlp_print_string
    mov rax, [rbp-32]
    push rax
    mov rax, 1
    push rax
    pop rsi
    pop rdi
    call mlp_println_numeric
    mov rax, str_14
    push rax
    pop rdi
    call mlp_println_string
    mov rax, str_15
    push rax
    pop rdi
    call mlp_println_string
    ; --- Implicit Return (Function Epilog) ---
    mov rsp, rbp
    pop rbp
    ret
test_file_io:
    ; --- Islec Tanimlama: test_file_io ---
    push rbp
    mov rbp, rsp
    sub rsp, 256
    mov rax, str_16
    push rax
    pop rdi
    call mlp_println_string
    ; --- DegiskenTanimlama: test_file ---
    mov rax, str_17
    mov [rbp-8], rax
    ; --- DegiskenTanimlama: content ---
    mov rax, str_18
    mov [rbp-16], rax
    mov rax, str_19
    push rax
    pop rdi
    call mlp_println_string
    ; --- DegiskenTanimlama: write_result ---
    mov rax, [rbp-8]
    push rax
    mov rax, [rbp-16]
    push rax
    pop rsi
    pop rdi
    call mlp_write_file
    mov [rbp-24], rax
    mov rax, str_20
    push rax
    pop rdi
    call mlp_print_string
    mov rax, [rbp-24]
    push rax
    mov rax, 1
    push rax
    pop rsi
    pop rdi
    call mlp_println_numeric
    mov rax, str_21
    push rax
    pop rdi
    call mlp_println_string
    ; --- DegiskenTanimlama: read_content ---
    mov rax, [rbp-8]
    push rax
    pop rdi
    call mlp_read_file
    mov [rbp-32], rax
    mov rax, str_22
    push rax
    pop rdi
    call mlp_print_string
    mov rax, [rbp-32]
    push rax
    pop rdi
    call mlp_println_string
    mov rax, str_23
    push rax
    pop rdi
    call mlp_println_string
    mov rax, str_24
    push rax
    pop rdi
    call mlp_println_string
    ; --- Implicit Return (Function Epilog) ---
    mov rsp, rbp
    pop rbp
    ret
mlp_main:
    ; --- Islec Tanimlama: main ---
    push rbp
    mov rbp, rsp
    sub rsp, 256
    mov rax, str_25
    push rax
    pop rdi
    call mlp_println_string
    mov rax, str_26
    push rax
    pop rdi
    call mlp_println_string
    mov rax, str_27
    push rax
    pop rdi
    call mlp_println_string
    mov rax, str_28
    push rax
    pop rdi
    call mlp_println_string
    mov rax, str_29
    push rax
    pop rdi
    call mlp_println_string
    call test_list
    call test_string
    call test_math
    call test_file_io
    mov rax, str_30
    push rax
    pop rdi
    call mlp_println_string
    mov rax, str_31
    push rax
    pop rdi
    call mlp_println_string
    mov rax, str_32
    push rax
    pop rdi
    call mlp_println_string
    mov rax, str_33
    push rax
    pop rdi
    call mlp_println_string
    ; --- Implicit Return (Function Epilog) ---
    mov rsp, rbp
    pop rbp
    ret

section .note.GNU-stack noalloc noexec nowrite progbits

db 0
