
#include <rapidjson/istreamwrapper.h>
#include <filesystem>
#include <fstream>
// #include <fmt/format.h>
#include <format>

#include "inout.h"


std::filesystem::path check_path(const fs::path& path) {
	const auto abs_path = std::filesystem::absolute(path);

	if(!std::filesystem::exists(abs_path)) {
		auto message
			= std::format("File does not exists: {} ", path.string());
		if(abs_path != path) {
			message += std::format(" absolute path: {}", abs_path.string());
		}
		throw std::runtime_error(message);
	}

	return abs_path;
}

std::filesystem::path check_path(const std::string& path) {
	return check_path(fs::path(path));
}
