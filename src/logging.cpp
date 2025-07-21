#include <filesystem>
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

#include "logging.h"



void set_up_logger(const fs::path& directory_path) {
	const auto console_sink = std::make_shared<spdlog::sinks::stdout_sink_st>();
	console_sink->set_level(spdlog::level::info);

	const std::string log_filepath = fmt::format("{}/log.txt", directory_path.string());
	auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_st>(log_filepath, true);

	std::initializer_list<spdlog::sink_ptr> sink_list{console_sink, file_sink};
	const auto logger = std::make_shared<spdlog::logger>("DARP Benchmark logger", sink_list);
	logger->set_level(spdlog::level::debug);

	spdlog::set_default_logger(logger);
	spdlog::flush_every(std::chrono::seconds(3));

	spdlog::info("setting up file logging to {}", log_filepath);
}
