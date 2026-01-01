/* fmt_formatter.h - MLP Code Formatter */
#ifndef FMT_FORMATTER_H
#define FMT_FORMATTER_H

#include "fmt_types.h"

/* Create formatter */
Formatter* formatter_create(Token** tokens, int token_count, FormatOptions options);

/* Free formatter */
void formatter_free(Formatter* formatter);

/* Format tokens */
char* formatter_format(Formatter* fmt);

/* Helper functions */
void formatter_append(Formatter* fmt, const char* str);
void formatter_append_indent(Formatter* fmt);
void formatter_append_space(Formatter* fmt);
void formatter_append_newline(Formatter* fmt);

#endif /* FMT_FORMATTER_H */
