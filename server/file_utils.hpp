#pragma once
#include <string>
#include <vector>

void add_to_backup_info(const std::string& filename);
void remove_from_backup_info(const std::string& filename);
std::vector<std::string> read_backup_info();
std::string get_backup_path(uint32_t user_id, const std::string& filename);
