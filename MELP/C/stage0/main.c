#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

// Codegen module
#include "modules/codegen_emit/codegen_emit.h"

// Modules
#include "modules/comments/comments.h"
#include "modules/parser_core/parser_core.h"
#include "modules/expression/expression.h"
#include "modules/expression/expression_parser.h"
#include "modules/expression/expression_codegen.h"
#include "modules/variable/variable.h"
#include "modules/variable/variable_parser.h"
#include "modules/variable/variable_codegen.h"
#include "modules/print/print.h"
#include "modules/print/print_parser.h"
#include "modules/print/print_codegen.h"
#include "modules/control_flow/control_flow.h"
#include "modules/control_flow/control_flow_parser.h"
#include "modules/control_flow/control_flow_codegen.h"
#include "modules/functions/functions.h"
#include "modules/functions/functions_parser.h"
#include "modules/functions/functions_codegen.h"

int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input.mlp> <output.c>\n", argv[0]);
        return 1;
    }
    
    // Read source file
    FILE* f = fopen(argv[1], "r");
    if (!f) {
        fprintf(stderr, "Error: Cannot open input file\n");
        return 1;
    }
    
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    char* source = malloc(size + 1);
    fread(source, 1, size, f);
    source[size] = '\0';
    fclose(f);
    
    // Create lexer
    Lexer* lexer = lexer_create(source);
    
    // Open output file
    FILE* out = fopen(argv[2], "w");
    if (!out) {
        fprintf(stderr, "Error: Cannot create output file\n");
        return 1;
    }
    
    // Initialize C code generator
    emit_c_init(out);
    
    // Parse and generate code
    printf("=== MLP-GCC C CODE GENERATION ===\n");
    int stmt_count = 0;
    int has_main = 0;
    
    Token* token = lexer_next_token(lexer);
    while (token && token->type != TOKEN_EOF) {
        // Handle import statements (skip them for now)
        if (token->type == TOKEN_IMPORT) {
            token_free(token);
            token = lexer_next_token(lexer);
            
            // Skip the import path (string)
            if (token && token->type == TOKEN_STRING) {
                token_free(token);
                token = lexer_next_token(lexer);
            }
            continue;
        }
        
        // Handle enum declarations
        if (token->type == TOKEN_ENUM) {
            token_free(token);
            token = lexer_next_token(lexer);
            
            if (token && token->type == TOKEN_IDENTIFIER) {
                emit_c("typedef enum {\n");
                char enum_name[128];
                strncpy(enum_name, token->value, sizeof(enum_name) - 1);
                token_free(token);
                token = lexer_next_token(lexer);
                
                // Parse enum members
                int member_count = 0;
                while (token && token->type != TOKEN_EOF && token->type == TOKEN_IDENTIFIER) {
                    if (member_count > 0) emit_c(",\n");
                    emit_c("    %s", token->value);
                    member_count++;
                    token_free(token);
                    token = lexer_next_token(lexer);
                }
                
                emit_c("\n} %s;\n\n", enum_name);
                stmt_count++;
                continue;
            }
        }
        // Handle struct declarations
        else if (token->type == TOKEN_STRUCT) {
            token_free(token);
            token = lexer_next_token(lexer);
            
            if (token && token->type == TOKEN_IDENTIFIER) {
                char struct_name[128];
                strncpy(struct_name, token->value, sizeof(struct_name) - 1);
                emit_c("typedef struct {\n");
                token_free(token);
                token = lexer_next_token(lexer);
                
                // Parse struct fields (simplified: type name pairs)
                while (token && token->type != TOKEN_EOF) {
                    // Check for end_struct keyword (as identifier)
                    if (token->type == TOKEN_IDENTIFIER && strcmp(token->value, "end_struct") == 0) {
                        break;
                    }
                    
                    if (token->type == TOKEN_IDENTIFIER) {
                        char type_name[64], field_name[64];
                        strncpy(type_name, token->value, sizeof(type_name) - 1);
                        token_free(token);
                        token = lexer_next_token(lexer);
                        
                        if (token && token->type == TOKEN_IDENTIFIER) {
                            strncpy(field_name, token->value, sizeof(field_name) - 1);
                            
                            // Type mapping
                            if (strcmp(type_name, "Numeric") == 0) {
                                emit_c("    int64_t %s;\n", field_name);
                            } else if (strcmp(type_name, "String") == 0) {
                                emit_c("    const char* %s;\n", field_name);
                            } else if (strcmp(type_name, "list") == 0) {
                                emit_c("    void* %s;  /* list */\n", field_name);
                            } else {
                                emit_c("    %s %s;\n", type_name, field_name);
                            }
                        }
                    }
                    if (token) {
                        token_free(token);
                    }
                    token = lexer_next_token(lexer);
                }
                
                emit_c("} %s;\n\n", struct_name);
                stmt_count++;
                
                // Skip "end_struct"
                if (token && token->type == TOKEN_IDENTIFIER && strcmp(token->value, "end_struct") == 0) {
                    token_free(token);
                    token = lexer_next_token(lexer);
                }
                continue;
            }
        }
        // Handle function declarations (MLP FULL PARSER)
        else if (token->type == TOKEN_FUNCTION) {
            token_free(token);
            token = lexer_next_token(lexer);
            
            if (token && token->type == TOKEN_IDENTIFIER) {
                char func_name[128];
                strncpy(func_name, token->value, sizeof(func_name) - 1);
                
                // Default return type
                char return_type[32] = "void";
                token_free(token);
                token = lexer_next_token(lexer);
                
                // Parse parameter list
                char params[512] = "";
                int has_params = 0;
                
                if (token && token->type == TOKEN_LPAREN) {
                    token_free(token);
                    token = lexer_next_token(lexer);
                    
                    // Parse parameters: name; name2; name3
                    while (token && token->type == TOKEN_IDENTIFIER) {
                        if (has_params > 0) {
                            strcat(params, ", ");
                        }
                        strcat(params, "int64_t ");
                        strcat(params, token->value);
                        has_params++;
                        
                        token_free(token);
                        token = lexer_next_token(lexer);
                        
                        // Skip semicolon separator
                        if (token && token->type == TOKEN_SEMICOLON) {
                            token_free(token);
                            token = lexer_next_token(lexer);
                        }
                    }
                    
                    // Skip closing paren
                    if (token && token->type == TOKEN_RPAREN) {
                        token_free(token);
                        token = lexer_next_token(lexer);
                    }
                }
                
                // Check for return type annotation (skip for now)
                // MLP uses: function name(...) or just function name(...)
                
                // Default to int64_t if not void
                strcpy(return_type, "int64_t");
                
                // Emit function signature
                if (has_params == 0) {
                    emit_c("%s %s(void) {\n", return_type, func_name);
                } else {
                    emit_c("%s %s(%s) {\n", return_type, func_name, params);
                }
                
                // Parse function body (MLP FULL SYNTAX)
                int body_depth = 0;
                int indent = 1;
                
                while (token && token->type != TOKEN_EOF && token->type != TOKEN_END_FUNCTION) {
                    // Check for end_function keyword (MLP style)
                    if (token->type == TOKEN_IDENTIFIER && strcmp(token->value, "end_function") == 0) {
                        break;
                    }
                    
                    // Variable declaration: Type varname = value
                    if (token->type == TOKEN_IDENTIFIER) {
                        char type_or_var[128];
                        strncpy(type_or_var, token->value, sizeof(type_or_var) - 1);
                        token_free(token);
                        token = lexer_next_token(lexer);
                        
                        // Check if it's a type declaration (Numeric, String, etc.)
                        if (token && token->type == TOKEN_IDENTIFIER) {
                            char var_name[128];
                            strncpy(var_name, token->value, sizeof(var_name) - 1);
                            token_free(token);
                            token = lexer_next_token(lexer);
                            
                            // Translate MLP types to C
                            char c_type[32] = "int64_t";
                            if (strcmp(type_or_var, "String") == 0) strcpy(c_type, "char*");
                            else if (strcmp(type_or_var, "Numeric") == 0) strcpy(c_type, "int64_t");
                            else if (strcmp(type_or_var, "Boolean") == 0) strcpy(c_type, "int");
                            
                            // Check for assignment
                            if (token && token->type == TOKEN_ASSIGN) {
                                token_free(token);
                                token = lexer_next_token(lexer);
                                
                                // Collect expression until end of statement
                                // Stop at: keywords (if/while/return/etc), identifiers at line start, end_*
                                char expr[512] = "";
                                int expr_has_content = 0;
                                while (token && token->type != TOKEN_EOF) {
                                    // Stop conditions
                                    if (token->type == TOKEN_IF || token->type == TOKEN_WHILE || 
                                        token->type == TOKEN_FOR || token->type == TOKEN_RETURN ||
                                        token->type == TOKEN_END_IF || token->type == TOKEN_END ||
                                        token->type == TOKEN_END_FUNCTION ||
                                        token->type == TOKEN_EXIT || token->type == TOKEN_CONTINUE) {
                                        break;
                                    }
                                    
                                    // Check for new statement (identifier without operator before)
                                    if (token->type == TOKEN_IDENTIFIER && expr_has_content) {
                                        // Peek ahead - if it's an assignment or type keyword, break
                                        Token* peek = lexer_next_token(lexer);
                                        int is_new_stmt = 0;
                                        if (peek) {
                                            if (peek->type == TOKEN_ASSIGN || peek->type == TOKEN_IDENTIFIER) {
                                                is_new_stmt = 1;
                                            }
                                            // Put token back (can't actually do this, so we break and let outer loop handle)
                                            token_free(peek);
                                        }
                                        if (is_new_stmt) {
                                            break;
                                        }
                                    }
                                    
                                    if (strlen(expr) > 0) strcat(expr, " ");
                                    strcat(expr, token->value);
                                    expr_has_content = 1;
                                    token_free(token);
                                    token = lexer_next_token(lexer);
                                }
                                
                                for (int i = 0; i < indent; i++) emit_c("    ");
                                emit_c("%s %s = %s;\n", c_type, var_name, expr);
                            } else {
                                // Just declaration
                                for (int i = 0; i < indent; i++) emit_c("    ");
                                emit_c("%s %s;\n", c_type, var_name);
                            }
                            continue;
                        } else {
                            // Assignment or function call: varname = expr or func()
                            if (token && token->type == TOKEN_ASSIGN) {
                                token_free(token);
                                token = lexer_next_token(lexer);
                                
                                // Collect expression
                                char expr[512] = "";
                                while (token && token->type != TOKEN_EOF) {
                                    // Stop at control flow keywords or new statements
                                    if (token->type == TOKEN_IF || token->type == TOKEN_WHILE || 
                                        token->type == TOKEN_FOR || token->type == TOKEN_RETURN ||
                                        token->type == TOKEN_END_IF || token->type == TOKEN_END ||
                                        token->type == TOKEN_END_FUNCTION ||
                                        token->type == TOKEN_EXIT || token->type == TOKEN_CONTINUE) {
                                        break;
                                    }
                                    
                                    if (strlen(expr) > 0) strcat(expr, " ");
                                    strcat(expr, token->value);
                                    token_free(token);
                                    token = lexer_next_token(lexer);
                                }
                                
                                for (int i = 0; i < indent; i++) emit_c("    ");
                                emit_c("%s = %s;\n", type_or_var, expr);
                                continue;
                            }
                            // If no assignment, might be function call - skip for now
                            continue;
                        }
                    }
                    
                    // If statement
                    if (token->type == TOKEN_IF) {
                        token_free(token);
                        token = lexer_next_token(lexer);
                        
                        // Collect condition
                        char condition[512] = "";
                        while (token && token->type != TOKEN_THEN) {
                            if (strlen(condition) > 0) strcat(condition, " ");
                            strcat(condition, token->value);
                            token_free(token);
                            token = lexer_next_token(lexer);
                        }
                        
                        // Skip THEN
                        if (token && token->type == TOKEN_THEN) {
                            token_free(token);
                            token = lexer_next_token(lexer);
                        }
                        
                        for (int i = 0; i < indent; i++) emit_c("    ");
                        emit_c("if (%s) {\n", condition);
                        indent++;
                        body_depth++;
                        continue;
                    }
                    
                    // End if
                    if (token->type == TOKEN_END_IF) {
                        indent--;
                        for (int i = 0; i < indent; i++) emit_c("    ");
                        emit_c("}\n");
                        body_depth--;
                        token_free(token);
                        token = lexer_next_token(lexer);
                        continue;
                    }
                    
                    // While statement
                    if (token->type == TOKEN_WHILE) {
                        token_free(token);
                        token = lexer_next_token(lexer);
                        
                        // Collect condition
                        char condition[512] = "";
                        while (token && (token->type != TOKEN_IDENTIFIER || 
                               (token && strcmp(token->value, "end") != 0))) {
                            if (strlen(condition) > 0) strcat(condition, " ");
                            strcat(condition, token->value);
                            token_free(token);
                            token = lexer_next_token(lexer);
                            
                            // Break on 'end' keyword or newline
                            if (token && token->type == TOKEN_IDENTIFIER && strcmp(token->value, "end") == 0) {
                                break;
                            }
                        }
                        
                        for (int i = 0; i < indent; i++) emit_c("    ");
                        emit_c("while (%s) {\n", condition);
                        indent++;
                        body_depth++;
                        continue;
                    }
                    
                    // End (while/for)
                    if (token->type == TOKEN_END) {
                        indent--;
                        for (int i = 0; i < indent; i++) emit_c("    ");
                        emit_c("}\n");
                        body_depth--;
                        token_free(token);
                        token = lexer_next_token(lexer);
                        continue;
                    }
                    
                    // Return statement
                    if (token->type == TOKEN_RETURN) {
                        token_free(token);
                        token = lexer_next_token(lexer);
                        
                        // Collect expression until end of statement
                        char expr[512] = "";
                        while (token && token->type != TOKEN_EOF) {
                            // Stop at control flow keywords
                            if (token->type == TOKEN_END_IF || token->type == TOKEN_END ||
                                token->type == TOKEN_END_FUNCTION ||
                                token->type == TOKEN_IF || token->type == TOKEN_WHILE ||
                                token->type == TOKEN_EXIT || token->type == TOKEN_CONTINUE) {
                                break;
                            }
                            
                            // Stop at identifier that might start new statement
                            if (token->type == TOKEN_IDENTIFIER && strlen(expr) > 0) {
                                // Simple heuristic: if we have content and see capitalized identifier (Type), break
                                if (token->value[0] >= 'A' && token->value[0] <= 'Z') {
                                    break;
                                }
                            }
                            
                            if (strlen(expr) > 0) strcat(expr, " ");
                            strcat(expr, token->value);
                            token_free(token);
                            token = lexer_next_token(lexer);
                        }
                        
                        for (int i = 0; i < indent; i++) emit_c("    ");
                        if (strlen(expr) > 0) {
                            emit_c("return %s;\n", expr);
                        } else {
                            emit_c("return;\n");
                        }
                        continue;
                    }
                    
                    // Break statement
                    if (token->type == TOKEN_EXIT) {
                        for (int i = 0; i < indent; i++) emit_c("    ");
                        emit_c("break;\n");
                        token_free(token);
                        token = lexer_next_token(lexer);
                        continue;
                    }
                    
                    // Continue statement
                    if (token->type == TOKEN_CONTINUE) {
                        for (int i = 0; i < indent; i++) emit_c("    ");
                        emit_c("continue;\n");
                        token_free(token);
                        token = lexer_next_token(lexer);
                        continue;
                    }
                    
                    // Unknown - skip
                    if (token) {
                        token_free(token);
                    }
                    token = lexer_next_token(lexer);
                }
                
                emit_c("}\n\n");
                stmt_count++;
                
                // Skip end_function
                if (token && token->type == TOKEN_END_FUNCTION) {
                    token_free(token);
                    token = lexer_next_token(lexer);
                } else if (token && token->type == TOKEN_IDENTIFIER && strcmp(token->value, "end_function") == 0) {
                    token_free(token);
                    token = lexer_next_token(lexer);
                }
                continue;
            }
        }
//         // Handle if statements
//         else if (token->type == TOKEN_IF) {
//             if (!has_main) {
//                 emit_c("int main(void) {\n");
//                 has_main = 1;
//             }
//             
//             ControlFlowParser* ctrl_parser = control_flow_parser_create(lexer);
//             ctrl_parser->current_token = token;
//             
//             IfStatement* if_stmt = control_flow_parse_if(ctrl_parser);
//             if (if_stmt) {
//                 control_flow_generate_if(out, if_stmt);
//                 stmt_count++;
//             }
//             
//             control_flow_parser_free(ctrl_parser);
//             
//             token = lexer_next_token(lexer);
//             continue;
//         }
//         // Handle while loops
//         else if (token->type == TOKEN_WHILE) {
//             if (!has_main) {
//                 emit_c("int main(void) {\n");
//                 has_main = 1;
//             }
//             
//             ControlFlowParser* ctrl_parser = control_flow_parser_create(lexer);
//             ctrl_parser->current_token = token;
//             
//             WhileStatement* while_stmt = control_flow_parse_while(ctrl_parser);
//             if (while_stmt) {
//                 control_flow_generate_while(out, while_stmt);
//                 stmt_count++;
//             }
//             
//             control_flow_parser_free(ctrl_parser);
//             
//             token = lexer_next_token(lexer);
//             continue;
//         }
        // Handle for loops
        else if (token->type == TOKEN_FOR) {
            if (!has_main) {
                emit_c("int main(void) {\n");
                has_main = 1;
            }
            
            ControlFlowParser* ctrl_parser = control_flow_parser_create(lexer);
            ctrl_parser->current_token = token;
            
            ForStatement* for_stmt = control_flow_parse_for(ctrl_parser);
            if (for_stmt) {
                control_flow_generate_for(out, for_stmt);
                stmt_count++;
            }
            
            control_flow_parser_free(ctrl_parser);
            
            token = lexer_next_token(lexer);
            continue;
        }
        // Handle print statements (inside main)
        else if (token->type == TOKEN_PRINT) {
            if (!has_main) {
                emit_c("int main(void) {\n");
                has_main = 1;
            }
            token_free(token);
            token = lexer_next_token(lexer);  // Get next token (should be LPAREN)
            
            // Skip LPAREN if present
            if (token && token->type == TOKEN_LPAREN) {
                token_free(token);
                token = lexer_next_token(lexer);
            }
            
            // Get string argument
            if (token && token->type == TOKEN_STRING) {
                emit_c("    printf(\"%s\\n\");\n", token->value);
                stmt_count++;
                token_free(token);
                token = lexer_next_token(lexer);
                
                // Skip RPAREN if present
                if (token && token->type == TOKEN_RPAREN) {
                    token_free(token);
                    token = lexer_next_token(lexer);
                }
            }
            continue;
        }
        // Handle variable declarations (numeric type)
        else if (token->type == TOKEN_NUMERIC) {
            char var_name[64] = {0};
            char var_value[64] = {0};
            
            token_free(token);
            token = lexer_next_token(lexer);
            
            if (token && token->type == TOKEN_IDENTIFIER) {
                strncpy(var_name, token->value, sizeof(var_name) - 1);
                token_free(token);
                token = lexer_next_token(lexer);
                
                // Skip '='
                if (token && token->type == TOKEN_ASSIGN) {
                    token_free(token);
                    token = lexer_next_token(lexer);
                    
                    if (token && token->type == TOKEN_NUMBER) {
                        strncpy(var_value, token->value, sizeof(var_value) - 1);
                        
                        if (!has_main) {
                            emit_c("int main(void) {\n");
                            has_main = 1;
                        }
                        
                        emit_c("    int64_t %s = %s;\n", var_name, var_value);
                        stmt_count++;
                    }
                }
            }
            continue;
        }
        
        if (token) {
            token_free(token);
        }
        token = lexer_next_token(lexer);
    }
    
    // Emit main function end (if we created one)
    if (has_main) {
        emit_c("    return 0;\n");
        emit_c("}\n");
    }
    
    // Finalize C code generator
    emit_c_finalize();
    
    printf("\n✅ C code generation successful!\n");
    printf("Generated %d statements.\n", stmt_count);
    printf("C code output: %s\n", argv[2]);
    
    fclose(out);
    free(source);
    
    return 0;
}
