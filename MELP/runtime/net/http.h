/**
 * MLP Standard Library - HTTP Client Header
 * 
 * HTTP/1.1 client implementation with URL parsing and request building
 * Architecture: Built on top of socket layer
 * 
 * YZ_01 - TODO #2 Task 1: Networking Library
 */

#ifndef MLP_NET_HTTP_H
#define MLP_NET_HTTP_H

#include <stdint.h>

// ============================================================================
// URL Parsing
// ============================================================================

/**
 * Parse URL into components
 * @param url: Full URL (e.g., "http://example.com:80/path?query=1")
 * @return: JSON-formatted string with components:
 *          {"scheme":"http","host":"example.com","port":80,"path":"/path","query":"query=1"}
 *          Returns empty string on parse error
 */
char* mlp_http_parse_url(const char* url);

/**
 * URL encode string (percent encoding)
 * @param str: String to encode
 * @return: Encoded string (caller must free)
 *          "hello world" -> "hello%20world"
 */
char* mlp_http_url_encode(const char* str);

/**
 * URL decode string (percent decoding)
 * @param str: Encoded string
 * @return: Decoded string (caller must free)
 *          "hello%20world" -> "hello world"
 */
char* mlp_http_url_decode(const char* str);

// ============================================================================
// HTTP Request Building
// ============================================================================

/**
 * Build HTTP request string
 * @param method: HTTP method (GET, POST, PUT, DELETE, etc.)
 * @param path: Request path (e.g., "/api/data")
 * @param host: Host header value
 * @param headers: Additional headers (multiline, ends with \r\n)
 * @param body: Request body (empty string if no body)
 * @return: Complete HTTP request string (caller must free)
 */
char* mlp_http_build_request(const char* method, const char* path, 
                              const char* host, const char* headers, 
                              const char* body);

// ============================================================================
// HTTP Response Parsing
// ============================================================================

/**
 * Parse HTTP response
 * @param response: Raw HTTP response string
 * @return: JSON-formatted string with parsed response:
 *          {"status":200,"reason":"OK","headers":"...","body":"..."}
 *          Returns empty string on parse error
 */
char* mlp_http_parse_response(const char* response);

// ============================================================================
// High-Level HTTP Request
// ============================================================================

/**
 * Perform complete HTTP request (connects, sends, receives, closes)
 * @param method: HTTP method (GET, POST, etc.)
 * @param url: Full URL
 * @param headers: Additional headers (optional, can be empty)
 * @param body: Request body (optional, can be empty)
 * @return: Complete HTTP response string (caller must free)
 *          Returns empty string on error
 */
char* mlp_http_request(const char* method, const char* url, 
                        const char* headers, const char* body);

/**
 * Perform HTTP GET request (convenience function)
 * @param url: Full URL
 * @return: Response body (caller must free), empty string on error
 */
char* mlp_http_get(const char* url);

/**
 * Perform HTTP POST request (convenience function)
 * @param url: Full URL
 * @param body: Request body
 * @param content_type: Content-Type header value
 * @return: Response body (caller must free), empty string on error
 */
char* mlp_http_post(const char* url, const char* body, const char* content_type);

// ============================================================================
// HTTP Header Helpers
// ============================================================================

/**
 * Add header to headers string
 * @param headers: Existing headers string (may be NULL)
 * @param name: Header name
 * @param value: Header value
 * @return: New headers string with added header (caller must free)
 */
char* mlp_http_add_header(const char* headers, const char* name, const char* value);

/**
 * Get header value from response
 * @param response: HTTP response string
 * @param header_name: Header name to find (case-insensitive)
 * @return: Header value (caller must free), empty string if not found
 */
char* mlp_http_get_header(const char* response, const char* header_name);

/**
 * Extract status code from response
 * @param response: HTTP response string
 * @return: Status code (e.g., 200), -1 on parse error
 */
int64_t mlp_http_get_status_code(const char* response);

/**
 * Extract body from response
 * @param response: HTTP response string
 * @return: Response body (caller must free), empty string if no body
 */
char* mlp_http_get_body(const char* response);

#endif // MLP_NET_HTTP_H
