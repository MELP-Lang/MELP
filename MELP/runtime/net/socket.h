/**
 * MLP Standard Library - Network Socket Functions Header
 * 
 * Provides TCP/UDP socket operations for MELP programs
 * Architecture: POSIX sockets (Linux/macOS compatible)
 * 
 * YZ_01 - TODO #2 Task 1: Networking Library
 */

#ifndef MLP_NET_SOCKET_H
#define MLP_NET_SOCKET_H

#include <stdint.h>

// ============================================================================
// Socket Operations - TCP
// ============================================================================

/**
 * Connect to TCP server
 * @param host: Server hostname or IP address
 * @param port: Server port number
 * @return: Socket descriptor (>0) on success, -1 on error
 */
int64_t mlp_socket_connect(const char* host, int64_t port);

/**
 * Send data through TCP socket
 * @param sock: Socket descriptor
 * @param data: Data to send
 * @param len: Length of data in bytes
 * @return: Bytes sent on success, -1 on error
 */
int64_t mlp_socket_send(int64_t sock, const char* data, int64_t len);

/**
 * Receive data from TCP socket
 * @param sock: Socket descriptor
 * @param max_bytes: Maximum bytes to receive
 * @return: Dynamically allocated string with received data (caller must free)
 *          Returns empty string "" on error or connection closed
 */
char* mlp_socket_recv(int64_t sock, int64_t max_bytes);

/**
 * Close socket connection
 * @param sock: Socket descriptor
 * @return: 0 on success, -1 on error
 */
int64_t mlp_socket_close(int64_t sock);

// ============================================================================
// Socket Operations - UDP
// ============================================================================

/**
 * Create UDP socket
 * @return: Socket descriptor (>0) on success, -1 on error
 */
int64_t mlp_socket_udp_create(void);

/**
 * Send UDP datagram
 * @param sock: Socket descriptor
 * @param host: Target hostname or IP address
 * @param port: Target port number
 * @param data: Data to send
 * @param len: Length of data in bytes
 * @return: Bytes sent on success, -1 on error
 */
int64_t mlp_socket_udp_send(int64_t sock, const char* host, int64_t port, const char* data, int64_t len);

/**
 * Receive UDP datagram
 * @param sock: Socket descriptor
 * @param max_bytes: Maximum bytes to receive
 * @return: Dynamically allocated string with received data (caller must free)
 *          Returns empty string "" on error
 */
char* mlp_socket_udp_recv(int64_t sock, int64_t max_bytes);

// ============================================================================
// Server Operations - TCP
// ============================================================================

/**
 * Create TCP server socket
 * @return: Socket descriptor (>0) on success, -1 on error
 */
int64_t mlp_socket_server_create(void);

/**
 * Bind socket to address and port
 * @param sock: Socket descriptor
 * @param port: Port number to bind to
 * @return: 0 on success, -1 on error
 */
int64_t mlp_socket_bind(int64_t sock, int64_t port);

/**
 * Listen for incoming connections
 * @param sock: Socket descriptor
 * @param backlog: Maximum number of pending connections
 * @return: 0 on success, -1 on error
 */
int64_t mlp_socket_listen(int64_t sock, int64_t backlog);

/**
 * Accept incoming connection
 * @param sock: Server socket descriptor
 * @return: Client socket descriptor (>0) on success, -1 on error
 */
int64_t mlp_socket_accept(int64_t sock);

// ============================================================================
// Utility Functions
// ============================================================================

/**
 * Set socket to non-blocking mode
 * @param sock: Socket descriptor
 * @return: 0 on success, -1 on error
 */
int64_t mlp_socket_set_nonblocking(int64_t sock);

/**
 * Set socket timeout
 * @param sock: Socket descriptor
 * @param timeout_sec: Timeout in seconds
 * @return: 0 on success, -1 on error
 */
int64_t mlp_socket_set_timeout(int64_t sock, int64_t timeout_sec);

/**
 * Get last socket error message
 * @return: Dynamically allocated error string (caller must free)
 */
char* mlp_socket_get_error(void);

#endif // MLP_NET_SOCKET_H
