#pragma once

#include <filesystem>

namespace fs = std::filesystem;

fs::path check_path(const fs::path& path);

fs::path check_path(const std::string& path);