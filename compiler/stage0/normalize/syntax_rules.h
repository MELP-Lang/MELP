#ifndef SYNTAX_RULES_H
#define SYNTAX_RULES_H

/**
 * PMPL Syntax Normalization Rules
 * 
 * Bu dosya kullanıcı input'undaki iki kelimelik keyword'lerin
 * PMPL'deki underscore versiyonlarına dönüşüm kurallarını içerir.
 * 
 * PMPL = Pragmatic MLP (Compiler'ın gördüğü TEK syntax)
 */

// Dönüşüm kuralı yapısı
typedef struct {
    const char* from;  // Kullanıcı input'u (iki kelime)
    const char* to;    // PMPL keyword (underscore ile)
} SyntaxRule;

// Tüm dönüşüm kuralları (22 kural)
static const SyntaxRule PMPL_RULES[] = {
    // Block Terminators (9 kural)
    {"end if",       "end_if"},
    {"end while",    "end_while"},
    {"end for",      "end_for"},
    {"end function", "end_function"},
    {"end struct",   "end_struct"},
    {"end switch",   "end_switch"},
    {"end match",    "end_match"},
    {"end operator", "end_operator"},
    {"end try",      "end_try"},
    
    // Control Flow (1 kural)
    {"else if",      "else_if"},
    
    // Loop Control - Exit (5 kural)
    {"exit if",      "exit_if"},
    {"exit for",     "exit_for"},
    {"exit while",   "exit_while"},
    {"exit function","exit_function"},
    {"exit switch",  "exit_switch"},
    
    // Loop Control - Continue (2 kural)
    {"continue for", "continue_for"},
    {"continue while","continue_while"},
    
    // Debug Keywords (4 kural)
    {"debug goto",   "debug_goto"},
    {"debug pause",  "debug_pause"},
    {"debug label",  "debug_label"},
    {"debug print",  "debug_print"},
    
    // State Keywords (1 kural)
    {"shared state", "shared_state"},
    
    // Terminator
    {NULL, NULL}
};

// Kural sayısı
#define PMPL_RULES_COUNT 22

#endif // SYNTAX_RULES_H
