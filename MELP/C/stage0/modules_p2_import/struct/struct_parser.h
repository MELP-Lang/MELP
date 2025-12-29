#ifndef STRUCT_PARSER_H
#define STRUCT_PARSER_H

#include "struct.h"
#include "../lexer/lexer.h"
#include "../parser_core/parser_core.h"  // YZ_88: Parser context for method bodies

// Parser fonksiyonları - Stage 1'de implement edilecek
StructDef* parse_struct_definition(Parser* parser);  // YZ_88: Now takes Parser*
StructInstance* parse_struct_instance(Lexer* lexer);
MemberAccess* parse_member_access(Lexer* lexer);

// YZ_82: Helper to detect if next tokens are a struct instance declaration
// Returns struct definition if "TypeName varname" pattern detected, NULL otherwise
StructDef* struct_try_parse_instance_decl(Lexer* lexer, Token* first_token);

#endif // STRUCT_PARSER_H
