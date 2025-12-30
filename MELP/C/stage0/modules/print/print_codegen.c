#include "print_codegen.h"
#include "print.h"
#include "../codegen_emit/codegen_emit.h"
#include <stdio.h>
#include <string.h>

static int string_counter = 0;
static int is_first_call = 1;

void codegen_print_statement(FILE* f, PrintStatement* stmt) {
    if (!stmt || !stmt->value) return;
    
    // Get string content (temporarily stored as Expression* pointer)
    const char* string_content = (const char*)stmt->value;
    
    // Initialize C code emission on first call
    if (is_first_call) {
        emit_c_init(f);
        emit_c("int main(void) {\n");
        is_first_call = 0;
    }
    
    // Generate C printf statement
    // MLP: print("hello")
    // C:   printf("hello\\n");
    emit_c_indent(1, "printf(\"%s\\n\");\n", string_content);
    
    string_counter++;
}

void codegen_print_finalize(FILE* f) {
    // Close main function and return
    emit_c_indent(1, "return 0;\n");
    emit_c("}\n");
    emit_c_finalize();
}


