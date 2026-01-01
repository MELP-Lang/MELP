// json_rpc.c - JSON-RPC 2.0 Protocol Handler
// YZ_03 - LSP Server (Phase 1)
// Date: 1 Jan 2026

#include "json_rpc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Simple JSON parser (minimal - for LSP messages)
// Uses cJSON library (borrowed from mlp-errors)

JsonRpcMessage* jsonrpc_parse(const char* input) {
    JsonRpcMessage* msg = calloc(1, sizeof(JsonRpcMessage));
    if (!msg) return NULL;
    
    // TODO: Implement actual JSON parsing
    // For now, stub implementation
    msg->type = JSONRPC_REQUEST;
    msg->method = strdup("initialize");
    msg->id = strdup("1");
    msg->params = strdup("{}");
    
    return msg;
}

char* jsonrpc_create_response(const char* id, const char* result) {
    // Allocate buffer for JSON response
    size_t len = strlen(id) + strlen(result) + 100;
    char* response = malloc(len);
    if (!response) return NULL;
    
    snprintf(response, len,
        "{\"jsonrpc\":\"2.0\",\"id\":\"%s\",\"result\":%s}",
        id, result);
    
    return response;
}

char* jsonrpc_create_error(const char* id, int code, const char* message) {
    size_t len = strlen(message) + 200;
    char* response = malloc(len);
    if (!response) return NULL;
    
    snprintf(response, len,
        "{\"jsonrpc\":\"2.0\",\"id\":\"%s\",\"error\":{\"code\":%d,\"message\":\"%s\"}}",
        id ? id : "null", code, message);
    
    return response;
}

char* jsonrpc_create_notification(const char* method, const char* params) {
    size_t len = strlen(method) + strlen(params) + 100;
    char* response = malloc(len);
    if (!response) return NULL;
    
    snprintf(response, len,
        "{\"jsonrpc\":\"2.0\",\"method\":\"%s\",\"params\":%s}",
        method, params);
    
    return response;
}

void jsonrpc_free_message(JsonRpcMessage* msg) {
    if (!msg) return;
    
    free(msg->id);
    free(msg->method);
    free(msg->params);
    free(msg->result);
    free(msg->error_message);
    free(msg);
}
