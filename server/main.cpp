#include "tcp_server.hpp"
#include "handlers.hpp"
#include <thread>
#include <iostream>

int main() {
    try {
        TcpServer srv("127.0.0.1", 1234);

        for (;;) {
            int client_fd = srv.listen_and_accept();
            if (client_fd < 0) continue;

            std::thread t(handle_client, client_fd);
            t.detach(); // each client handled in its own thread
        }
    } catch (const std::exception& ex) {
        std::cerr << "[SERVER] Fatal: " << ex.what() << std::endl;
        return 1;
    }
}
