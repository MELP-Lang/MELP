// LSP Type Definitions
// Language Server Protocol types for MLP
#ifndef LSP_TYPES_H
#define LSP_TYPES_H

#include <stdbool.h>

// LSP Message Types
typedef enum {
    LSP_REQUEST,
    LSP_RESPONSE,
    LSP_NOTIFICATION
} LSPMessageType;

// LSP Methods
typedef enum {
    LSP_INITIALIZE,
    LSP_INITIALIZED,
    LSP_SHUTDOWN,
    LSP_EXIT,
    LSP_TEXT_DOCUMENT_DID_OPEN,
    LSP_TEXT_DOCUMENT_DID_CHANGE,
    LSP_TEXT_DOCUMENT_DID_CLOSE,
    LSP_TEXT_DOCUMENT_COMPLETION,
    LSP_TEXT_DOCUMENT_DEFINITION,
    LSP_TEXT_DOCUMENT_HOVER,
    LSP_UNKNOWN
} LSPMethod;

// Position in a text document (0-indexed)
typedef struct {
    int line;
    int character;
} Position;

// Range in a text document
typedef struct {
    Position start;
    Position end;
} Range;

// Text document identifier
typedef struct {
    char* uri;  // file:///path/to/file.mlp
} TextDocumentIdentifier;

// Text document item
typedef struct {
    char* uri;
    char* languageId;  // "mlp"
    int version;
    char* text;
} TextDocumentItem;

// Completion item kinds
typedef enum {
    COMPLETION_TEXT = 1,
    COMPLETION_METHOD = 2,
    COMPLETION_FUNCTION = 3,
    COMPLETION_CONSTRUCTOR = 4,
    COMPLETION_FIELD = 5,
    COMPLETION_VARIABLE = 6,
    COMPLETION_CLASS = 7,
    COMPLETION_INTERFACE = 8,
    COMPLETION_MODULE = 9,
    COMPLETION_PROPERTY = 10,
    COMPLETION_UNIT = 11,
    COMPLETION_VALUE = 12,
    COMPLETION_ENUM = 13,
    COMPLETION_KEYWORD = 14,
    COMPLETION_SNIPPET = 15,
    COMPLETION_COLOR = 16,
    COMPLETION_FILE = 17,
    COMPLETION_REFERENCE = 18
} CompletionItemKind;

// Completion item
typedef struct {
    char* label;
    CompletionItemKind kind;
    char* detail;
    char* documentation;
    char* insertText;
} CompletionItem;

// Location (for go-to-definition)
typedef struct {
    char* uri;
    Range range;
} Location;

// Hover content
typedef struct {
    char* language;  // "mlp"
    char* value;     // code snippet
} MarkupContent;

typedef struct {
    MarkupContent contents;
    Range* range;  // optional
} Hover;

// Diagnostic severity
typedef enum {
    DIAGNOSTIC_ERROR = 1,
    DIAGNOSTIC_WARNING = 2,
    DIAGNOSTIC_INFORMATION = 3,
    DIAGNOSTIC_HINT = 4
} DiagnosticSeverity;

// Diagnostic (for error reporting)
typedef struct {
    Range range;
    DiagnosticSeverity severity;
    char* code;        // "E0001"
    char* message;
    char* source;      // "mlp"
} Diagnostic;

#endif // LSP_TYPES_H
