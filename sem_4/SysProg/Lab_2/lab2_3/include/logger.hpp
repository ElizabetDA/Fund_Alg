#pragma once
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <sstream>

// Базовый обработчик логов
class LogHandler {
public:
	virtual ~LogHandler() = default;
	virtual void write(const std::string& msg) = 0;
};

// Пишет логи в любой std::ostream
class StreamLoggerHandler : public LogHandler {
	std::ostream& stream_;
public:
	explicit StreamLoggerHandler(std::ostream& stream = std::cout);
	void write(const std::string& msg) override;
};

// Пишет логи в файл
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
		CRITICAL, ERROR, WARNING, INFO, DEBUG
	};

	class Builder;

	// Вывести одно сообщение, если level ≤ текущего
	void log(Level level, const std::string& message);
	void close();  // при необходимости

private:
	Logger(std::string name,
		   Level level,
		   std::vector<std::unique_ptr<LogHandler>> handlers);

	std::string                                 logger_name_;
	Level                                       log_level_;
	std::vector<std::unique_ptr<LogHandler>>    handlers_;

	static std::string timestamp();
};

class Logger::Builder {
	std::string name_    = "default";
	Logger::Level level_ = Logger::Level::DEBUG;
	std::vector<std::unique_ptr<LogHandler>> handlers_;
public:
	Builder() = default;

	Builder& setName(const std::string& name);
	Builder& setLevel(Level level);
	Builder& addHandler(std::unique_ptr<LogHandler> handler);
	Logger*  build();
};
