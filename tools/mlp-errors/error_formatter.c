#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error_types.h"

// ANSI color codes
#define ANSI_RESET      "\033[0m"
#define ANSI_BOLD       "\033[1m"
#define ANSI_RED        "\033[31m"
#define ANSI_YELLOW     "\033[33m"
#define ANSI_BLUE       "\033[34m"
#define ANSI_CYAN       "\033[36m"

// Color helper functions
const char* get_severity_color(const char* severity) {
    if (strcmp(severity, "error") == 0) {
        return ANSI_RED;
    } else if (strcmp(severity, "warning") == 0) {
        return ANSI_YELLOW;
    }
    return ANSI_CYAN;  // info
}

const char* get_severity_label(const char* severity, const char* lang) {
    if (strcmp(severity, "error") == 0) {
        return (strcmp(lang, "tr") == 0) ? "Hata" : "Error";
    } else if (strcmp(severity, "warning") == 0) {
        return (strcmp(lang, "tr") == 0) ? "Uyarı" : "Warning";
    }
    return (strcmp(lang, "tr") == 0) ? "Bilgi" : "Info";
}

// Read source line from file
char* read_source_line(const char* file, int line_num) {
    FILE* fp = fopen(file, "r");
    if (!fp) return NULL;

    char buffer[MAX_SOURCE_LINE_LEN];
    int current_line = 1;

    while (fgets(buffer, sizeof(buffer), fp)) {
        if (current_line == line_num) {
            // Remove trailing newline
            size_t len = strlen(buffer);
            if (len > 0 && buffer[len - 1] == '\n') {
                buffer[len - 1] = '\0';
            }
            fclose(fp);
            char* result = strdup(buffer);
            return result;
        }
        current_line++;
    }

    fclose(fp);
    return NULL;
}

// Replace placeholders in message  
char* replace_placeholders(const char* template, ErrorContext* ctx) {
    if (!template) return "";
    
    static char buffer[MAX_MESSAGE_LEN];
    char result[MAX_MESSAGE_LEN];
    
    strncpy(result, template, MAX_MESSAGE_LEN - 1);
    result[MAX_MESSAGE_LEN - 1] = '\0';

    // Placeholder mapping
    const char* names[] = {"{var}", "{func}", "{type}", "{expected}", "{found}",
                          "{op}", "{module}", "{file}", "{import}", "{left}"};
    
    for (int i = 0; i < ctx->placeholder_count && i < 10; i++) {
        if (!ctx->placeholders[i]) continue;
        
        char* pos;
        while ((pos = strstr(result, names[i])) != NULL) {
            size_t name_len = strlen(names[i]);
            size_t value_len = strlen(ctx->placeholders[i]);
            size_t remaining = strlen(pos + name_len);
            
            // Check if we have enough space
            if (strlen(result) - name_len + value_len >= MAX_MESSAGE_LEN - 1) {
                break;  // Not enough space
            }
            
            // Move the rest of the string
            memmove(pos + value_len, pos + name_len, remaining + 1);
            // Copy the replacement
            memcpy(pos, ctx->placeholders[i], value_len);
        }
    }
    
    strncpy(buffer, result, MAX_MESSAGE_LEN - 1);
    buffer[MAX_MESSAGE_LEN - 1] = '\0';
    return buffer;
}

// Format error message with colors and source context
char* format_error(ErrorContext* ctx, const char* lang) {
    ErrorMessage* msg = get_error_message(ctx->error_code, lang);
    if (!msg) {
        fprintf(stderr, "Error: Unknown error code '%s'\n", ctx->error_code);
        return NULL;
    }

    static char output[MAX_MESSAGE_LEN * 2];
    char* out = output;
    int remaining = sizeof(output);

    const char* color = get_severity_color(msg->severity);
    const char* label = get_severity_label(msg->severity, lang);

    // Header: Error[E0001]: Title
    int written = snprintf(out, remaining, "%s%s%s[%s]: %s%s\n",
                          color, ANSI_BOLD, label, ctx->error_code,
                          replace_placeholders(msg->title, ctx), ANSI_RESET);
    out += written;
    remaining -= written;

    // Location: --> file:line:column
    written = snprintf(out, remaining, "  %s-->%s %s:%d:%d\n",
                      ANSI_BLUE, ANSI_RESET, ctx->file, ctx->line, ctx->column);
    out += written;
    remaining -= written;

    // Source line
    char* source_line = read_source_line(ctx->file, ctx->line);
    if (source_line) {
        // Line number and separator
        written = snprintf(out, remaining, "   %s|%s\n", ANSI_BLUE, ANSI_RESET);
        out += written;
        remaining -= written;

        // Source code line
        written = snprintf(out, remaining, "%s%2d%s %s|%s %s\n",
                          ANSI_BLUE, ctx->line, ANSI_RESET,
                          ANSI_BLUE, ANSI_RESET, source_line);
        out += written;
        remaining -= written;

        // Underline (^^^)
        written = snprintf(out, remaining, "   %s|%s ",
                          ANSI_BLUE, ANSI_RESET);
        out += written;
        remaining -= written;

        for (int i = 0; i < ctx->column - 1; i++) {
            written = snprintf(out, remaining, " ");
            out += written;
            remaining -= written;
        }

        written = snprintf(out, remaining, "%s^%s %s\n",
                          color, ANSI_RESET,
                          replace_placeholders(msg->message, ctx));
        out += written;
        remaining -= written;

        free(source_line);
    }

    // Separator
    written = snprintf(out, remaining, "   %s|%s\n", ANSI_BLUE, ANSI_RESET);
    out += written;
    remaining -= written;

    // Help message
    if (msg->help && strlen(msg->help) > 0) {
        written = snprintf(out, remaining, "   %s=%s %shelp:%s %s\n",
                          ANSI_CYAN, ANSI_RESET,
                          ANSI_BOLD, ANSI_RESET,
                          replace_placeholders(msg->help, ctx));
        out += written;
        remaining -= written;
    }

    free_error_message(msg);
    return output;
}
