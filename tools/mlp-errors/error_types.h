#ifndef ERROR_TYPES_H
#define ERROR_TYPES_H

#define MAX_PLACEHOLDERS 10
#define MAX_PLACEHOLDER_LEN 256
#define MAX_MESSAGE_LEN 4096
#define MAX_SOURCE_LINE_LEN 1024

typedef struct {
    char* error_code;           // E0001, W0001, etc.
    char* file;                 // Source file path
    int line;                   // Line number
    int column;                 // Column number
    char* placeholders[MAX_PLACEHOLDERS];  // {var}, {func}, etc.
    int placeholder_count;
} ErrorContext;

typedef struct {
    char* title;
    char* message;
    char* help;
    char* example;
    char* severity;             // error, warning, info
} ErrorMessage;

// Function prototypes
int load_error_database(const char* json_path);
ErrorMessage* get_error_message(const char* code, const char* lang);
char* format_error(ErrorContext* ctx, const char* lang);
void free_error_message(ErrorMessage* msg);
void cleanup_error_database(void);

#endif // ERROR_TYPES_H
