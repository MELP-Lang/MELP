// JSON-RPC 2.0 Protocol Handler Implementation
#include "jsonrpc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// strdup is not standard C99, define if needed
#ifndef _GNU_SOURCE
char* strdup(const char* s) {
    size_t len = strlen(s) + 1;
    char* copy = malloc(len);
    if (copy) {
        memcpy(copy, s, len);
    }
    return copy;
}
#endif

// Parse JSON-RPC request from string
JSONRPCRequest* jsonrpc_parse_request(const char* json_str) {
    cJSON* root = cJSON_Parse(json_str);
    if (!root) {
        return NULL;
    }

    JSONRPCRequest* request = malloc(sizeof(JSONRPCRequest));
    if (!request) {
        cJSON_Delete(root);
        return NULL;
    }

    // Parse jsonrpc version
    cJSON* jsonrpc = cJSON_GetObjectItem(root, "jsonrpc");
    request->jsonrpc = jsonrpc ? strdup(cJSON_GetStringValue(jsonrpc)) : strdup("2.0");

    // Parse id (can be NULL for notifications)
    cJSON* id = cJSON_GetObjectItem(root, "id");
    if (id) {
        if (cJSON_IsNumber(id)) {
            char id_str[32];
            snprintf(id_str, sizeof(id_str), "%d", id->valueint);
            request->id = strdup(id_str);
        } else {
            request->id = strdup(cJSON_GetStringValue(id));
        }
    } else {
        request->id = NULL;
    }

    // Parse method
    cJSON* method = cJSON_GetObjectItem(root, "method");
    request->method = method ? strdup(cJSON_GetStringValue(method)) : NULL;

    // Parse params (keep as cJSON object)
    cJSON* params = cJSON_GetObjectItem(root, "params");
    request->params = params ? cJSON_Duplicate(params, 1) : NULL;

    cJSON_Delete(root);
    return request;
}

// Create JSON-RPC response
char* jsonrpc_create_response(const char* id, cJSON* result) {
    cJSON* response = cJSON_CreateObject();
    cJSON_AddStringToObject(response, "jsonrpc", "2.0");
    
    // Add id (handle both string and number)
    if (id) {
        // Try to parse as number
        char* endptr;
        long id_num = strtol(id, &endptr, 10);
        if (*endptr == '\0') {
            cJSON_AddNumberToObject(response, "id", id_num);
        } else {
            cJSON_AddStringToObject(response, "id", id);
        }
    } else {
        cJSON_AddNullToObject(response, "id");
    }
    
    cJSON_AddItemToObject(response, "result", result ? result : cJSON_CreateNull());

    char* json_str = cJSON_PrintUnformatted(response);
    cJSON_Delete(response);
    return json_str;
}

// Create JSON-RPC error response
char* jsonrpc_create_error(const char* id, int code, const char* message) {
    cJSON* response = cJSON_CreateObject();
    cJSON_AddStringToObject(response, "jsonrpc", "2.0");
    
    if (id) {
        cJSON_AddStringToObject(response, "id", id);
    } else {
        cJSON_AddNullToObject(response, "id");
    }

    cJSON* error = cJSON_CreateObject();
    cJSON_AddNumberToObject(error, "code", code);
    cJSON_AddStringToObject(error, "message", message);
    cJSON_AddItemToObject(response, "error", error);

    char* json_str = cJSON_PrintUnformatted(response);
    cJSON_Delete(response);
    return json_str;
}

// Create JSON-RPC notification
char* jsonrpc_create_notification(const char* method, cJSON* params) {
    cJSON* notification = cJSON_CreateObject();
    cJSON_AddStringToObject(notification, "jsonrpc", "2.0");
    cJSON_AddStringToObject(notification, "method", method);
    if (params) {
        cJSON_AddItemToObject(notification, "params", params);
    }

    char* json_str = cJSON_PrintUnformatted(notification);
    cJSON_Delete(notification);
    return json_str;
}

// Free request
void jsonrpc_free_request(JSONRPCRequest* request) {
    if (!request) return;
    
    free(request->jsonrpc);
    free(request->id);
    free(request->method);
    if (request->params) {
        cJSON_Delete(request->params);
    }
    free(request);
}

// Convert method string to enum
LSPMethod jsonrpc_method_from_string(const char* method) {
    if (!method) return LSP_UNKNOWN;
    
    if (strcmp(method, "initialize") == 0) return LSP_INITIALIZE;
    if (strcmp(method, "initialized") == 0) return LSP_INITIALIZED;
    if (strcmp(method, "shutdown") == 0) return LSP_SHUTDOWN;
    if (strcmp(method, "exit") == 0) return LSP_EXIT;
    if (strcmp(method, "textDocument/didOpen") == 0) return LSP_TEXT_DOCUMENT_DID_OPEN;
    if (strcmp(method, "textDocument/didChange") == 0) return LSP_TEXT_DOCUMENT_DID_CHANGE;
    if (strcmp(method, "textDocument/didClose") == 0) return LSP_TEXT_DOCUMENT_DID_CLOSE;
    if (strcmp(method, "textDocument/completion") == 0) return LSP_TEXT_DOCUMENT_COMPLETION;
    if (strcmp(method, "textDocument/definition") == 0) return LSP_TEXT_DOCUMENT_DEFINITION;
    if (strcmp(method, "textDocument/hover") == 0) return LSP_TEXT_DOCUMENT_HOVER;
    
    return LSP_UNKNOWN;
}

// Read JSON-RPC message from stdin (Content-Length header protocol)
char* jsonrpc_read_message() {
    char header[256];
    int content_length = 0;

    // Read headers until we find Content-Length
    while (fgets(header, sizeof(header), stdin)) {
        if (strncmp(header, "Content-Length: ", 16) == 0) {
            content_length = atoi(header + 16);
        }
        // Empty line marks end of headers
        if (strcmp(header, "\r\n") == 0 || strcmp(header, "\n") == 0) {
            break;
        }
    }

    if (content_length == 0) {
        return NULL;
    }

    // Read content
    char* content = malloc(content_length + 1);
    if (!content) {
        return NULL;
    }

    size_t bytes_read = fread(content, 1, content_length, stdin);
    if (bytes_read != (size_t)content_length) {
        free(content);
        return NULL;
    }

    content[content_length] = '\0';
    return content;
}

// Write JSON-RPC message to stdout (with Content-Length header)
void jsonrpc_write_message(const char* json_str) {
    if (!json_str) return;

    size_t content_length = strlen(json_str);
    fprintf(stdout, "Content-Length: %zu\r\n\r\n%s", content_length, json_str);
    fflush(stdout);
}
