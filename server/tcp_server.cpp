#include "tcp_server.hpp"
#include <stdexcept>
#include <iostream>
#include <cstring>
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
    if (client_fd_ >= 0) ::close(client_fd_);
    if (listen_fd_ >= 0) ::close(listen_fd_);
}

bool TcpServer::listen_and_accept() {
    sockaddr_in cli{};
    socklen_t len = sizeof(cli);
    client_fd_ = ::accept(listen_fd_, (sockaddr*)&cli, &len);
    if (client_fd_ < 0) {
        std::perror("accept");
        return false;
    }
    std::cout << "[SERVER] Client connected\n";
    return true;
}

bool TcpServer::recv_exact(std::vector<uint8_t>& buf, size_t n) {
    buf.resize(n);
    size_t got = 0;
    while (got < n) {
        ssize_t r = ::recv(client_fd_, buf.data() + got, n - got, 0);
        if (r <= 0) return false;
        got += static_cast<size_t>(r);
    }
    return true;
}

bool TcpServer::recv_some(std::vector<uint8_t>& buf, size_t max_n) {
    buf.resize(max_n);
    ssize_t r = ::recv(client_fd_, buf.data(), max_n, 0);
    if (r <= 0) return false;
    buf.resize(static_cast<size_t>(r));
    return true;
}

bool TcpServer::send_all(const std::vector<uint8_t>& data) {
    size_t sent = 0;
    while (sent < data.size()) {
        ssize_t s = ::send(client_fd_, data.data() + sent, data.size() - sent, 0);
        if (s <= 0) return false;
        sent += static_cast<size_t>(s);
    }
    return true;
}

void TcpServer::close_client() {
    if (client_fd_ >= 0) {
        ::close(client_fd_);
        client_fd_ = -1;
    }
}
