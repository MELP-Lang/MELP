// LSP Protocol Handler
// Implements Language Server Protocol methods
#ifndef LSP_PROTOCOL_H
#define LSP_PROTOCOL_H

#include "jsonrpc.h"
#include "lsp_types.h"
#include "symbol_table.h"

// Document cache entry
typedef struct DocumentEntry {
    char* uri;
    char* content;
    int version;
    SymbolTable* symbols;
    struct DocumentEntry* next;
} DocumentEntry;

// LSP Server state
typedef struct {
    bool initialized;
    bool shutdown_requested;
    char* root_uri;
    char* workspace_folder;
    DocumentEntry* documents;  // Cached documents
} LSPServerState;

// Initialize LSP server state
LSPServerState* lsp_server_init();

// Free LSP server state
void lsp_server_free(LSPServerState* server);

// Handle LSP requests
char* lsp_handle_initialize(LSPServerState* server, JSONRPCRequest* request);
char* lsp_handle_initialized(LSPServerState* server, JSONRPCRequest* request);
char* lsp_handle_shutdown(LSPServerState* server, JSONRPCRequest* request);
void lsp_handle_exit(LSPServerState* server);

// Handle text document notifications
void lsp_handle_did_open(LSPServerState* server, JSONRPCRequest* request);
void lsp_handle_did_change(LSPServerState* server, JSONRPCRequest* request);
void lsp_handle_did_close(LSPServerState* server, JSONRPCRequest* request);

// Handle text document requests
char* lsp_handle_completion(LSPServerState* server, JSONRPCRequest* request);
char* lsp_handle_definition(LSPServerState* server, JSONRPCRequest* request);
char* lsp_handle_hover(LSPServerState* server, JSONRPCRequest* request);

// Main message dispatch
char* lsp_dispatch_message(LSPServerState* server, const char* message);

#endif // LSP_PROTOCOL_H
