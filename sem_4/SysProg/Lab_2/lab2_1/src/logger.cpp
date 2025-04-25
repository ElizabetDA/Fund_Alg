#include "logger.hpp"

// ========== FileLogHandler ==========
FileLogHandler::FileLogHandler(const std::string& filename) {
	file.open(filename, std::ios::app);
	if (!file) throw std::runtime_error("Unable to open log file");
}

void FileLogHandler::write(const std::string& message) {
	if (file) file << message << std::endl;
}

void FileLogHandler::close() {
	if (file.is_open()) file.close();
}

// ========== ConsoleLogHandler ==========
void ConsoleLogHandler::write(const std::string& message) {
	std::cout << message << std::endl;
}

// ========== Logger ==========
void Logger::log(LogLevel level, const std::string& message) {
	if (level < currentLevel) return;
	for (const auto& handler : handlers) {
		handler->write(message);
	}
}

void Logger::close() {
	for (const auto& handler : handlers) {
		if (auto* f = dynamic_cast<FileLogHandler*>(handler.get())) {
			f->close();
		}
	}
}

// ========== Logger::Builder ==========
Logger::Builder& Logger::Builder::addFileHandler(const std::string& filename) {
	logger->handlers.emplace_back(std::make_unique<FileLogHandler>(filename));
	return *this;
}

Logger::Builder& Logger::Builder::addConsoleHandler() {
	logger->handlers.emplace_back(std::make_unique<ConsoleLogHandler>());
	return *this;
}

Logger::Builder& Logger::Builder::setLevel(LogLevel level) {
	logger->currentLevel = level;
	return *this;
}

std::unique_ptr<Logger> Logger::Builder::build() {
	return std::move(logger);
}
