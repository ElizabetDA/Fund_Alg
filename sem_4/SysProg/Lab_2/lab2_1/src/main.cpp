#include "../include/logger.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <random>

int main() {
	auto logger = LoggerBuilder("AppLogger").build();

	logger->addHandler(std::make_unique<FileLoggerHandler>("output.log"));
	logger->addHandler(std::make_unique<StreamLoggerHandler>(std::cout));

	std::vector<std::string> messages = {
		"Запуск системы",
		"Ошибка при подключении к базе",
		"Внимание: превышен лимит",
		"Завершение работы",
		"Процесс завершён успешно"
	};

	std::random_device rd;

	for (int i = 0; i < 100; ++i) {
		int raw = rd() % 5;
		const std::string& msg = messages[rd() % messages.size()];
		logger->log(static_cast<Logger::Level>(raw), msg);
	}

	logger->close();
	delete logger;
	return 0;
}
