/**
 * MLP Standard Library - Network Socket Implementation
 * 
 * POSIX sockets implementation for TCP/UDP networking
 * Compatible with Linux and macOS
 * 
 * YZ_01 - TODO #2 Task 1: Networking Library
 */

#include "socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>

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

// Thread-local error storage
static __thread char last_error[256] = {0};

// ============================================================================
// Internal Helper Functions
// ============================================================================

static void set_error(const char* msg) {
    snprintf(last_error, sizeof(last_error), "%s: %s", msg, strerror(errno));
}

static void clear_error(void) {
    last_error[0] = '\0';
}

// Resolve hostname to IP address
static int resolve_host(const char* host, struct sockaddr_in* addr, int64_t port) {
    struct hostent* he;
    
    memset(addr, 0, sizeof(*addr));
    addr->sin_family = AF_INET;
    addr->sin_port = htons((uint16_t)port);
    
    // Try direct IP address first
    if (inet_pton(AF_INET, host, &addr->sin_addr) == 1) {
        return 0;  // Success
    }
    
    // Resolve hostname
    he = gethostbyname(host);
    if (he == NULL) {
        set_error("Failed to resolve hostname");
        return -1;
    }
    
    memcpy(&addr->sin_addr, he->h_addr_list[0], he->h_length);
    return 0;
}

// ============================================================================
// TCP Socket Operations
// ============================================================================

int64_t mlp_socket_connect(const char* host, int64_t port) {
    if (!host || port <= 0 || port > 65535) {
        set_error("Invalid host or port");
        return -1;
    }
    
    clear_error();
    
    // Create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        set_error("Failed to create socket");
        return -1;
    }
    
    // Resolve and connect
    struct sockaddr_in server_addr;
    if (resolve_host(host, &server_addr, port) < 0) {
        close(sock);
        return -1;
    }
    
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        set_error("Failed to connect");
        close(sock);
        return -1;
    }
    
    return (int64_t)sock;
}

int64_t mlp_socket_send(int64_t sock, const char* data, int64_t len) {
    if (sock <= 0 || !data || len <= 0) {
        set_error("Invalid socket or data");
        return -1;
    }
    
    clear_error();
    
    ssize_t sent = send((int)sock, data, (size_t)len, 0);
    if (sent < 0) {
        set_error("Failed to send data");
        return -1;
    }
    
    return (int64_t)sent;
}

char* mlp_socket_recv(int64_t sock, int64_t max_bytes) {
    if (sock <= 0 || max_bytes <= 0) {
        set_error("Invalid socket or buffer size");
        return strdup("");
    }
    
    clear_error();
    
    // Allocate buffer
    char* buffer = (char*)malloc((size_t)max_bytes + 1);
    if (!buffer) {
        set_error("Failed to allocate buffer");
        return strdup("");
    }
    
    // Receive data
    ssize_t received = recv((int)sock, buffer, (size_t)max_bytes, 0);
    if (received < 0) {
        set_error("Failed to receive data");
        free(buffer);
        return strdup("");
    }
    
    if (received == 0) {
        // Connection closed
        free(buffer);
        return strdup("");
    }
    
    // Null-terminate
    buffer[received] = '\0';
    return buffer;
}

int64_t mlp_socket_close(int64_t sock) {
    if (sock <= 0) {
        set_error("Invalid socket");
        return -1;
    }
    
    clear_error();
    
    if (close((int)sock) < 0) {
        set_error("Failed to close socket");
        return -1;
    }
    
    return 0;
}

// ============================================================================
// UDP Socket Operations
// ============================================================================

int64_t mlp_socket_udp_create(void) {
    clear_error();
    
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        set_error("Failed to create UDP socket");
        return -1;
    }
    
    return (int64_t)sock;
}

int64_t mlp_socket_udp_send(int64_t sock, const char* host, int64_t port, const char* data, int64_t len) {
    if (sock <= 0 || !host || !data || len <= 0 || port <= 0 || port > 65535) {
        set_error("Invalid parameters");
        return -1;
    }
    
    clear_error();
    
    // Resolve target address
    struct sockaddr_in target_addr;
    if (resolve_host(host, &target_addr, port) < 0) {
        return -1;
    }
    
    // Send datagram
    ssize_t sent = sendto((int)sock, data, (size_t)len, 0,
                          (struct sockaddr*)&target_addr, sizeof(target_addr));
    if (sent < 0) {
        set_error("Failed to send UDP datagram");
        return -1;
    }
    
    return (int64_t)sent;
}

char* mlp_socket_udp_recv(int64_t sock, int64_t max_bytes) {
    if (sock <= 0 || max_bytes <= 0) {
        set_error("Invalid socket or buffer size");
        return strdup("");
    }
    
    clear_error();
    
    // Allocate buffer
    char* buffer = (char*)malloc((size_t)max_bytes + 1);
    if (!buffer) {
        set_error("Failed to allocate buffer");
        return strdup("");
    }
    
    // Receive datagram
    struct sockaddr_in sender_addr;
    socklen_t sender_len = sizeof(sender_addr);
    ssize_t received = recvfrom((int)sock, buffer, (size_t)max_bytes, 0,
                                (struct sockaddr*)&sender_addr, &sender_len);
    if (received < 0) {
        set_error("Failed to receive UDP datagram");
        free(buffer);
        return strdup("");
    }
    
    // Null-terminate
    buffer[received] = '\0';
    return buffer;
}

// ============================================================================
// TCP Server Operations
// ============================================================================

int64_t mlp_socket_server_create(void) {
    clear_error();
    
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        set_error("Failed to create server socket");
        return -1;
    }
    
    // Set SO_REUSEADDR to avoid "Address already in use" error
    int opt = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        set_error("Failed to set SO_REUSEADDR");
        close(sock);
        return -1;
    }
    
    return (int64_t)sock;
}

int64_t mlp_socket_bind(int64_t sock, int64_t port) {
    if (sock <= 0 || port <= 0 || port > 65535) {
        set_error("Invalid socket or port");
        return -1;
    }
    
    clear_error();
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons((uint16_t)port);
    
    if (bind((int)sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        set_error("Failed to bind socket");
        return -1;
    }
    
    return 0;
}

int64_t mlp_socket_listen(int64_t sock, int64_t backlog) {
    if (sock <= 0 || backlog <= 0) {
        set_error("Invalid socket or backlog");
        return -1;
    }
    
    clear_error();
    
    if (listen((int)sock, (int)backlog) < 0) {
        set_error("Failed to listen on socket");
        return -1;
    }
    
    return 0;
}

int64_t mlp_socket_accept(int64_t sock) {
    if (sock <= 0) {
        set_error("Invalid socket");
        return -1;
    }
    
    clear_error();
    
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    int client_sock = accept((int)sock, (struct sockaddr*)&client_addr, &client_len);
    if (client_sock < 0) {
        set_error("Failed to accept connection");
        return -1;
    }
    
    return (int64_t)client_sock;
}

// ============================================================================
// Utility Functions
// ============================================================================

int64_t mlp_socket_set_nonblocking(int64_t sock) {
    if (sock <= 0) {
        set_error("Invalid socket");
        return -1;
    }
    
    clear_error();
    
    int flags = fcntl((int)sock, F_GETFL, 0);
    if (flags < 0) {
        set_error("Failed to get socket flags");
        return -1;
    }
    
    if (fcntl((int)sock, F_SETFL, flags | O_NONBLOCK) < 0) {
        set_error("Failed to set non-blocking mode");
        return -1;
    }
    
    return 0;
}

int64_t mlp_socket_set_timeout(int64_t sock, int64_t timeout_sec) {
    if (sock <= 0 || timeout_sec < 0) {
        set_error("Invalid socket or timeout");
        return -1;
    }
    
    clear_error();
    
    struct timeval tv;
    tv.tv_sec = (long)timeout_sec;
    tv.tv_usec = 0;
    
    if (setsockopt((int)sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        set_error("Failed to set receive timeout");
        return -1;
    }
    
    if (setsockopt((int)sock, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) < 0) {
        set_error("Failed to set send timeout");
        return -1;
    }
    
    return 0;
}

char* mlp_socket_get_error(void) {
    return strdup(last_error[0] ? last_error : "No error");
}
