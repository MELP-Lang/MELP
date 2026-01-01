// MLP Language Server (LSP)
// Entry point for the LSP server
// YZ_03 - LSP Server (Phase 1 - Complete Implementation)
// Date: 1 Jan 2026

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "lsp_protocol.h"
#include "jsonrpc.h"

int main(int argc, char** argv) {
    // Check for --stdio flag (optional, but standard for LSP)
    if (argc > 1 && strcmp(argv[1], "--stdio") != 0) {
        fprintf(stderr, "Usage: %s [--stdio]\n", argv[0]);
        fprintf(stderr, "MLP Language Server Protocol (LSP) Server\n");
        return 1;
    }

    // Initialize server state
    LSPServerState* server = lsp_server_init();
    if (!server) {
        fprintf(stderr, "Failed to initialize LSP server\n");
        return 1;
    }

    fprintf(stderr, "[LSP] MLP Language Server started\n");
    fprintf(stderr, "[LSP] Waiting for messages on stdin...\n");

    // Main message loop
    while (true) {
        // Read message from stdin
        char* message = jsonrpc_read_message();
        if (!message) {
            // EOF or error
            break;
        }

        fprintf(stderr, "[LSP] Received: %s\n", message);

        // Dispatch message
        char* response = lsp_dispatch_message(server, message);

        // Send response (if not a notification)
        if (response) {
            jsonrpc_write_message(response);
            fprintf(stderr, "[LSP] Sent: %s\n", response);
            free(response);
        }

        free(message);

        // Check if shutdown was requested
        if (server->shutdown_requested) {
            fprintf(stderr, "[LSP] Shutdown requested, waiting for exit...\n");
        }
    }

    fprintf(stderr, "[LSP] Server stopped\n");
    lsp_server_free(server);
    return 0;
}
