#pragma once

#include <spdlog/spdlog.h>
#include <filesystem>

namespace fs = std::filesystem;

void set_up_logger(const fs::path& directory_path);