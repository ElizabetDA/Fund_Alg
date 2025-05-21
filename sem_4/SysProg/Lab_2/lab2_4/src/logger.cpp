#include "../include/logger.hpp"
#include <stdexcept>

// определяем глобальный указатель
Logger* g_logger = nullptr;

// --- StreamHandler ---
StreamLoggerHandler::StreamLoggerHandler(std::ostream& stream)
  : stream_(stream) {}

void StreamLoggerHandler::write(const std::string& msg) {
    stream_ << msg << std::endl;
}

// --- FileHandler ---
FileLoggerHandler::FileLoggerHandler(const std::string& filename)
  : file_(filename, std::ios::app)
{
    if (!file_.is_open())
        throw std::runtime_error("Cannot open log file: " + filename);
}

void FileLoggerHandler::write(const std::string& msg) {
    file_ << msg << std::endl;
}

FileLoggerHandler::~FileLoggerHandler() {
    if (file_.is_open())
        file_.close();
}

// --- Logger impl ---
Logger::Logger(std::string name, Level level, std::vector<std::unique_ptr<LogHandler>> handlers)
  : logger_name_(std::move(name))
  , log_level_(level)
  , handlers_(std::move(handlers))
{}

std::string Logger::timestamp() {
    auto now  = std::chrono::system_clock::now();
    auto t    = std::chrono::system_clock::to_time_t(now);
    std::tm tm;
  #ifdef _WIN32
    localtime_s(&tm, &t);
  #else
    localtime_r(&t, &tm);
  #endif
    std::ostringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void Logger::log(Level level, const std::string& message) {
    if (level > log_level_) return;
    static const char* names[] = {"CRITICAL","ERROR","WARNING","INFO","DEBUG"};
    std::ostringstream ss;
    ss << "[" << timestamp() << "] ["
       << logger_name_ << "] ["
       << names[int(level)] << "] "
       << message;
    auto out = ss.str();
    for (auto& h : handlers_) h->write(out);
}

void Logger::close() {
    handlers_.clear();
}

// --- Builder impl ---
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
    auto ptr = new Logger(name_, level_, std::move(handlers_));
    ::g_logger = ptr;
    return ptr;
}