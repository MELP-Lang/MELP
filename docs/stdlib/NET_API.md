# MELP Networking Library - API Documentation

**Version:** 1.0  
**Author:** YZ_01  
**Date:** 1 Ocak 2026  
**Status:** Production Ready ✅

---

## Overview

The MELP Networking Library provides TCP/UDP socket operations and HTTP client functionality for MELP programs. Built on POSIX sockets, it offers a clean, Optional-based API for network communication.

### Features

- ✅ TCP client sockets
- ✅ UDP datagram sockets
- ✅ TCP server sockets
- ✅ HTTP/1.1 client (GET, POST, PUT, DELETE)
- ✅ URL parsing and manipulation
- ✅ URL encoding/decoding
- ✅ Optional-based error handling
- ✅ Thread-safe operations
- ✅ Cross-platform (Linux/macOS)

### Modules

```
stdlib/net/
├── socket.mlp    # Low-level TCP/UDP operations
├── http.mlp      # High-level HTTP client
└── url.mlp       # URL utilities
```

---

## Socket API (socket.mlp)

### TCP Client Operations

#### `socket_connect(host: string, port: numeric) -> optional`

Connect to a TCP server.

**Parameters:**
- `host`: Hostname or IP address (e.g., "example.com", "192.168.1.1")
- `port`: Port number (1-65535)

**Returns:**
- `Optional<socket>`: Some(socket_descriptor) on success, None on failure

**Example:**
```mlp
optional sock = socket_connect("example.com", 80)
if optional_has_value(sock) then
    numeric fd = optional_unwrap(sock)
    yazdir("Connected! Socket: " + string(fd))
end_if
```

---

#### `socket_write(sock: numeric, data: string) -> optional`

Send data through TCP socket.

**Parameters:**
- `sock`: Socket descriptor from socket_connect()
- `data`: Data to send

**Returns:**
- `Optional<numeric>`: Some(bytes_sent) on success, None on failure

**Example:**
```mlp
optional sent = socket_write(sock, "GET / HTTP/1.1\r\n\r\n")
if optional_has_value(sent) then
    numeric bytes = optional_unwrap(sent)
    yazdir("Sent " + string(bytes) + " bytes")
end_if
```

---

#### `socket_read(sock: numeric, max_bytes: numeric) -> optional`

Receive data from TCP socket.

**Parameters:**
- `sock`: Socket descriptor
- `max_bytes`: Maximum bytes to read (buffer size)

**Returns:**
- `Optional<string>`: Some(data) on success, None on failure/closed

**Example:**
```mlp
optional response = socket_read(sock, 4096)
if optional_has_value(response) then
    string data = optional_unwrap(response)
    yazdir("Received: " + data)
end_if
```

---

#### `socket_read_default(sock: numeric) -> optional`

Receive data with default buffer size (4096 bytes).

**Parameters:**
- `sock`: Socket descriptor

**Returns:**
- `Optional<string>`: Some(data) on success, None on failure

**Example:**
```mlp
optional response = socket_read_default(sock)
```

---

#### `socket_close(sock: numeric) -> boolean`

Close socket connection.

**Parameters:**
- `sock`: Socket descriptor

**Returns:**
- `boolean`: true on success, false on failure

**Example:**
```mlp
boolean closed = socket_close(sock)
if closed then
    yazdir("Connection closed")
end_if
```

---

### UDP Operations

#### `socket_udp_create() -> optional`

Create UDP socket.

**Returns:**
- `Optional<socket>`: Some(socket_descriptor) on success, None on failure

**Example:**
```mlp
optional sock = socket_udp_create()
```

---

#### `socket_udp_send(sock: numeric, host: string, port: numeric, data: string) -> optional`

Send UDP datagram.

**Parameters:**
- `sock`: UDP socket descriptor
- `host`: Target hostname or IP
- `port`: Target port
- `data`: Data to send

**Returns:**
- `Optional<numeric>`: Some(bytes_sent) on success, None on failure

**Example:**
```mlp
optional sent = socket_udp_send(sock, "localhost", 9000, "Hello UDP!")
```

---

#### `socket_udp_recv(sock: numeric, max_bytes: numeric) -> optional`

Receive UDP datagram.

**Parameters:**
- `sock`: UDP socket descriptor
- `max_bytes`: Maximum bytes to receive

**Returns:**
- `Optional<string>`: Some(data) on success, None on failure

**Example:**
```mlp
optional data = socket_udp_recv(sock, 1024)
```

---

### TCP Server Operations

#### `socket_server_create(port: numeric) -> optional`

Create, bind, and start listening on TCP server socket.

**Parameters:**
- `port`: Port to bind to (1-65535)

**Returns:**
- `Optional<socket>`: Some(server_socket) on success, None on failure

**Example:**
```mlp
optional server = socket_server_create(8080)
if optional_has_value(server) then
    yazdir("Server listening on port 8080")
end_if
```

---

#### `socket_accept(server_sock: numeric) -> optional`

Accept incoming connection (blocking).

**Parameters:**
- `server_sock`: Server socket from socket_server_create()

**Returns:**
- `Optional<socket>`: Some(client_socket) on success, None on failure

**Example:**
```mlp
optional client = socket_accept(server_sock)
if optional_has_value(client) then
    numeric client_fd = optional_unwrap(client)
    // Handle client connection
    socket_close(client_fd)
end_if
```

---

## HTTP API (http.mlp)

### High-Level HTTP Functions

#### `http_get(url: string) -> optional`

Perform HTTP GET request.

**Parameters:**
- `url`: Full URL (e.g., "http://example.com/api/data")

**Returns:**
- `Optional<string>`: Some(full_response) on success, None on failure
  - Response includes headers and body

**Example:**
```mlp
optional response = http_get("http://httpbin.org/get")
if optional_has_value(response) then
    string full_response = optional_unwrap(response)
    numeric status = http_get_status_code(full_response)
    string body = http_get_body(full_response)
    yazdir("Status: " + string(status))
    yazdir("Body: " + body)
end_if
```

---

#### `http_get_with_headers(url: string, headers: string) -> optional`

HTTP GET with custom headers.

**Parameters:**
- `url`: Full URL
- `headers`: Headers string (multiline, ends with \r\n)

**Returns:**
- `Optional<string>`: Some(response) on success, None on failure

**Example:**
```mlp
string headers = "Authorization: Bearer token123\r\nAccept: application/json\r\n"
optional response = http_get_with_headers("http://api.example.com/data", headers)
```

---

#### `http_post(url: string, body: string) -> optional`

HTTP POST with form data (application/x-www-form-urlencoded).

**Parameters:**
- `url`: Full URL
- `body`: Request body

**Returns:**
- `Optional<string>`: Some(response) on success, None on failure

**Example:**
```mlp
string body = "username=test&password=secret"
optional response = http_post("http://example.com/login", body)
```

---

#### `http_post_json(url: string, json_body: string) -> optional`

HTTP POST with JSON body (application/json).

**Parameters:**
- `url`: Full URL
- `json_body`: JSON string

**Returns:**
- `Optional<string>`: Some(response) on success, None on failure

**Example:**
```mlp
string json = '{"name": "MLP", "version": "1.0"}'
optional response = http_post_json("http://api.example.com/data", json)
```

---

#### `http_post_with_headers(url: string, headers: string, body: string) -> optional`

HTTP POST with custom headers.

**Parameters:**
- `url`: Full URL
- `headers`: Custom headers
- `body`: Request body

**Returns:**
- `Optional<string>`: Some(response) on success, None on failure

---

#### `http_put(url: string, body: string) -> optional`

HTTP PUT request.

**Parameters:**
- `url`: Full URL
- `body`: Request body

**Returns:**
- `Optional<string>`: Some(response) on success, None on failure

---

#### `http_delete(url: string) -> optional`

HTTP DELETE request.

**Parameters:**
- `url`: Full URL

**Returns:**
- `Optional<string>`: Some(response) on success, None on failure

---

### HTTP Response Utilities

#### `http_get_status_code(response: string) -> numeric`

Extract HTTP status code from response.

**Parameters:**
- `response`: Full HTTP response string

**Returns:**
- `numeric`: Status code (e.g., 200, 404, 500)

**Example:**
```mlp
numeric status = http_get_status_code(response)
if status == 200 then
    yazdir("Success!")
end_if
```

---

#### `http_get_body(response: string) -> string`

Extract response body from HTTP response.

**Parameters:**
- `response`: Full HTTP response string

**Returns:**
- `string`: Response body (after \r\n\r\n separator)

**Example:**
```mlp
string body = http_get_body(response)
yazdir(body)
```

---

#### `http_is_success(response: string) -> boolean`

Check if response has 2xx status code.

**Parameters:**
- `response`: Full HTTP response string

**Returns:**
- `boolean`: true if status 200-299, false otherwise

**Example:**
```mlp
if http_is_success(response) then
    yazdir("Request successful")
end_if
```

---

## URL API (url.mlp)

### URL Parsing

#### `url_parse(url: string) -> optional`

Parse URL into components.

**Parameters:**
- `url`: Full URL

**Returns:**
- `Optional<string>`: Some(JSON_string) with components, None on parse error
  - JSON format: `{"scheme":"http","host":"example.com","port":80,"path":"/api","query":"k=v"}`

**Example:**
```mlp
optional parsed = url_parse("http://example.com:8080/api/data?key=value")
```

---

#### `url_get_scheme(url: string) -> optional`

Extract scheme from URL (http, https, ftp, etc.).

**Parameters:**
- `url`: Full URL

**Returns:**
- `Optional<string>`: Some(scheme) on success, None if no scheme

**Example:**
```mlp
optional scheme = url_get_scheme("https://example.com")
// Returns: Some("https")
```

---

#### `url_get_host(url: string) -> optional`

Extract hostname from URL.

**Parameters:**
- `url`: Full URL

**Returns:**
- `Optional<string>`: Some(host) on success, None on failure

**Example:**
```mlp
optional host = url_get_host("http://api.example.com/path")
// Returns: Some("api.example.com")
```

---

#### `url_get_path(url: string) -> optional`

Extract path from URL.

**Parameters:**
- `url`: Full URL

**Returns:**
- `Optional<string>`: Some(path) on success, None on failure

**Example:**
```mlp
optional path = url_get_path("http://example.com/api/users?page=1")
// Returns: Some("/api/users")
```

---

### URL Encoding

#### `url_encode(str: string) -> string`

URL encode string (percent encoding).

**Parameters:**
- `str`: String to encode

**Returns:**
- `string`: Encoded string

**Example:**
```mlp
string encoded = url_encode("hello world")
// Returns: "hello+world"

string encoded2 = url_encode("a&b=c")
// Returns: "a%26b%3Dc"
```

---

#### `url_decode(str: string) -> string`

URL decode string.

**Parameters:**
- `str`: Encoded string

**Returns:**
- `string`: Decoded string

**Example:**
```mlp
string decoded = url_decode("hello+world")
// Returns: "hello world"
```

---

### URL Building

#### `url_build(scheme: string, host: string, port: numeric, path: string) -> string`

Build URL from components.

**Parameters:**
- `scheme`: URL scheme (http, https, etc.)
- `host`: Hostname
- `port`: Port number
- `path`: Path (with or without leading /)

**Returns:**
- `string`: Complete URL

**Example:**
```mlp
string url = url_build("https", "api.example.com", 443, "/v1/users")
// Returns: "https://api.example.com/v1/users"
```

---

#### `url_add_query(url: string, key: string, value: string) -> string`

Add query parameter to URL.

**Parameters:**
- `url`: Base URL
- `key`: Parameter name
- `value`: Parameter value

**Returns:**
- `string`: URL with added query parameter

**Example:**
```mlp
string url = "http://api.example.com/search"
url = url_add_query(url, "q", "test")
url = url_add_query(url, "limit", "10")
// Returns: "http://api.example.com/search?q=test&limit=10"
```

---

### URL Validation

#### `url_is_valid(url: string) -> boolean`

Check if URL is valid.

**Parameters:**
- `url`: URL to validate

**Returns:**
- `boolean`: true if valid, false otherwise

---

#### `url_is_absolute(url: string) -> boolean`

Check if URL is absolute (has scheme).

**Parameters:**
- `url`: URL to check

**Returns:**
- `boolean`: true if absolute, false if relative

---

## Complete Example: Web Scraper

```mlp
import http from "stdlib/net/http.mlp"
import url from "stdlib/net/url.mlp"

function fetch_json_data(string api_url) -> optional
    // Add query parameters
    string final_url = api_url
    final_url = url_add_query(final_url, "format", "json")
    final_url = url_add_query(final_url, "limit", "10")
    
    // Make HTTP request
    optional response = http_get(final_url)
    if not optional_has_value(response) then
        yazdir("ERROR: Request failed")
        return optional_none()
    end_if
    
    string full_response = optional_unwrap(response)
    
    // Check status
    if not http_is_success(full_response) then
        numeric status = http_get_status_code(full_response)
        yazdir("ERROR: HTTP " + string(status))
        return optional_none()
    end_if
    
    // Extract body
    string body = http_get_body(full_response)
    return optional_some(body)
end_function

// Usage
optional data = fetch_json_data("http://api.example.com/data")
if optional_has_value(data) then
    string json = optional_unwrap(data)
    yazdir("Data: " + json)
end_if
```

---

## Complete Example: Simple HTTP Server

```mlp
import socket from "stdlib/net/socket.mlp"

function start_http_server(numeric port) -> void
    // Create server
    optional server = socket_server_create(port)
    if not optional_has_value(server) then
        yazdir("ERROR: Failed to create server")
        return
    end_if
    
    numeric server_fd = optional_unwrap(server)
    yazdir("Server listening on port " + string(port))
    
    // Accept loop
    while true
        optional client = socket_accept(server_fd)
        if not optional_has_value(client) then
            continue
        end_if
        
        numeric client_fd = optional_unwrap(client)
        
        // Read request
        optional request = socket_read_default(client_fd)
        if optional_has_value(request) then
            string req = optional_unwrap(request)
            yazdir("Request: " + req)
            
            // Send response
            string response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello from MLP!\r\n"
            socket_write(client_fd, response)
        end_if
        
        socket_close(client_fd)
    end_while
    
    socket_close(server_fd)
end_function

// Start server on port 8080
start_http_server(8080)
```

---

## Error Handling

All networking functions use Optional types for error handling:

```mlp
optional result = socket_connect("example.com", 80)

if optional_has_value(result) then
    // Success
    numeric sock = optional_unwrap(result)
    // Use socket...
else
    // Error occurred
    yazdir("ERROR: Connection failed")
end_if
```

---

## Performance Notes

- **TCP Connection:** ~50-100ms (network dependent)
- **HTTP Request:** ~100-200ms (network dependent)
- **URL Parsing:** <1ms
- **URL Encoding:** <1ms
- **Memory:** All allocations are freed automatically

---

## Limitations

1. **HTTPS not supported** - Requires TLS/SSL library
2. **HTTP/1.1 only** - No HTTP/2 or HTTP/3
3. **Synchronous only** - No async/await
4. **No connection pooling** - New connection per request
5. **No chunked encoding** - Fixed Content-Length only

---

## Thread Safety

- All socket operations are thread-safe
- Error messages use thread-local storage
- Multiple threads can use separate sockets concurrently

---

## Platform Support

- ✅ Linux (tested)
- ✅ macOS (compatible, POSIX)
- ❌ Windows (requires WinSock port)

---

**Documentation Version:** 1.0  
**Last Updated:** 1 Ocak 2026  
**Author:** YZ_01
