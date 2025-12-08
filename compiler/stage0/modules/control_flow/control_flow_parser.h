#ifndef CONTROL_FLOW_PARSER_H
#define CONTROL_FLOW_PARSER_H

#include "control_flow.h"
#include "../lexer/lexer.h"

// ============================================================================
// STATELESS TEMPLATE FUNCTIONS - NO MALLOC/FREE!
// ============================================================================
// These functions are templates - they don't create parser structs.
// Token ownership: Caller owns input token, function owns tokens it reads.
// ============================================================================

// Parse if statement
// Input: if_token - the TOKEN_IF (caller still owns it, won't be freed)
// Returns: IfStatement* or NULL on error
// Note: Reads additional tokens from lexer, frees only what it reads
IfStatement* control_flow_parse_if(Lexer* lexer, Token* if_token);

// Parse while statement  
// Input: while_token - the TOKEN_WHILE (caller still owns it, won't be freed)
// Returns: WhileStatement* or NULL on error
// Note: Reads additional tokens from lexer, frees only what it reads
WhileStatement* control_flow_parse_while(Lexer* lexer, Token* while_token);

// Parse for statement (future)
ForStatement* control_flow_parse_for(Lexer* lexer, Token* for_token);

// Parse expression-based if (Tier 1 feature)
IfStatement* control_flow_parse_if_expr(Lexer* lexer, Token* current_token);

// ============================================================================
// DEPRECATED - Old stateful parser API (will be removed)
// ============================================================================
typedef struct {
    Lexer* lexer;
    Token* current_token;
} ControlFlowParser;

ControlFlowParser* control_flow_parser_create(Lexer* lexer);
void control_flow_parser_free(ControlFlowParser* parser);

#endif // CONTROL_FLOW_PARSER_H
