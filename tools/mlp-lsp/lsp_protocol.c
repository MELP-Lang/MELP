// LSP Protocol Handler Implementation
#include "lsp_protocol.h"
#include "mlp_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Document cache operations
static DocumentEntry* document_cache_find(LSPServerState* server, const char* uri);
static void document_cache_add(LSPServerState* server, const char* uri, const char* content, int version);
static void document_cache_update(LSPServerState* server, const char* uri, const char* content, int version);
static void document_cache_remove(LSPServerState* server, const char* uri);
static void document_cache_free_all(LSPServerState* server);

// Initialize LSP server state
LSPServerState* lsp_server_init() {
    LSPServerState* server = malloc(sizeof(LSPServerState));
    if (!server) return NULL;

    server->initialized = false;
    server->shutdown_requested = false;
    server->root_uri = NULL;
    server->workspace_folder = NULL;
    server->documents = NULL;

    return server;
}

// Free LSP server state
void lsp_server_free(LSPServerState* server) {
    if (!server) return;

    free(server->root_uri);
    free(server->workspace_folder);
    document_cache_free_all(server);
    free(server);
}

// Handle initialize request
char* lsp_handle_initialize(LSPServerState* server, JSONRPCRequest* request) {
    if (!server || !request) {
        return jsonrpc_create_error(request ? request->id : NULL, -32600, "Invalid Request");
    }

    // Extract rootUri from params
    if (request->params) {
        cJSON* rootUri = cJSON_GetObjectItem(request->params, "rootUri");
        if (rootUri && cJSON_IsString(rootUri)) {
            server->root_uri = strdup(cJSON_GetStringValue(rootUri));
        }

        cJSON* workspaceFolders = cJSON_GetObjectItem(request->params, "workspaceFolders");
        if (workspaceFolders && cJSON_IsArray(workspaceFolders)) {
            cJSON* first = cJSON_GetArrayItem(workspaceFolders, 0);
            if (first) {
                cJSON* uri = cJSON_GetObjectItem(first, "uri");
                if (uri && cJSON_IsString(uri)) {
                    server->workspace_folder = strdup(cJSON_GetStringValue(uri));
                }
            }
        }
    }

    server->initialized = true;

    // Build capabilities response
    cJSON* result = cJSON_CreateObject();
    cJSON* capabilities = cJSON_CreateObject();

    // Text document sync (1 = Full sync)
    cJSON_AddNumberToObject(capabilities, "textDocumentSync", 1);

    // Completion provider
    cJSON* completionProvider = cJSON_CreateObject();
    cJSON_AddBoolToObject(completionProvider, "resolveProvider", false);
    cJSON* triggerCharacters = cJSON_CreateArray();
    cJSON_AddItemToArray(triggerCharacters, cJSON_CreateString("."));
    cJSON_AddItemToArray(triggerCharacters, cJSON_CreateString(":"));
    cJSON_AddItemToObject(completionProvider, "triggerCharacters", triggerCharacters);
    cJSON_AddItemToObject(capabilities, "completionProvider", completionProvider);

    // Hover provider
    cJSON_AddBoolToObject(capabilities, "hoverProvider", true);

    // Definition provider
    cJSON_AddBoolToObject(capabilities, "definitionProvider", true);

    cJSON_AddItemToObject(result, "capabilities", capabilities);

    // Server info
    cJSON* serverInfo = cJSON_CreateObject();
    cJSON_AddStringToObject(serverInfo, "name", "mlp-lsp");
    cJSON_AddStringToObject(serverInfo, "version", "0.1.0");
    cJSON_AddItemToObject(result, "serverInfo", serverInfo);

    return jsonrpc_create_response(request->id, result);
}

// Handle initialized notification
char* lsp_handle_initialized(LSPServerState* server, JSONRPCRequest* request) {
    (void)server;
    (void)request;
    // This is a notification, no response needed
    return NULL;
}

// Handle shutdown request
char* lsp_handle_shutdown(LSPServerState* server, JSONRPCRequest* request) {
    if (!server || !request) {
        return jsonrpc_create_error(request ? request->id : NULL, -32600, "Invalid Request");
    }

    server->shutdown_requested = true;

    // Return null result
    cJSON* result = cJSON_CreateNull();
    return jsonrpc_create_response(request->id, result);
}

// Handle exit notification
void lsp_handle_exit(LSPServerState* server) {
    if (server && server->shutdown_requested) {
        exit(0);
    } else {
        exit(1);
    }
}

// Handle textDocument/didOpen notification
void lsp_handle_did_open(LSPServerState* server, JSONRPCRequest* request) {
    if (!server || !request || !request->params) return;

    cJSON* textDocument = cJSON_GetObjectItem(request->params, "textDocument");
    if (!textDocument) return;

    cJSON* uri = cJSON_GetObjectItem(textDocument, "uri");
    cJSON* text = cJSON_GetObjectItem(textDocument, "text");
    cJSON* version = cJSON_GetObjectItem(textDocument, "version");

    if (uri && text) {
        const char* uri_str = cJSON_GetStringValue(uri);
        const char* text_str = cJSON_GetStringValue(text);
        int version_num = version ? version->valueint : 1;
        
        // Cache document and parse
        document_cache_add(server, uri_str, text_str, version_num);
        
        fprintf(stderr, "[LSP] Document opened: %s (parsed %d symbols)\n", 
                uri_str, 
                document_cache_find(server, uri_str) ? 
                    document_cache_find(server, uri_str)->symbols->count : 0);
    }
}

// Handle textDocument/didChange notification
void lsp_handle_did_change(LSPServerState* server, JSONRPCRequest* request) {
    if (!server || !request || !request->params) return;

    cJSON* textDocument = cJSON_GetObjectItem(request->params, "textDocument");
    if (!textDocument) return;

    cJSON* uri = cJSON_GetObjectItem(textDocument, "uri");
    cJSON* version = cJSON_GetObjectItem(textDocument, "version");
    cJSON* contentChanges = cJSON_GetObjectItem(request->params, "contentChanges");

    if (uri && contentChanges && cJSON_IsArray(contentChanges)) {
        const char* uri_str = cJSON_GetStringValue(uri);
        int version_num = version ? version->valueint : 0;
        
        // Get first change (full document sync)
        cJSON* change = cJSON_GetArrayItem(contentChanges, 0);
        if (change) {
            cJSON* text = cJSON_GetObjectItem(change, "text");
            if (text) {
                document_cache_update(server, uri_str, cJSON_GetStringValue(text), version_num);
                fprintf(stderr, "[LSP] Document changed: %s\n", uri_str);
            }
        }
    }
}

// Handle textDocument/didClose notification
void lsp_handle_did_close(LSPServerState* server, JSONRPCRequest* request) {
    if (!server || !request || !request->params) return;

    cJSON* textDocument = cJSON_GetObjectItem(request->params, "textDocument");
    if (!textDocument) return;

    cJSON* uri = cJSON_GetObjectItem(textDocument, "uri");
    if (uri) {
        const char* uri_str = cJSON_GetStringValue(uri);
        document_cache_remove(server, uri_str);
        fprintf(stderr, "[LSP] Document closed: %s\n", uri_str);
    }
}

// Handle textDocument/completion request
char* lsp_handle_completion(LSPServerState* server, JSONRPCRequest* request) {
    if (!server || !request) {
        return jsonrpc_create_error(NULL, -32600, "Invalid Request");
    }

    cJSON* result = cJSON_CreateArray();

    // Get document
    cJSON* textDocument = cJSON_GetObjectItem(request->params, "textDocument");
    if (textDocument) {
        cJSON* uri = cJSON_GetObjectItem(textDocument, "uri");
        if (uri) {
            const char* uri_str = cJSON_GetStringValue(uri);
            DocumentEntry* doc = document_cache_find(server, uri_str);
            
            if (doc && doc->symbols) {
                // Add all symbols as completion items
                int count = 0;
                Symbol** symbols = symbol_table_get_all(doc->symbols, &count);
                
                for (int i = 0; i < count; i++) {
                    Symbol* sym = symbols[i];
                    
                    cJSON* item = cJSON_CreateObject();
                    cJSON_AddStringToObject(item, "label", sym->name);
                    cJSON_AddNumberToObject(item, "kind", symbol_kind_to_completion_kind(sym->kind));
                    
                    if (sym->signature) {
                        cJSON_AddStringToObject(item, "detail", sym->signature);
                    } else if (sym->type_name) {
                        cJSON_AddStringToObject(item, "detail", sym->type_name);
                    }
                    
                    if (sym->documentation) {
                        cJSON_AddStringToObject(item, "documentation", sym->documentation);
                    }
                    
                    cJSON_AddItemToArray(result, item);
                }
                
                free(symbols);
                fprintf(stderr, "[LSP] Completion: %d items\n", count);
            }
        }
    }

    return jsonrpc_create_response(request->id, result);
}

// Handle textDocument/definition request
char* lsp_handle_definition(LSPServerState* server, JSONRPCRequest* request) {
    if (!server || !request) {
        return jsonrpc_create_error(NULL, -32600, "Invalid Request");
    }

    cJSON* result = cJSON_CreateNull();

    // Get document and position
    cJSON* textDocument = cJSON_GetObjectItem(request->params, "textDocument");
    cJSON* position = cJSON_GetObjectItem(request->params, "position");
    
    if (textDocument && position) {
        cJSON* uri = cJSON_GetObjectItem(textDocument, "uri");
        cJSON* line = cJSON_GetObjectItem(position, "line");
        cJSON* character = cJSON_GetObjectItem(position, "character");
        
        if (uri && line && character) {
            const char* uri_str = cJSON_GetStringValue(uri);
            DocumentEntry* doc = document_cache_find(server, uri_str);
            
            if (doc && doc->symbols && doc->content) {
                // Get word at position
                int line_num = line->valueint;
                int char_pos = character->valueint;
                
                // Extract word at cursor position
                char word[256] = {0};
                const char* content = doc->content;
                int current_line = 0;
                int current_col = 0;
                int word_start = -1;
                int word_end = -1;
                
                for (int i = 0; content[i] != '\0'; i++) {
                    if (content[i] == '\n') {
                        current_line++;
                        current_col = 0;
                        continue;
                    }
                    
                    if (current_line == line_num) {
                        if (current_col == char_pos && (isalnum(content[i]) || content[i] == '_')) {
                            // Find word boundaries
                            word_start = i;
                            while (word_start > 0 && (isalnum(content[word_start-1]) || content[word_start-1] == '_')) {
                                word_start--;
                            }
                            word_end = i;
                            while (content[word_end] != '\0' && (isalnum(content[word_end]) || content[word_end] == '_')) {
                                word_end++;
                            }
                            
                            int len = word_end - word_start;
                            if (len > 0 && len < 256) {
                                strncpy(word, content + word_start, len);
                                word[len] = '\0';
                            }
                            break;
                        }
                    }
                    current_col++;
                }
                
                // Find symbol definition
                if (strlen(word) > 0) {
                    Symbol* symbol = symbol_table_find(doc->symbols, word);
                    if (symbol && symbol->file_uri) {
                        cJSON_Delete(result);
                        result = cJSON_CreateObject();
                        
                        cJSON_AddStringToObject(result, "uri", symbol->file_uri);
                        
                        cJSON* range = cJSON_CreateObject();
                        cJSON* start = cJSON_CreateObject();
                        cJSON_AddNumberToObject(start, "line", symbol->position.line);
                        cJSON_AddNumberToObject(start, "character", symbol->position.character);
                        cJSON_AddItemToObject(range, "start", start);
                        
                        cJSON* end = cJSON_CreateObject();
                        cJSON_AddNumberToObject(end, "line", symbol->position.line);
                        cJSON_AddNumberToObject(end, "character", symbol->position.character + (int)strlen(symbol->name));
                        cJSON_AddItemToObject(range, "end", end);
                        
                        cJSON_AddItemToObject(result, "range", range);
                        
                        fprintf(stderr, "[LSP] Definition found: %s at line %d\n", word, symbol->position.line);
                    }
                }
            }
        }
    }

    return jsonrpc_create_response(request->id, result);
}

// Handle textDocument/hover request
char* lsp_handle_hover(LSPServerState* server, JSONRPCRequest* request) {
    if (!server || !request) {
        return jsonrpc_create_error(NULL, -32600, "Invalid Request");
    }

    cJSON* result = cJSON_CreateNull();

    // Get document and position
    cJSON* textDocument = cJSON_GetObjectItem(request->params, "textDocument");
    cJSON* position = cJSON_GetObjectItem(request->params, "position");
    
    if (textDocument && position) {
        cJSON* uri = cJSON_GetObjectItem(textDocument, "uri");
        cJSON* line = cJSON_GetObjectItem(position, "line");
        cJSON* character = cJSON_GetObjectItem(position, "character");
        
        if (uri && line && character) {
            const char* uri_str = cJSON_GetStringValue(uri);
            DocumentEntry* doc = document_cache_find(server, uri_str);
            
            if (doc && doc->symbols && doc->content) {
                // Get word at position (same logic as definition)
                int line_num = line->valueint;
                int char_pos = character->valueint;
                
                char word[256] = {0};
                const char* content = doc->content;
                int current_line = 0;
                int current_col = 0;
                
                for (int i = 0; content[i] != '\0'; i++) {
                    if (content[i] == '\n') {
                        current_line++;
                        current_col = 0;
                        continue;
                    }
                    
                    if (current_line == line_num) {
                        if (current_col == char_pos && (isalnum(content[i]) || content[i] == '_')) {
                            int word_start = i;
                            while (word_start > 0 && (isalnum(content[word_start-1]) || content[word_start-1] == '_')) {
                                word_start--;
                            }
                            int word_end = i;
                            while (content[word_end] != '\0' && (isalnum(content[word_end]) || content[word_end] == '_')) {
                                word_end++;
                            }
                            
                            int len = word_end - word_start;
                            if (len > 0 && len < 256) {
                                strncpy(word, content + word_start, len);
                                word[len] = '\0';
                            }
                            break;
                        }
                    }
                    current_col++;
                }
                
                // Find symbol and create hover info
                if (strlen(word) > 0) {
                    Symbol* symbol = symbol_table_find(doc->symbols, word);
                    if (symbol) {
                        cJSON_Delete(result);
                        result = cJSON_CreateObject();
                        
                        // Create hover content
                        char hover_text[512];
                        if (symbol->signature) {
                            snprintf(hover_text, sizeof(hover_text), "```mlp\n%s\n```", symbol->signature);
                        } else if (symbol->type_name) {
                            snprintf(hover_text, sizeof(hover_text), "```mlp\n%s: %s\n```", symbol->name, symbol->type_name);
                        } else {
                            snprintf(hover_text, sizeof(hover_text), "```mlp\n%s\n```\n%s", 
                                    symbol->name, symbol_kind_to_string(symbol->kind));
                        }
                        
                        if (symbol->documentation) {
                            strcat(hover_text, "\n\n");
                            strcat(hover_text, symbol->documentation);
                        }
                        
                        cJSON* contents = cJSON_CreateObject();
                        cJSON_AddStringToObject(contents, "kind", "markdown");
                        cJSON_AddStringToObject(contents, "value", hover_text);
                        cJSON_AddItemToObject(result, "contents", contents);
                        
                        fprintf(stderr, "[LSP] Hover info for: %s\n", word);
                    }
                }
            }
        }
    }

    return jsonrpc_create_response(request->id, result);
}

// Main message dispatch
char* lsp_dispatch_message(LSPServerState* server, const char* message) {
    if (!server || !message) {
        return jsonrpc_create_error(NULL, -32600, "Invalid Request");
    }

    // Parse JSON-RPC request
    JSONRPCRequest* request = jsonrpc_parse_request(message);
    if (!request) {
        return jsonrpc_create_error(NULL, -32700, "Parse error");
    }

    // Dispatch based on method
    LSPMethod method = jsonrpc_method_from_string(request->method);
    char* response = NULL;

    switch (method) {
        case LSP_INITIALIZE:
            response = lsp_handle_initialize(server, request);
            break;
        
        case LSP_INITIALIZED:
            response = lsp_handle_initialized(server, request);
            break;
        
        case LSP_SHUTDOWN:
            response = lsp_handle_shutdown(server, request);
            break;
        
        case LSP_EXIT:
            lsp_handle_exit(server);
            break;
        
        case LSP_TEXT_DOCUMENT_DID_OPEN:
            lsp_handle_did_open(server, request);
            break;
        
        case LSP_TEXT_DOCUMENT_DID_CHANGE:
            lsp_handle_did_change(server, request);
            break;
        
        case LSP_TEXT_DOCUMENT_DID_CLOSE:
            lsp_handle_did_close(server, request);
            break;
        
        case LSP_TEXT_DOCUMENT_COMPLETION:
            response = lsp_handle_completion(server, request);
            break;
        
        case LSP_TEXT_DOCUMENT_DEFINITION:
            response = lsp_handle_definition(server, request);
            break;
        
        case LSP_TEXT_DOCUMENT_HOVER:
            response = lsp_handle_hover(server, request);
            break;
        
        case LSP_UNKNOWN:
        default:
            response = jsonrpc_create_error(request->id, -32601, "Method not found");
            break;
    }

    jsonrpc_free_request(request);
    return response;
}

// Document cache operations

static DocumentEntry* document_cache_find(LSPServerState* server, const char* uri) {
    if (!server || !uri) return NULL;
    
    DocumentEntry* current = server->documents;
    while (current) {
        if (strcmp(current->uri, uri) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

static void document_cache_add(LSPServerState* server, const char* uri, const char* content, int version) {
    if (!server || !uri || !content) return;
    
    // Check if already exists
    DocumentEntry* existing = document_cache_find(server, uri);
    if (existing) {
        document_cache_update(server, uri, content, version);
        return;
    }
    
    // Create new entry
    DocumentEntry* entry = malloc(sizeof(DocumentEntry));
    if (!entry) return;
    
    entry->uri = strdup(uri);
    entry->content = strdup(content);
    entry->version = version;
    entry->next = server->documents;
    
    // Parse document
    Parser* parser = parser_create(content, uri);
    if (parser) {
        parser_parse(parser);
        entry->symbols = parser_get_symbols(parser);
        parser_free(parser);
    } else {
        entry->symbols = NULL;
    }
    
    server->documents = entry;
}

static void document_cache_update(LSPServerState* server, const char* uri, const char* content, int version) {
    if (!server || !uri || !content) return;
    
    DocumentEntry* entry = document_cache_find(server, uri);
    if (!entry) {
        document_cache_add(server, uri, content, version);
        return;
    }
    
    // Update content
    free(entry->content);
    entry->content = strdup(content);
    entry->version = version;
    
    // Re-parse
    if (entry->symbols) {
        symbol_table_free(entry->symbols);
    }
    
    Parser* parser = parser_create(content, uri);
    if (parser) {
        parser_parse(parser);
        entry->symbols = parser_get_symbols(parser);
        parser_free(parser);
    } else {
        entry->symbols = NULL;
    }
}

static void document_cache_remove(LSPServerState* server, const char* uri) {
    if (!server || !uri) return;
    
    DocumentEntry** prev_next = &server->documents;
    DocumentEntry* current = server->documents;
    
    while (current) {
        if (strcmp(current->uri, uri) == 0) {
            *prev_next = current->next;
            
            free(current->uri);
            free(current->content);
            if (current->symbols) {
                symbol_table_free(current->symbols);
            }
            free(current);
            return;
        }
        
        prev_next = &current->next;
        current = current->next;
    }
}

static void document_cache_free_all(LSPServerState* server) {
    if (!server) return;
    
    DocumentEntry* current = server->documents;
    while (current) {
        DocumentEntry* next = current->next;
        
        free(current->uri);
        free(current->content);
        if (current->symbols) {
            symbol_table_free(current->symbols);
        }
        free(current);
        
        current = next;
    }
    
    server->documents = NULL;
}
