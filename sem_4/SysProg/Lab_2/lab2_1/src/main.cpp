#include "logger.hpp"

int main() {
	try {
		auto logger = Logger::Builder()
						  .addConsoleHandler()
						  .addFileHandler("log.txt")
						  .setLevel(LogLevel::INFO)
						  .build();

		logger->log(LogLevel::DEBUG, "This won't be shown");
		logger->log(LogLevel::INFO, "App started");
		logger->log(LogLevel::ERROR, "Something went wrong!");

		logger->close();
	} catch (const std::exception& e) {
		std::cerr << "Logger error: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}
