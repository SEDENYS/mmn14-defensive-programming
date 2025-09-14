#pragma once
#include <cstdint>
#include <string>
#include <vector>

// עטיפה דקה על Berkeley sockets (מתאים למק). נחליף/נרחיב ל-Winsock בהמשך.
class TcpServer {
public:
    TcpServer(const std::string& ip, uint16_t port);
    ~TcpServer();
    bool listen_and_accept();               // מאזין ומקבל חיבור יחיד
    bool recv_exact(std::vector<uint8_t>& buf, size_t n); // קורא בדיוק n בייטים
    bool recv_some(std::vector<uint8_t>& buf, size_t max_n); // אופציונלי
    bool send_all(const std::vector<uint8_t>& data);    // שולח הכל
    void close_client();
    int get_client_fd() const {
    return client_fd_;
}


private:
    int listen_fd_ = -1;
    int client_fd_ = -1;
};
