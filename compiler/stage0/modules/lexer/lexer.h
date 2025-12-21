#ifndef LEXER_H
#define LEXER_H

typedef enum {
    TOKEN_NUMERIC,
    TOKEN_STRING_TYPE,     // string keyword (type declaration)
    TOKEN_BOOLEAN,
    TOKEN_ARRAY,           // Array keyword (type declaration) - YZ_74
    TOKEN_LIST,            // list keyword (type declaration)
    TOKEN_MAP,             // map keyword (type declaration) - YZ_201
    TOKEN_TUPLE,           // tuple keyword (type declaration)
    TOKEN_IDENTIFIER,
    TOKEN_ASSIGN,
    TOKEN_NUMBER,
    TOKEN_STRING,          // "string literal"
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_NULL,            // null (YZ_202)
    // Arithmetic operators
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_MOD,             // %
    TOKEN_POWER,           // **
    TOKEN_XOR,             // xor (also ^) - bitwise XOR
    // Comparison operators
    TOKEN_EQUAL,           // ==
    TOKEN_NOT_EQUAL,       // !=
    TOKEN_LESS,            // <
    TOKEN_GREATER,         // >
    TOKEN_LESS_EQUAL,      // <=
    TOKEN_GREATER_EQUAL,   // >=
    // Logical operators
    TOKEN_AND,             // &&
    TOKEN_OR,              // ||
    TOKEN_NOT,             // !
    // Control flow keywords
    TOKEN_IF,
    TOKEN_THEN,
    TOKEN_ELSE,
    TOKEN_END,
    TOKEN_WHILE,
    TOKEN_DO,
    TOKEN_FOR,
    TOKEN_FROM,            // from (for loop range start)
    TOKEN_TO,
    TOKEN_DOWNTO,
    TOKEN_SWITCH,          // switch
    TOKEN_CASE,            // case
    TOKEN_DEFAULT,         // default
    TOKEN_EACH,            // for each item in collection
    TOKEN_IN,              // for each item in collection
    TOKEN_EXIT,            // exit, exit for, exit while, exit if, exit function
    
    // Exception handling keywords (YZ_207)
    TOKEN_TRY,             // try
    TOKEN_CATCH,           // catch
    TOKEN_FINALLY,         // finally
    TOKEN_THROW,           // throw
    TOKEN_EXCEPTION,       // exception (type definition)
    
    // ⭐ PMPL Block Terminators (underscore keywords)
    TOKEN_END_IF,          // end_if
    TOKEN_END_WHILE,       // end_while
    TOKEN_END_FOR,         // end_for
    TOKEN_END_FUNCTION,    // end_function
    TOKEN_END_STRUCT,      // end_struct
    TOKEN_END_ENUM,        // end_enum
    TOKEN_END_SWITCH,      // end_switch
    TOKEN_END_MATCH,       // end_match
    TOKEN_END_OPERATOR,    // end_operator
    TOKEN_END_TRY,         // end_try
    TOKEN_END_EXCEPTION,   // end_exception
    
    // ⭐ PMPL Control Flow
    TOKEN_ELSE_IF,         // else_if
    
    // ⭐ PMPL Loop Control
    TOKEN_CONTINUE,        // continue (standalone - current loop)
    TOKEN_EXIT_IF,         // exit_if
    TOKEN_EXIT_FOR,        // exit_for
    TOKEN_EXIT_WHILE,      // exit_while
    TOKEN_EXIT_FUNCTION,   // exit_function
    TOKEN_EXIT_SWITCH,     // exit_switch
    TOKEN_CONTINUE_FOR,    // continue_for
    TOKEN_CONTINUE_WHILE,  // continue_while
    
    // ⭐ PMPL Debug Block
    TOKEN_DEBUG,           // debug (block start or single-line prefix)
    TOKEN_END_DEBUG,       // end_debug (block end)
    TOKEN_GOTO,            // goto (debug-only)
    TOKEN_PAUSE,           // pause (debug-only)
    
    // ⭐ PMPL State Keywords
    TOKEN_SHARED_STATE,    // shared_state
    // Function keywords
    TOKEN_FUNCTION,
    TOKEN_RETURN,
    TOKEN_RETURNS,         // returns (for return type declaration)
    TOKEN_AS,              // as (alternative for return type: func() as numeric)
    // Struct keywords
    TOKEN_STRUCT,
    TOKEN_ENUM,
    TOKEN_METHOD,          // method (struct method)
    TOKEN_END_METHOD,      // end_method
    TOKEN_SELF,            // self (method receiver)
    // Module keywords
    TOKEN_IMPORT,          // import module_name
    TOKEN_MODULE,          // module keyword
    TOKEN_EXPORT,          // export (YZ_204)
    // Const keyword
    TOKEN_CONST,           // const (constant declaration)
    TOKEN_REF,             // ref (reference parameter)
    TOKEN_OUT,             // out (output parameter)
    TOKEN_OPTIONAL,        // optional (optional parameter)
    // Null safety operators (YZ_202)
    TOKEN_QUESTION,        // ? (nullable type, safe navigation)
    TOKEN_DOUBLE_QUESTION, // ?? (null coalescing)
    // I/O keywords
    TOKEN_PRINT,
    TOKEN_PRINTLN,         // println builtin (YZ_47)
    // List operation keywords (YZ_200)
    TOKEN_APPEND,          // append(list; value)
    TOKEN_PREPEND,         // prepend(list; value)
    TOKEN_LENGTH,          // length(list)
    TOKEN_CLEAR,           // clear(list)
    // Map operation keywords (YZ_201)
    TOKEN_INSERT,          // insert(map; key; value)
    TOKEN_GET,             // get(map; key)
    TOKEN_REMOVE,          // remove(map; key)
    TOKEN_HAS_KEY,         // has_key(map; key)
    // Ownership keywords (YZ_211)
    TOKEN_MOVE,            // move (ownership transfer)
    TOKEN_BORROW,          // borrow (temporary access)
    TOKEN_MUT,             // mut (mutable borrow)
    
    // Collections Library keywords (YZ_213)
    // List extensions
    TOKEN_INSERT_AT,       // insert_at(list; index; value)
    TOKEN_FIND_IN_LIST,    // find_in_list(list; value)
    TOKEN_SORT_LIST,       // sort_list(list)
    // Map extensions
    TOKEN_MAP_KEYS,        // map_keys(map) -> list
    TOKEN_MAP_VALUES,      // map_values(map) -> list
    TOKEN_CLEAR_MAP,       // clear_map(map)
    // Set operations
    TOKEN_CREATE_SET,      // create_set() -> Set
    TOKEN_ADD_TO_SET,      // add_to_set(set; value)
    TOKEN_SET_CONTAINS,    // set_contains(set; value) -> bool
    TOKEN_REMOVE_FROM_SET, // remove_from_set(set; value)
    TOKEN_SET_UNION,       // set_union(set1; set2) -> Set
    TOKEN_SET_INTERSECTION,// set_intersection(set1; set2) -> Set
    TOKEN_SET_DIFFERENCE,  // set_difference(set1; set2) -> Set
    TOKEN_SET_SIZE,        // set_size(set) -> numeric
    // Queue operations
    TOKEN_CREATE_QUEUE,    // create_queue() -> Queue
    TOKEN_ENQUEUE,         // enqueue(queue; value)
    TOKEN_DEQUEUE,         // dequeue(queue) -> value
    TOKEN_QUEUE_PEEK,      // queue_peek(queue) -> value
    TOKEN_QUEUE_SIZE,      // queue_size(queue) -> numeric
    TOKEN_QUEUE_IS_EMPTY,  // queue_is_empty(queue) -> bool
    // Stack operations
    TOKEN_CREATE_STACK,    // create_stack() -> Stack
    TOKEN_PUSH,            // push(stack; value)
    TOKEN_POP,             // pop(stack) -> value
    TOKEN_STACK_PEEK,      // stack_peek(stack) -> value
    TOKEN_STACK_SIZE,      // stack_size(stack) -> numeric
    TOKEN_STACK_IS_EMPTY,  // stack_is_empty(stack) -> bool
    
    // Delimiters
    TOKEN_LPAREN,          // (
    TOKEN_RPAREN,          // )
    TOKEN_LBRACKET,        // [
    TOKEN_RBRACKET,        // ]
    TOKEN_LBRACE,          // { (precision/block)
    TOKEN_RBRACE,          // } (precision/block)
    TOKEN_LANGLE,          // < (tuple literal, not comparison)
    TOKEN_RANGLE,          // > (tuple literal, not comparison)
    TOKEN_COMMA,           // ,
    TOKEN_COLON,           // :
    TOKEN_COLON_ASSIGN,    // := (default value assignment)
    TOKEN_SEMICOLON,       // ;
    TOKEN_DOT,             // .
    TOKEN_ELLIPSIS,        // ... (variadic parameters)
    TOKEN_CONTINUATION,    // _ or \ (line continuation)
    TOKEN_EOF,
    TOKEN_ERROR
} TokenType;

typedef struct {
    TokenType type;
    char* value;
    int line;
    int has_leading_whitespace;  // YZ_24: 1 if whitespace precedes token, 0 otherwise
} Token;

typedef struct {
    const char* source;
    int pos;
    int line;
    Token* pushback_token;  // ✅ For parser coordination: last read token can be "ungot"
} Lexer;

Lexer* lexer_create(const char* source);
void lexer_free(Lexer* lexer);
Token* lexer_next_token(Lexer* lexer);
void lexer_unget_token(Lexer* lexer, Token* token);  // ✅ Push token back
void token_free(Token* token);

#endif
