#pragma once
#include <string>
#include <memory>
#include <vector>
#include <fstream>
#include <iostream>

enum class LogLevel {
	DEBUG,
	INFO,
	WARNING,
	ERROR,
	CRITICAL
};

class LogHandler {
public:
	virtual ~LogHandler() = default;
	virtual void write(const std::string& message) = 0;
};

class FileLogHandler : public LogHandler {
public:
	explicit FileLogHandler(const std::string& filename);
	void write(const std::string& message) override;
	void close();
private:
	std::ofstream file;
};

class ConsoleLogHandler : public LogHandler {
public:
	void write(const std::string& message) override;
};

class Logger {
public:
	class Builder;

	void log(LogLevel level, const std::string& message);
	void close();

private:
	Logger() = default;
	std::vector<std::unique_ptr<LogHandler>> handlers;
	LogLevel currentLevel = LogLevel::DEBUG;

	friend class Builder;
};

class Logger::Builder {
public:
	Builder& addFileHandler(const std::string& filename);
	Builder& addConsoleHandler();
	Builder& setLevel(LogLevel level);
	std::unique_ptr<Logger> build();

private:
	std::unique_ptr<Logger> logger = std::make_unique<Logger>();
};
