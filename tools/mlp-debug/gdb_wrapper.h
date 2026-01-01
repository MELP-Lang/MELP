#ifndef GDB_WRAPPER_H
#define GDB_WRAPPER_H

#include "source_mapper.h"
#include <stdbool.h>

// GDB session context
typedef struct {
    char* binary_path;        // Path to executable
    char* mlp_source;         // MLP source file
    SourceMap* source_map;    // Source mapping
    FILE* gdb_in;             // GDB stdin
    FILE* gdb_out;            // GDB stdout
    int gdb_pid;              // GDB process ID
    bool running;             // Is GDB running?
} GDBSession;

// Initialize GDB session
GDBSession* gdb_session_create(const char* binary_path, const char* mlp_source);

// Start GDB
bool gdb_start(GDBSession* session);

// Send command to GDB
bool gdb_send_command(GDBSession* session, const char* command);

// Read response from GDB
char* gdb_read_response(GDBSession* session);

// Set breakpoint at MLP line
bool gdb_breakpoint_mlp(GDBSession* session, int mlp_line);

// Run program
bool gdb_run(GDBSession* session);

// Step one line (MLP line)
bool gdb_step(GDBSession* session);

// Continue execution
bool gdb_continue(GDBSession* session);

// Print variable value
char* gdb_print_variable(GDBSession* session, const char* var_name);

// Get current MLP line
int gdb_current_mlp_line(GDBSession* session);

// Quit GDB
void gdb_quit(GDBSession* session);

// Free GDB session
void gdb_session_free(GDBSession* session);

#endif // GDB_WRAPPER_H
