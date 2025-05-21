#include "../include/logger.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <memory>

int main() {
	Logger* logger = Logger::Builder()
		.addHandler(std::make_unique<FileLoggerHandler>("test.log"))
		.addHandler(std::make_unique<StreamLoggerHandler>(std::cout))
		.setName("Logger")
		.build();

	std::vector<std::string> messages = {
		"Запуск системы",
		"Ошибка при подключении к базе",
		"Внимание: превышен лимит",
		"Завершение работы",
		"Процесс завершён успешно"
	};

	std::random_device rd;

	for (int i = 0; i < 100; ++i) {
		int lvl = rd() % 5;  // выбираем случайный уровень
		const std::string& msg = messages[rd() % messages.size()];
		logger->log(static_cast<Logger::Level>(lvl), msg);
	}

	logger->close();
	delete logger;
	return 0;
}
