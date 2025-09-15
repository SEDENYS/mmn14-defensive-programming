#include "tcp_server.hpp"
#include <stdexcept>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

TcpServer::TcpServer(const std::string& ip, uint16_t port) {
    listen_fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd_ < 0) throw std::runtime_error("socket() failed");

    int opt = 1;
    setsockopt(listen_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (::inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0) {
        throw std::runtime_error("inet_pton() failed");
    }

    if (::bind(listen_fd_, (sockaddr*)&addr, sizeof(addr)) < 0) {
        throw std::runtime_error("bind() failed");
    }
    if (::listen(listen_fd_, 16) < 0) {
        throw std::runtime_error("listen() failed");
    }

    std::cout << "[SERVER] Listening on " << ip << ":" << port << "\n";
}

TcpServer::~TcpServer() {
    if (listen_fd_ >= 0) ::close(listen_fd_);
}

int TcpServer::listen_and_accept() {
    sockaddr_in cli{};
    socklen_t len = sizeof(cli);
    int client_fd = ::accept(listen_fd_, (sockaddr*)&cli, &len);
    if (client_fd < 0) {
        std::perror("accept");
        return -1;
    }
    std::cout << "[SERVER] Client connected\n";
    return client_fd;
}
