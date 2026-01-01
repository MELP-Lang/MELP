#include "gdb_wrapper.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

// Initialize GDB session
GDBSession* gdb_session_create(const char* binary_path, const char* mlp_source) {
    GDBSession* session = (GDBSession*)malloc(sizeof(GDBSession));
    if (!session) return NULL;
    
    session->binary_path = strdup(binary_path);
    session->mlp_source = strdup(mlp_source);
    session->gdb_in = NULL;
    session->gdb_out = NULL;
    session->gdb_pid = -1;
    session->running = false;
    
    // Load or create source map
    // First try to load from C file
    char c_file[512];
    snprintf(c_file, sizeof(c_file), "%s.c", binary_path);
    
    session->source_map = source_map_load_from_file(c_file);
    if (!session->source_map) {
        // Create empty map
        session->source_map = source_map_create(mlp_source, c_file);
    }
    
    return session;
}

// Start GDB
bool gdb_start(GDBSession* session) {
    if (!session || session->running) return false;
    
    int pipe_in[2];   // Parent -> GDB
    int pipe_out[2];  // GDB -> Parent
    
    if (pipe(pipe_in) < 0 || pipe(pipe_out) < 0) {
        return false;
    }
    
    pid_t pid = fork();
    if (pid < 0) {
        close(pipe_in[0]); close(pipe_in[1]);
        close(pipe_out[0]); close(pipe_out[1]);
        return false;
    }
    
    if (pid == 0) {
        // Child process: exec GDB
        dup2(pipe_in[0], STDIN_FILENO);
        dup2(pipe_out[1], STDOUT_FILENO);
        dup2(pipe_out[1], STDERR_FILENO);
        
        close(pipe_in[0]); close(pipe_in[1]);
        close(pipe_out[0]); close(pipe_out[1]);
        
        execlp("gdb", "gdb", "-q", "-interpreter=mi", session->binary_path, NULL);
        exit(1); // If exec fails
    }
    
    // Parent process
    close(pipe_in[0]);
    close(pipe_out[1]);
    
    session->gdb_in = fdopen(pipe_in[1], "w");
    session->gdb_out = fdopen(pipe_out[0], "r");
    session->gdb_pid = pid;
    session->running = true;
    
    return true;
}

// Send command to GDB
bool gdb_send_command(GDBSession* session, const char* command) {
    if (!session || !session->running || !session->gdb_in) {
        return false;
    }
    
    fprintf(session->gdb_in, "%s\n", command);
    fflush(session->gdb_in);
    return true;
}

// Read response from GDB (simplified - reads until prompt)
char* gdb_read_response(GDBSession* session) {
    if (!session || !session->running || !session->gdb_out) {
        return NULL;
    }
    
    static char buffer[4096];
    char line[512];
    buffer[0] = '\0';
    
    while (fgets(line, sizeof(line), session->gdb_out)) {
        strncat(buffer, line, sizeof(buffer) - strlen(buffer) - 1);
        
        // Check for GDB prompt (simplified)
        if (strstr(line, "(gdb)") || strstr(line, "^done") || strstr(line, "^running")) {
            break;
        }
    }
    
    return buffer;
}

// Set breakpoint at MLP line
bool gdb_breakpoint_mlp(GDBSession* session, int mlp_line) {
    if (!session || !session->source_map) return false;
    
    // Convert MLP line to C line
    int c_line = source_map_mlp_to_c(session->source_map, mlp_line);
    if (c_line < 0) {
        fprintf(stderr, "Warning: Cannot map MLP line %d to C line\n", mlp_line);
        c_line = mlp_line; // Fallback: assume 1:1 mapping
    }
    
    // Get C file name
    char c_file[512];
    snprintf(c_file, sizeof(c_file), "%s.c", session->binary_path);
    
    // Send breakpoint command to GDB
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "break %s:%d", c_file, c_line);
    
    printf("Setting breakpoint at MLP line %d (C line %d)\n", mlp_line, c_line);
    return gdb_send_command(session, cmd);
}

// Run program
bool gdb_run(GDBSession* session) {
    if (!session) return false;
    
    printf("Starting program execution...\n");
    return gdb_send_command(session, "run");
}

// Step one line (MLP line)
bool gdb_step(GDBSession* session) {
    if (!session) return false;
    
    return gdb_send_command(session, "step");
}

// Continue execution
bool gdb_continue(GDBSession* session) {
    if (!session) return false;
    
    printf("Continuing execution...\n");
    return gdb_send_command(session, "continue");
}

// Print variable value
char* gdb_print_variable(GDBSession* session, const char* var_name) {
    if (!session || !var_name) return NULL;
    
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "print %s", var_name);
    
    if (!gdb_send_command(session, cmd)) {
        return NULL;
    }
    
    return gdb_read_response(session);
}

// Get current MLP line (parse GDB output)
int gdb_current_mlp_line(GDBSession* session) {
    if (!session || !session->source_map) return -1;
    
    // Send "where" command to get current location
    if (!gdb_send_command(session, "where")) {
        return -1;
    }
    
    char* response = gdb_read_response(session);
    if (!response) return -1;
    
    // Parse response to extract C line number
    // Look for pattern like: at file.c:123
    char* at = strstr(response, " at ");
    if (at) {
        char* colon = strchr(at, ':');
        if (colon) {
            int c_line = atoi(colon + 1);
            // Convert C line to MLP line
            return source_map_c_to_mlp(session->source_map, c_line);
        }
    }
    
    return -1;
}

// Quit GDB
void gdb_quit(GDBSession* session) {
    if (!session || !session->running) return;
    
    gdb_send_command(session, "quit");
    
    if (session->gdb_in) {
        fclose(session->gdb_in);
        session->gdb_in = NULL;
    }
    
    if (session->gdb_out) {
        fclose(session->gdb_out);
        session->gdb_out = NULL;
    }
    
    if (session->gdb_pid > 0) {
        // Wait for GDB to terminate
        waitpid(session->gdb_pid, NULL, 0);
        session->gdb_pid = -1;
    }
    
    session->running = false;
}

// Free GDB session
void gdb_session_free(GDBSession* session) {
    if (!session) return;
    
    if (session->running) {
        gdb_quit(session);
    }
    
    free(session->binary_path);
    free(session->mlp_source);
    
    if (session->source_map) {
        source_map_free(session->source_map);
    }
    
    free(session);
}

// Interactive debugger commands
void gdb_interactive_loop(GDBSession* session) {
    if (!session) return;
    
    printf("=== MLP Debugger (GDB Wrapper) ===\n");
    printf("Commands: break <line>, run, step, continue, print <var>, quit\n\n");
    
    char command[256];
    
    while (1) {
        printf("(mlp-debug) ");
        fflush(stdout);
        
        if (!fgets(command, sizeof(command), stdin)) {
            break;
        }
        
        // Remove newline
        command[strcspn(command, "\n")] = 0;
        
        if (strlen(command) == 0) continue;
        
        // Parse command
        if (strncmp(command, "break ", 6) == 0) {
            int line = atoi(command + 6);
            gdb_breakpoint_mlp(session, line);
            gdb_read_response(session);
        }
        else if (strcmp(command, "run") == 0) {
            gdb_run(session);
            gdb_read_response(session);
        }
        else if (strcmp(command, "step") == 0 || strcmp(command, "s") == 0) {
            gdb_step(session);
            char* resp = gdb_read_response(session);
            printf("%s\n", resp);
            int mlp_line = gdb_current_mlp_line(session);
            if (mlp_line >= 0) {
                printf("Now at MLP line %d\n", mlp_line);
            }
        }
        else if (strcmp(command, "continue") == 0 || strcmp(command, "c") == 0) {
            gdb_continue(session);
            gdb_read_response(session);
        }
        else if (strncmp(command, "print ", 6) == 0 || strncmp(command, "p ", 2) == 0) {
            const char* var = (command[0] == 'p') ? command + 2 : command + 6;
            char* value = gdb_print_variable(session, var);
            if (value) {
                printf("%s\n", value);
            }
        }
        else if (strcmp(command, "quit") == 0 || strcmp(command, "q") == 0) {
            break;
        }
        else if (strcmp(command, "help") == 0 || strcmp(command, "h") == 0) {
            printf("Commands:\n");
            printf("  break <line> - Set breakpoint at MLP line\n");
            printf("  run          - Start program execution\n");
            printf("  step (s)     - Step one line\n");
            printf("  continue (c) - Continue execution\n");
            printf("  print <var>  - Print variable value\n");
            printf("  quit (q)     - Exit debugger\n");
        }
        else {
            printf("Unknown command: %s (type 'help' for commands)\n", command);
        }
    }
    
    printf("Exiting debugger...\n");
}
