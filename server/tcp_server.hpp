#pragma once
#include <string>
#include <vector>
#include <cstdint>

// Simple TCP server wrapper (blocking I/O)
class TcpServer {
public:
    // Create and bind a server socket on given IP and port
    TcpServer(const std::string& ip, uint16_t port);

    // Close listening socket
    ~TcpServer();

    // Accept a new client, return its file descriptor
    int listen_and_accept();

private:
    int listen_fd_{-1}; // listening socket
};
