#include "../include/logger.hpp"
#include <stdexcept>

// ——— StreamLoggerHandler ———
StreamLoggerHandler::StreamLoggerHandler(std::ostream& stream)
  : stream_(stream) {}

void StreamLoggerHandler::write(const std::string& msg) {
    stream_ << msg << std::endl;
}

// ——— FileLoggerHandler ———
FileLoggerHandler::FileLoggerHandler(const std::string& filename)
  : file_(filename, std::ios::app)
{
    if (!file_) throw std::runtime_error("Cannot open log file: " + filename);
}

void FileLoggerHandler::write(const std::string& msg) {
    file_ << msg << std::endl;
}

FileLoggerHandler::~FileLoggerHandler() {
    if (file_.is_open()) file_.close();
}

// ——— Logger ———
Logger::Logger(std::string name,
               Level level,
               std::vector<std::unique_ptr<LogHandler>> handlers)
  : logger_name_(std::move(name)),
    log_level_(level),
    handlers_(std::move(handlers))
{}

std::string Logger::timestamp() {
    auto now = std::chrono::system_clock::now();
    auto t   = std::chrono::system_clock::to_time_t(now);
    std::tm tm;
    localtime_r(&t, &tm);
    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);
    return buf;
}

void Logger::log(Level level, const std::string& message) {
    if (level > log_level_) return;

    static const char* names[] = {
        "CRITICAL", "ERROR", "WARNING", "INFO", "DEBUG"
    };
    std::ostringstream oss;
    oss << timestamp()
        << " [" << names[static_cast<int>(level)] << "] "
        << logger_name_ << ": " << message;
    auto out = oss.str();

    for (auto& h : handlers_) {
        h->write(out);
    }
}

void Logger::close() {
    // ничего особо не нужно — handlers закроют себя в деструкторах
}

// ——— Builder ———
Logger::Builder& Logger::Builder::setName(const std::string& name) {
    name_ = name; return *this;
}

Logger::Builder& Logger::Builder::setLevel(Level level) {
    level_ = level; return *this;
}

Logger::Builder& Logger::Builder::addHandler(std::unique_ptr<LogHandler> handler) {
    handlers_.push_back(std::move(handler));
    return *this;
}

Logger* Logger::Builder::build() {
    return new Logger(name_, level_, std::move(handlers_));
}
