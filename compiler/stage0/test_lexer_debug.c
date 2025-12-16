#include <stdio.h>
#include "modules/lexer/lexer.h"

int main() {
    const char* code = "if x > 0 then\n    print(5)\nend if\n";
    
    Lexer* lexer = lexer_create(code);
    Token* tok;
    
    printf("Lexer test:\n");
    while ((tok = lexer_next_token(lexer)) != NULL) {
        if (tok->type == TOKEN_EOF) {
            token_free(tok);
            break;
        }
        
        printf("  Token type=%d value='%s'\n", tok->type, tok->value ? tok->value : "(null)");
        token_free(tok);
    }
    
    lexer_free(lexer);
    return 0;
}
