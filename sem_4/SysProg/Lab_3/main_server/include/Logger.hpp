#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <string>
#include <sstream>

class Logger {
public:
	template<typename... Args>
	static void info(const std::string& fmt, Args... args) {
		std::cout << "[INFO] " << format(fmt, args...) << std::endl;
	}
	template<typename... Args>
	static void error(const std::string& fmt, Args... args) {
		std::cerr << "[ERROR] " << format(fmt, args...) << std::endl;
	}
private:
	// базовый случай
	static std::string format(const std::string& s) {
		return s;
	}
	// рекурсивно заменяем первое '{}' на toString(v)
	template<typename T, typename... Rest>
	static std::string format(const std::string& s, T v, Rest... r) {
		auto pos = s.find("{}");
		if (pos == std::string::npos) return s;
		std::string out = s.substr(0,pos) + toString(v) + s.substr(pos+2);
		return format(out, r...);
	}

	// для чисел
	template<typename T,
			 typename std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
	static std::string toString(T v) {
		return std::to_string(v);
	}
	// для std::string
	static std::string toString(const std::string& v) {
		return v;
	}
	// для C-строк
	static std::string toString(const char* v) {
		return v ? std::string(v) : std::string();
	}
	// для любых прочих типов — через ostringstream
	template<typename T,
			 typename std::enable_if_t<!std::is_arithmetic_v<T>, int> = 0>
	static std::string toString(const T& v) {
		std::ostringstream oss;
		oss << v;
		return oss.str();
	}
};

#endif // LOGGER_HPP
