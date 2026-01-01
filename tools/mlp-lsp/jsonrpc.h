// JSON-RPC 2.0 Protocol Handler
// Handles JSON-RPC messages for LSP
#ifndef JSONRPC_H
#define JSONRPC_H

#include "cJSON.h"
#include "lsp_types.h"

// JSON-RPC Message
typedef struct {
    char* jsonrpc;       // "2.0"
    char* id;            // Request ID (can be NULL for notifications)
    char* method;        // Method name
    cJSON* params;       // Parameters (JSON object)
} JSONRPCRequest;

typedef struct {
    char* jsonrpc;       // "2.0"
    char* id;            // Request ID
    cJSON* result;       // Result (JSON object)
    cJSON* error;        // Error (JSON object, NULL if success)
} JSONRPCResponse;

// Parse JSON-RPC request from string
JSONRPCRequest* jsonrpc_parse_request(const char* json_str);

// Create JSON-RPC response
char* jsonrpc_create_response(const char* id, cJSON* result);

// Create JSON-RPC error response
char* jsonrpc_create_error(const char* id, int code, const char* message);

// Create JSON-RPC notification
char* jsonrpc_create_notification(const char* method, cJSON* params);

// Free request
void jsonrpc_free_request(JSONRPCRequest* request);

// Convert method string to enum
LSPMethod jsonrpc_method_from_string(const char* method);

// Read JSON-RPC message from stdin (Content-Length header protocol)
char* jsonrpc_read_message();

// Write JSON-RPC message to stdout (with Content-Length header)
void jsonrpc_write_message(const char* json_str);

#endif // JSONRPC_H
