#include "file_utils.hpp"
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <filesystem>
using namespace std::filesystem;


void add_to_backup_info(const std::string& filename) {
    std::ifstream in("backup.info");
    std::vector<std::string> files;
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty()) files.push_back(line);
    }
    in.close();

    if (std::find(files.begin(), files.end(), filename) == files.end()) {
        std::ofstream out("backup.info", std::ios::app);
        out << filename << "\n";
    }
}

void remove_from_backup_info(const std::string& filename) {
    std::ifstream in("backup.info");
    std::vector<std::string> files;
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty() && line != filename) files.push_back(line);
    }
    in.close();

    std::ofstream out("backup.info");
    for (const auto& f : files) {
        out << f << "\n";
    }
}

std::vector<std::string> read_backup_info() {
    std::ifstream in("backup.info");
    std::vector<std::string> files;
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty()) files.push_back(line);
    }
    return files;
}
std::string get_backup_path(uint32_t user_id, const std::string& filename) {
    const char* base = std::getenv("BACKUP_BASE_DIR");
    std::string base_dir = base ? base : "backupsvr"; // ברירת מחדל: backupsvr יחסית ל-CWD
    std::string dir = base_dir + "/" + std::to_string(user_id);
    std::filesystem::create_directories(dir);
    return dir + "/" + filename;
}