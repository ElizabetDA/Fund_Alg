#include "../include/logger.hpp"
#include <chrono>
#include <iomanip>
#include <sstream>

StreamLoggerHandler::StreamLoggerHandler(std::ostream& stream) : stream_(stream) {}
void StreamLoggerHandler::write(const std::string& msg) {
    stream_ << msg << std::endl;
}

FileLoggerHandler::FileLoggerHandler(const std::string& filename) {
    file_.open(filename, std::ios::app);
    if (!file_) throw std::runtime_error("Cannot open log file: " + filename);
}
void FileLoggerHandler::write(const std::string& msg) {
    if (file_) file_ << msg << std::endl;
}
FileLoggerHandler::~FileLoggerHandler() {
    if (file_.is_open()) file_.close();
}

Logger::Logger(std::string name, Level level)
    : logger_name_(std::move(name)), log_level_(level) {}

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

void Logger::addHandler(std::unique_ptr<LogHandler> handler) {
    handlers_.push_back(std::move(handler));
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

Logger::Builder::Builder(std::string name) : name_(std::move(name)) {}

Logger::Builder& Logger::Builder::setLevel(Level level) {
    level_ = level;
    return *this;
}

Logger* Logger::Builder::build() {
    auto* logger = new Logger(name_, level_);
    logger->addHandler(std::make_unique<StreamLoggerHandler>(std::cout)); // по умолчанию
    return logger;
}
