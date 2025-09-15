#pragma once

#include <string>
#include <vector>
#include <cstdint>  // for uint32_t

/**
 * @brief Adds a filename to backup.info if it does not already exist.
 * @param filename Name of the file to add.
 */
void add_to_backup_info(const std::string& filename);

/**
 * @brief Removes a filename from backup.info.
 * @param filename Name of the file to remove.
 */
void remove_from_backup_info(const std::string& filename);

/**
 * @brief Reads all filenames from backup.info.
 * @return A vector containing the list of filenames.
 */
std::vector<std::string> read_backup_info();

/**
 * @brief Builds the full backup path for a given user and filename.
 *        Creates the user’s directory if it does not exist.
 * @param user_id User ID used to create the directory path.
 * @param filename Name of the file.
 * @return Full backup path as a string.
 */
std::string get_backup_path(uint32_t user_id, const std::string& filename);
