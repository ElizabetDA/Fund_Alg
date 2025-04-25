#include "../include/logger.hpp"
#include <chrono>
#include <iomanip>
#include <sstream>

// --- StreamLoggerHandler ---
StreamLoggerHandler::StreamLoggerHandler(std::ostream& stream) : stream_(stream) {}
void StreamLoggerHandler::write(const std::string& msg) {
    stream_ << msg << std::endl;
}

// --- FileLoggerHandler ---
FileLoggerHandler::FileLoggerHandler(const std::string& filename) {
    file_.open(filename, std::ios::app);
    if (!file_) throw std::runtime_error("Cannot open file: " + filename);
}
void FileLoggerHandler::write(const std::string& msg) {
    if (file_) file_ << msg << std::endl;
}
FileLoggerHandler::~FileLoggerHandler() {
    if (file_.is_open()) file_.close();
}

// --- Logger ---
Logger::Logger(std::string name, Level level, std::vector<std::unique_ptr<LogHandler>> handlers)
    : logger_name_(std::move(name)), log_level_(level), handlers_(std::move(handlers)) {}

void Logger::log(Level level, const std::string& message) {
    if (level > log_level_) return;

    std::string prefix;
    switch (level) {
        case Level::CRITICAL: prefix = "[CRITICAL] "; break;
        case Level::ERROR:    prefix = "[ERROR] "; break;
        case Level::WARNING:  prefix = "[WARNING] "; break;
        case Level::INFO:     prefix = "[INFO] "; break;
        case Level::DEBUG:    prefix = "[DEBUG] "; break;
    }

    std::string full = timestamp() + " " + prefix + message;
    for (auto& handler : handlers_) {
        handler->write(full);
    }
}

void Logger::close() {
    handlers_.clear();
}

std::string Logger::timestamp() {
    auto now = std::chrono::system_clock::now();
    auto t   = std::chrono::system_clock::to_time_t(now);
    std::tm tm;
    localtime_r(&t, &tm);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

// --- Builder ---
Logger::Builder& Logger::Builder::setName(const std::string& name) {
    name_ = name;
    return *this;
}

Logger::Builder& Logger::Builder::setLevel(Level level) {
    level_ = level;
    return *this;
}

Logger::Builder& Logger::Builder::addHandler(std::unique_ptr<LogHandler> handler) {
    handlers_.push_back(std::move(handler));
    return *this;
}

Logger* Logger::Builder::build() {
    if (handlers_.empty()) {
        handlers_.push_back(std::make_unique<StreamLoggerHandler>(std::cout));
    }
    return new Logger(name_, level_, std::move(handlers_));
}
