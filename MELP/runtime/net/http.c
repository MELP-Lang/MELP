/**
 * MLP Standard Library - HTTP Client Implementation
 * 
 * HTTP/1.1 client built on POSIX sockets
 * Supports GET, POST, PUT, DELETE with headers and body
 * 
 * YZ_01 - TODO #2 Task 1: Networking Library
 */

#define _GNU_SOURCE  // For strcasestr
#include "http.h"
#include "socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// For strdup on older systems
#if !defined(_POSIX_C_SOURCE) || _POSIX_C_SOURCE < 200809L
char* strdup(const char* s) {
    if (!s) return NULL;
    size_t len = strlen(s) + 1;
    char* copy = malloc(len);
    if (copy) memcpy(copy, s, len);
    return copy;
}
#endif

// ============================================================================
// URL Parsing
// ============================================================================

typedef struct {
    char* scheme;   // "http" or "https"
    char* host;     // hostname or IP
    int64_t port;   // port number
    char* path;     // path (starts with /)
    char* query;    // query string (after ?)
} ParsedURL;

static void free_parsed_url(ParsedURL* url) {
    if (!url) return;
    free(url->scheme);
    free(url->host);
    free(url->path);
    free(url->query);
}

static ParsedURL* parse_url_internal(const char* url) {
    if (!url) return NULL;
    
    ParsedURL* parsed = (ParsedURL*)calloc(1, sizeof(ParsedURL));
    if (!parsed) return NULL;
    
    const char* p = url;
    
    // Parse scheme (http:// or https://)
    const char* scheme_end = strstr(p, "://");
    if (!scheme_end) {
        free(parsed);
        return NULL;
    }
    
    size_t scheme_len = scheme_end - p;
    parsed->scheme = (char*)malloc(scheme_len + 1);
    memcpy(parsed->scheme, p, scheme_len);
    parsed->scheme[scheme_len] = '\0';
    
    p = scheme_end + 3;  // Skip "://"
    
    // Parse host and optional port
    const char* path_start = strchr(p, '/');
    const char* query_start = strchr(p, '?');
    const char* host_end = path_start ? path_start : (query_start ? query_start : p + strlen(p));
    
    // Check for port
    const char* colon = strchr(p, ':');
    if (colon && colon < host_end) {
        // Host with port
        size_t host_len = colon - p;
        parsed->host = (char*)malloc(host_len + 1);
        memcpy(parsed->host, p, host_len);
        parsed->host[host_len] = '\0';
        
        // Parse port
        parsed->port = atoi(colon + 1);
    } else {
        // Host without port (use default)
        size_t host_len = host_end - p;
        parsed->host = (char*)malloc(host_len + 1);
        memcpy(parsed->host, p, host_len);
        parsed->host[host_len] = '\0';
        
        // Default ports
        if (strcmp(parsed->scheme, "https") == 0) {
            parsed->port = 443;
        } else {
            parsed->port = 80;
        }
    }
    
    // Parse path
    if (path_start) {
        const char* path_end = query_start ? query_start : path_start + strlen(path_start);
        size_t path_len = path_end - path_start;
        parsed->path = (char*)malloc(path_len + 1);
        memcpy(parsed->path, path_start, path_len);
        parsed->path[path_len] = '\0';
    } else {
        parsed->path = strdup("/");
    }
    
    // Parse query
    if (query_start) {
        parsed->query = strdup(query_start + 1);
    } else {
        parsed->query = strdup("");
    }
    
    return parsed;
}

char* mlp_http_parse_url(const char* url) {
    ParsedURL* parsed = parse_url_internal(url);
    if (!parsed) return strdup("");
    
    // Build JSON response
    char buffer[2048];
    snprintf(buffer, sizeof(buffer),
             "{\"scheme\":\"%s\",\"host\":\"%s\",\"port\":%lld,\"path\":\"%s\",\"query\":\"%s\"}",
             parsed->scheme, parsed->host, (long long)parsed->port, parsed->path, parsed->query);
    
    free_parsed_url(parsed);
    free(parsed);
    
    return strdup(buffer);
}

// ============================================================================
// URL Encoding/Decoding
// ============================================================================

char* mlp_http_url_encode(const char* str) {
    if (!str) return strdup("");
    
    size_t len = strlen(str);
    char* encoded = (char*)malloc(len * 3 + 1);  // Worst case: all chars encoded
    if (!encoded) return strdup("");
    
    char* p = encoded;
    for (size_t i = 0; i < len; i++) {
        unsigned char c = str[i];
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            *p++ = c;
        } else if (c == ' ') {
            *p++ = '+';
        } else {
            sprintf(p, "%%%02X", c);
            p += 3;
        }
    }
    *p = '\0';
    
    return encoded;
}

char* mlp_http_url_decode(const char* str) {
    if (!str) return strdup("");
    
    size_t len = strlen(str);
    char* decoded = (char*)malloc(len + 1);
    if (!decoded) return strdup("");
    
    char* p = decoded;
    for (size_t i = 0; i < len; i++) {
        if (str[i] == '+') {
            *p++ = ' ';
        } else if (str[i] == '%' && i + 2 < len) {
            int value;
            sscanf(&str[i + 1], "%2x", &value);
            *p++ = (char)value;
            i += 2;
        } else {
            *p++ = str[i];
        }
    }
    *p = '\0';
    
    return decoded;
}

// ============================================================================
// HTTP Request Building
// ============================================================================

char* mlp_http_build_request(const char* method, const char* path,
                              const char* host, const char* headers,
                              const char* body) {
    if (!method || !path || !host) return strdup("");
    
    size_t body_len = body ? strlen(body) : 0;
    size_t buffer_size = 1024 + strlen(method) + strlen(path) + strlen(host) + 
                         (headers ? strlen(headers) : 0) + body_len;
    
    char* request = (char*)malloc(buffer_size);
    if (!request) return strdup("");
    
    // Request line
    int offset = snprintf(request, buffer_size, "%s %s HTTP/1.1\r\n", method, path);
    
    // Host header (required in HTTP/1.1)
    offset += snprintf(request + offset, buffer_size - offset, "Host: %s\r\n", host);
    
    // Connection header
    offset += snprintf(request + offset, buffer_size - offset, "Connection: close\r\n");
    
    // Content-Length for POST/PUT
    if (body_len > 0) {
        offset += snprintf(request + offset, buffer_size - offset, 
                          "Content-Length: %zu\r\n", body_len);
    }
    
    // Additional headers
    if (headers && strlen(headers) > 0) {
        offset += snprintf(request + offset, buffer_size - offset, "%s", headers);
    }
    
    // End of headers
    offset += snprintf(request + offset, buffer_size - offset, "\r\n");
    
    // Body
    if (body_len > 0) {
        memcpy(request + offset, body, body_len);
        offset += body_len;
        request[offset] = '\0';
    }
    
    return request;
}

// ============================================================================
// HTTP Response Parsing
// ============================================================================

char* mlp_http_parse_response(const char* response) {
    if (!response || strlen(response) == 0) return strdup("");
    
    // Extract status code
    int status = -1;
    const char* status_line_end = strstr(response, "\r\n");
    if (status_line_end) {
        sscanf(response, "HTTP/%*d.%*d %d", &status);
    }
    
    // Find headers/body separator
    const char* body_start = strstr(response, "\r\n\r\n");
    const char* body = "";
    char* headers_copy = NULL;
    
    if (body_start) {
        body = body_start + 4;
        
        // Extract headers
        size_t headers_len = body_start - response;
        headers_copy = (char*)malloc(headers_len + 1);
        if (headers_copy) {
            memcpy(headers_copy, response, headers_len);
            headers_copy[headers_len] = '\0';
        }
    }
    
    // Build JSON response (simplified - proper JSON escaping needed for production)
    size_t buffer_size = 1024 + strlen(body) + (headers_copy ? strlen(headers_copy) : 0);
    char* result = (char*)malloc(buffer_size);
    if (result) {
        snprintf(result, buffer_size,
                 "{\"status\":%d,\"headers\":\"%s\",\"body\":\"%s\"}",
                 status, headers_copy ? "..." : "", body);
    }
    
    free(headers_copy);
    return result ? result : strdup("");
}

// ============================================================================
// High-Level HTTP Request
// ============================================================================

char* mlp_http_request(const char* method, const char* url,
                        const char* headers, const char* body) {
    if (!method || !url) return strdup("");
    
    // Parse URL
    ParsedURL* parsed = parse_url_internal(url);
    if (!parsed) return strdup("");
    
    // Note: HTTPS not supported yet (needs TLS/SSL)
    if (strcmp(parsed->scheme, "https") == 0) {
        free_parsed_url(parsed);
        free(parsed);
        return strdup("");  // HTTPS not implemented
    }
    
    // Connect to server
    int64_t sock = mlp_socket_connect(parsed->host, parsed->port);
    if (sock <= 0) {
        free_parsed_url(parsed);
        free(parsed);
        return strdup("");
    }
    
    // Build request
    char* request = mlp_http_build_request(method, parsed->path, parsed->host, headers, body);
    
    // Send request
    int64_t sent = mlp_socket_send(sock, request, strlen(request));
    free(request);
    
    if (sent <= 0) {
        mlp_socket_close(sock);
        free_parsed_url(parsed);
        free(parsed);
        return strdup("");
    }
    
    // Receive response (multiple reads for large responses)
    size_t total_size = 0;
    size_t buffer_capacity = 8192;
    char* response_buffer = (char*)malloc(buffer_capacity);
    if (!response_buffer) {
        mlp_socket_close(sock);
        free_parsed_url(parsed);
        free(parsed);
        return strdup("");
    }
    
    while (1) {
        char* chunk = mlp_socket_recv(sock, 4096);
        if (!chunk || strlen(chunk) == 0) {
            free(chunk);
            break;
        }
        
        size_t chunk_len = strlen(chunk);
        if (total_size + chunk_len >= buffer_capacity) {
            buffer_capacity *= 2;
            char* new_buffer = (char*)realloc(response_buffer, buffer_capacity);
            if (!new_buffer) {
                free(chunk);
                free(response_buffer);
                mlp_socket_close(sock);
                free_parsed_url(parsed);
                free(parsed);
                return strdup("");
            }
            response_buffer = new_buffer;
        }
        
        memcpy(response_buffer + total_size, chunk, chunk_len);
        total_size += chunk_len;
        free(chunk);
    }
    
    response_buffer[total_size] = '\0';
    
    mlp_socket_close(sock);
    free_parsed_url(parsed);
    free(parsed);
    
    return response_buffer;
}

char* mlp_http_get(const char* url) {
    return mlp_http_request("GET", url, "", "");
}

char* mlp_http_post(const char* url, const char* body, const char* content_type) {
    char headers[256];
    snprintf(headers, sizeof(headers), "Content-Type: %s\r\n", content_type);
    return mlp_http_request("POST", url, headers, body);
}

// ============================================================================
// HTTP Header Helpers
// ============================================================================

char* mlp_http_add_header(const char* headers, const char* name, const char* value) {
    size_t size = (headers ? strlen(headers) : 0) + strlen(name) + strlen(value) + 10;
    char* result = (char*)malloc(size);
    if (!result) return strdup("");
    
    if (headers && strlen(headers) > 0) {
        snprintf(result, size, "%s%s: %s\r\n", headers, name, value);
    } else {
        snprintf(result, size, "%s: %s\r\n", name, value);
    }
    
    return result;
}

char* mlp_http_get_header(const char* response, const char* header_name) {
    if (!response || !header_name) return strdup("");
    
    // Find header (case-insensitive search)
    char search[256];
    snprintf(search, sizeof(search), "\n%s:", header_name);
    
    const char* pos = strcasestr(response, search);
    if (!pos) return strdup("");
    
    pos += strlen(search);
    while (*pos == ' ') pos++;  // Skip spaces
    
    const char* end = strstr(pos, "\r\n");
    if (!end) return strdup("");
    
    size_t len = end - pos;
    char* value = (char*)malloc(len + 1);
    if (!value) return strdup("");
    
    memcpy(value, pos, len);
    value[len] = '\0';
    
    return value;
}

int64_t mlp_http_get_status_code(const char* response) {
    if (!response) return -1;
    
    int status = -1;
    sscanf(response, "HTTP/%*d.%*d %d", &status);
    return (int64_t)status;
}

char* mlp_http_get_body(const char* response) {
    if (!response) return strdup("");
    
    const char* body_start = strstr(response, "\r\n\r\n");
    if (!body_start) return strdup("");
    
    return strdup(body_start + 4);
}
