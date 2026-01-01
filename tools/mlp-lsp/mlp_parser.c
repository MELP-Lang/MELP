// MLP Parser Implementation
#include "mlp_parser.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

// MLP keywords
static const char* MLP_KEYWORDS[] = {
    "fn", "let", "if", "else", "elif", "while", "for", "in",
    "return", "break", "continue", "pass", "import", "from",
    "type", "struct", "enum", "True", "False", "None",
    "and", "or", "not", "is", "as", NULL
};

// Check if word is keyword
bool parser_is_keyword(const char* word) {
    for (int i = 0; MLP_KEYWORDS[i] != NULL; i++) {
        if (strcmp(word, MLP_KEYWORDS[i]) == 0) {
            return true;
        }
    }
    return false;
}

// Create parser
Parser* parser_create(const char* source, const char* file_uri) {
    Parser* parser = malloc(sizeof(Parser));
    if (!parser) return NULL;
    
    parser->source = source;
    parser->file_uri = file_uri ? strdup(file_uri) : NULL;
    parser->position = 0;
    parser->line = 0;
    parser->column = 0;
    parser->symbols = symbol_table_create();
    
    parser->current_token.type = TOKEN_EOF;
    parser->current_token.value = NULL;
    parser->current_token.line = 0;
    parser->current_token.column = 0;
    
    return parser;
}

// Free parser
void parser_free(Parser* parser) {
    if (!parser) return;
    
    free((char*)parser->file_uri);
    if (parser->current_token.value) {
        free(parser->current_token.value);
    }
    // Note: symbol table ownership transferred via parser_get_symbols()
    // Only free if not transferred
    if (parser->symbols) {
        symbol_table_free(parser->symbols);
    }
    free(parser);
}

// Free token
void parser_free_token(Token* token) {
    if (token && token->value) {
        free(token->value);
        token->value = NULL;
    }
}

// Peek character
static char parser_peek(Parser* parser) {
    if (parser->source[parser->position] == '\0') {
        return '\0';
    }
    return parser->source[parser->position];
}

// Advance position
static char parser_advance(Parser* parser) {
    char c = parser->source[parser->position];
    if (c == '\0') return '\0';
    
    parser->position++;
    parser->column++;
    
    if (c == '\n') {
        parser->line++;
        parser->column = 0;
    }
    
    return c;
}

// Skip whitespace (except newlines)
static void parser_skip_whitespace(Parser* parser) {
    while (parser_peek(parser) == ' ' || parser_peek(parser) == '\t' || parser_peek(parser) == '\r') {
        parser_advance(parser);
    }
}

// Next token
Token parser_next_token(Parser* parser) {
    Token token;
    token.value = NULL;
    token.line = parser->line;
    token.column = parser->column;
    
    parser_skip_whitespace(parser);
    
    char c = parser_peek(parser);
    
    // EOF
    if (c == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    // Newline
    if (c == '\n') {
        parser_advance(parser);
        token.type = TOKEN_NEWLINE;
        return token;
    }
    
    // Comment
    if (c == '#') {
        while (parser_peek(parser) != '\n' && parser_peek(parser) != '\0') {
            parser_advance(parser);
        }
        return parser_next_token(parser);  // Skip comment, get next token
    }
    
    // String
    if (c == '"' || c == '\'') {
        char quote = c;
        parser_advance(parser);  // Skip opening quote
        
        int start = parser->position;
        while (parser_peek(parser) != quote && parser_peek(parser) != '\0') {
            parser_advance(parser);
        }
        
        int length = parser->position - start;
        token.value = malloc(length + 1);
        strncpy(token.value, parser->source + start, length);
        token.value[length] = '\0';
        
        parser_advance(parser);  // Skip closing quote
        token.type = TOKEN_STRING;
        return token;
    }
    
    // Number
    if (isdigit(c)) {
        int start = parser->position;
        while (isdigit(parser_peek(parser)) || parser_peek(parser) == '.') {
            parser_advance(parser);
        }
        
        int length = parser->position - start;
        token.value = malloc(length + 1);
        strncpy(token.value, parser->source + start, length);
        token.value[length] = '\0';
        token.type = TOKEN_NUMBER;
        return token;
    }
    
    // Identifier or keyword
    if (isalpha(c) || c == '_') {
        int start = parser->position;
        while (isalnum(parser_peek(parser)) || parser_peek(parser) == '_') {
            parser_advance(parser);
        }
        
        int length = parser->position - start;
        token.value = malloc(length + 1);
        strncpy(token.value, parser->source + start, length);
        token.value[length] = '\0';
        
        if (parser_is_keyword(token.value)) {
            token.type = TOKEN_KEYWORD;
        } else {
            token.type = TOKEN_IDENTIFIER;
        }
        return token;
    }
    
    // Operators and punctuation
    parser_advance(parser);
    token.value = malloc(2);
    token.value[0] = c;
    token.value[1] = '\0';
    
    if (strchr("()[]{}:,.", c)) {
        token.type = TOKEN_PUNCTUATION;
    } else {
        token.type = TOKEN_OPERATOR;
    }
    
    return token;
}

// Parse function definition: fn name(params) -> return_type:
static void parser_parse_function(Parser* parser, Token* fn_token) {
    // Get function name
    Token name = parser_next_token(parser);
    if (name.type != TOKEN_IDENTIFIER) {
        parser_free_token(&name);
        return;
    }
    
    // Create function symbol
    Symbol* func = symbol_create(name.value, SYMBOL_FUNCTION, NULL);
    func->file_uri = parser->file_uri ? strdup(parser->file_uri) : NULL;
    func->position.line = fn_token->line;
    func->position.character = fn_token->column;
    
    // Build signature
    char signature[256] = "fn(";
    
    // Parse parameters
    Token lparen = parser_next_token(parser);
    if (lparen.type == TOKEN_PUNCTUATION && lparen.value[0] == '(') {
        bool first_param = true;
        
        while (1) {
            Token param = parser_next_token(parser);
            
            if (param.type == TOKEN_PUNCTUATION && param.value[0] == ')') {
                parser_free_token(&param);
                break;
            }
            
            if (param.type == TOKEN_IDENTIFIER) {
                if (!first_param) {
                    strcat(signature, ", ");
                }
                
                // Check for type annotation
                Token colon = parser_next_token(parser);
                if (colon.type == TOKEN_PUNCTUATION && colon.value[0] == ':') {
                    Token type = parser_next_token(parser);
                    if (type.type == TOKEN_IDENTIFIER) {
                        strcat(signature, type.value);
                        
                        // Add parameter as symbol (in function scope)
                        symbol_table_enter_scope(parser->symbols);
                        Symbol* param_sym = symbol_create(param.value, SYMBOL_PARAMETER, type.value);
                        param_sym->file_uri = parser->file_uri ? strdup(parser->file_uri) : NULL;
                        symbol_table_add(parser->symbols, param_sym);
                    }
                    parser_free_token(&type);
                } else {
                    strcat(signature, "Any");
                }
                parser_free_token(&colon);
                
                first_param = false;
            }
            
            parser_free_token(&param);
            
            // Check for comma or closing paren
            Token sep = parser_next_token(parser);
            if (sep.type == TOKEN_PUNCTUATION && sep.value[0] == ')') {
                parser_free_token(&sep);
                break;
            }
            parser_free_token(&sep);
        }
    }
    parser_free_token(&lparen);
    
    strcat(signature, ")");
    
    // Parse return type
    Token arrow = parser_next_token(parser);
    if (arrow.type == TOKEN_OPERATOR) {  // ->
        Token ret_type = parser_next_token(parser);
        if (ret_type.type == TOKEN_IDENTIFIER) {
            strcat(signature, " -> ");
            strcat(signature, ret_type.value);
            func->type_name = strdup(ret_type.value);
        }
        parser_free_token(&ret_type);
    }
    parser_free_token(&arrow);
    
    func->signature = strdup(signature);
    symbol_table_add(parser->symbols, func);
    
    parser_free_token(&name);
}

// Parse variable definition: let name = value or let name: type = value
static void parser_parse_let(Parser* parser, Token* let_token) {
    // Get variable name
    Token name = parser_next_token(parser);
    if (name.type != TOKEN_IDENTIFIER) {
        parser_free_token(&name);
        return;
    }
    
    Symbol* var = symbol_create(name.value, SYMBOL_VARIABLE, NULL);
    var->file_uri = parser->file_uri ? strdup(parser->file_uri) : NULL;
    var->position.line = let_token->line;
    var->position.character = let_token->column;
    
    // Check for type annotation
    Token next = parser_next_token(parser);
    if (next.type == TOKEN_PUNCTUATION && next.value[0] == ':') {
        Token type = parser_next_token(parser);
        if (type.type == TOKEN_IDENTIFIER) {
            var->type_name = strdup(type.value);
        }
        parser_free_token(&type);
    }
    parser_free_token(&next);
    
    symbol_table_add(parser->symbols, var);
    parser_free_token(&name);
}

// Parse import: from module import name
static void parser_parse_import(Parser* parser, Token* from_token) {
    (void)from_token;
    
    // Parse: from module import name
    Token module = parser_next_token(parser);
    if (module.type == TOKEN_IDENTIFIER) {
        Token import_kw = parser_next_token(parser);
        if (import_kw.type == TOKEN_KEYWORD) {
            Token name = parser_next_token(parser);
            if (name.type == TOKEN_IDENTIFIER) {
                Symbol* import_sym = symbol_create(name.value, SYMBOL_IMPORT, NULL);
                import_sym->file_uri = parser->file_uri ? strdup(parser->file_uri) : NULL;
                import_sym->documentation = malloc(strlen(module.value) + 20);
                sprintf(import_sym->documentation, "from %s", module.value);
                symbol_table_add(parser->symbols, import_sym);
            }
            parser_free_token(&name);
        }
        parser_free_token(&import_kw);
    }
    parser_free_token(&module);
}

// Main parse function
bool parser_parse(Parser* parser) {
    if (!parser) return false;
    
    // Add built-in keywords as symbols
    for (int i = 0; MLP_KEYWORDS[i] != NULL; i++) {
        Symbol* keyword = symbol_create(MLP_KEYWORDS[i], SYMBOL_KEYWORD, NULL);
        symbol_table_add(parser->symbols, keyword);
    }
    
    // Parse tokens
    while (1) {
        Token token = parser_next_token(parser);
        
        if (token.type == TOKEN_EOF) {
            parser_free_token(&token);
            break;
        }
        
        if (token.type == TOKEN_KEYWORD) {
            if (strcmp(token.value, "fn") == 0) {
                parser_parse_function(parser, &token);
            } else if (strcmp(token.value, "let") == 0) {
                parser_parse_let(parser, &token);
            } else if (strcmp(token.value, "from") == 0) {
                parser_parse_import(parser, &token);
            }
        }
        
        parser_free_token(&token);
    }
    
    return true;
}

// Get symbol table (ownership transferred)
SymbolTable* parser_get_symbols(Parser* parser) {
    if (!parser) return NULL;
    
    SymbolTable* symbols = parser->symbols;
    parser->symbols = NULL;  // Transfer ownership
    return symbols;
}
