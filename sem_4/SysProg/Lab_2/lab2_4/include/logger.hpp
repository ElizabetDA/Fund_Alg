// include/logger.hpp
#pragma once
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

class LogHandler {
public:
	virtual ~LogHandler() = default;
	virtual void write(const std::string& msg) = 0;
};

class StreamLoggerHandler : public LogHandler {
	std::ostream& stream_;
public:
	explicit StreamLoggerHandler(std::ostream& stream = std::cout);
	void write(const std::string& msg) override;
};

class FileLoggerHandler : public LogHandler {
	std::ofstream file_;
public:
	explicit FileLoggerHandler(const std::string& filename);
	void write(const std::string& msg) override;
	~FileLoggerHandler() override;
};

class Logger {
public:
	enum class Level {
		CRITICAL,
		ERROR,
		WARNING,
		INFO,
		DEBUG
	};

	class Builder;

	void log(Level level, const std::string& message);
	void close();

private:
	Logger(std::string name, Level level, std::vector<std::unique_ptr<LogHandler>> handlers);

	std::string logger_name_;
	Level log_level_;
	std::vector<std::unique_ptr<LogHandler>> handlers_;

	static std::string timestamp();
};

// глобальный указатель на логгер
extern Logger* g_logger;

using LoggerBuilder = Logger::Builder;

class Logger::Builder {
	std::string name_   = "default";
	Level level_        = Level::DEBUG;
	std::vector<std::unique_ptr<LogHandler>> handlers_;

public:
	Builder() = default;

	Builder& setName(const std::string& name);
	Builder& setLevel(Level level);
	Builder& addHandler(std::unique_ptr<LogHandler> handler);
	Logger* build();
};
