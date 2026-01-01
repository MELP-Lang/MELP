// json_rpc.h - JSON-RPC 2.0 Protocol Handler
// YZ_03 - LSP Server (Phase 1)
// Date: 1 Jan 2026

#ifndef JSON_RPC_H
#define JSON_RPC_H

#include <stddef.h>
#include <stdbool.h>

// JSON-RPC message types
typedef enum {
    JSONRPC_REQUEST,
    JSONRPC_RESPONSE,
    JSONRPC_NOTIFICATION,
    JSONRPC_ERROR
} JsonRpcMessageType;

// JSON-RPC message structure
typedef struct {
    JsonRpcMessageType type;
    char* id;              // NULL for notifications
    char* method;          // For requests/notifications
    char* params;          // JSON string
    char* result;          // For responses
    int error_code;        // For errors
    char* error_message;   // For errors
} JsonRpcMessage;

// Function prototypes
JsonRpcMessage* jsonrpc_parse(const char* input);
char* jsonrpc_create_response(const char* id, const char* result);
char* jsonrpc_create_error(const char* id, int code, const char* message);
char* jsonrpc_create_notification(const char* method, const char* params);
void jsonrpc_free_message(JsonRpcMessage* msg);

// Error codes (LSP standard)
#define JSONRPC_PARSE_ERROR      -32700
#define JSONRPC_INVALID_REQUEST  -32600
#define JSONRPC_METHOD_NOT_FOUND -32601
#define JSONRPC_INVALID_PARAMS   -32602
#define JSONRPC_INTERNAL_ERROR   -32603

#endif // JSON_RPC_H
