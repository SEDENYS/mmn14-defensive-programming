#pragma once

#include <string>
#include <cstdint>
#include <vector>

// Check protocol version
void check_version(uint8_t version);

// Send response to client
void send_response(int client_fd, uint16_t status, const std::string &filename,
                   const std::vector<uint8_t> &payload = {});

// Handle backup request
void handle_backup(int client_fd, uint32_t user_id, const std::string &filename);

// Handle delete request
void handle_delete(int client_fd, uint32_t user_id, const std::string &filename);

// Handle retrieve request
void handle_retrieve(int client_fd, uint32_t user_id, const std::string &filename);

// Handle list request
void handle_list(int client_fd, uint32_t user_id);

// Handle client connection
void handle_client(int client_fd);
