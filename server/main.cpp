#include "tcp_server.hpp"
#include "handlers.hpp"
#include <thread>
#include <iostream>

int main() {
    try {
        TcpServer srv("127.0.0.1", 1234);
        for (;;) {
            if (!srv.listen_and_accept()) continue;

            int client_fd = srv.get_client_fd();
            std::thread t(handle_client, client_fd);
            t.detach();
        }
    } catch (const std::exception& ex) {
        std::cerr << "[SERVER] Fatal: " << ex.what() << std::endl;
        return 1;
    }
}
