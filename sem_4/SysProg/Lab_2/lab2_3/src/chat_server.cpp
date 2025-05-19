#include "../include/chat_server.hpp"
#include <memory>
#include "../include/logger.hpp"
#include <sys/types.h>
#include <sys/ipc.h>

#include <thread>
#include <atomic>
#include <iostream>
#include <cstring>

ChatServer::ChatServer(key_t key, Logger* logger)
  : shm_(key),
	sem_(key),
	logger_(logger)
{}

ChatServer::~ChatServer() = default;

void ChatServer::initHeader() {
	// Определяем полный размер SHM:
	std::size_t size = sizeof(ShmHeader) + CHAT_BUF_SIZE * sizeof(Message);

	// Блокируем и полностью обнуляем память
	sem_.down(SemaphoreManager::MUTEX);
	std::memset(shm_.header(), 0, size);
	// Теперь явно ставим начальные значения
	shm_.header()->client_count = 0;
	shm_.header()->write_index  = 0;
	sem_.up(SemaphoreManager::MUTEX);
}

void ChatServer::run() {
	initHeader();
	logger_->log(Logger::Level::INFO, "Server started");

	std::size_t server_read = 0;

	while (true) {
		// Ждём, пока кто-то что-то присылает
		sem_.down(SemaphoreManager::MSGSEM);

		// Читаем под защёлкой MUTEX
		sem_.down(SemaphoreManager::MUTEX);
		Message msg = shm_.buffer()[server_read % CHAT_BUF_SIZE];
		server_read++;
		sem_.up(SemaphoreManager::MUTEX);

		logger_->log(Logger::Level::INFO,
			"Broadcasting from " + std::to_string(msg.from_id) + ": " + msg.text);

		// «Разослать» всем подключённым (кроме отправителя) семафор MSGSEM
		sem_.down(SemaphoreManager::MUTEX);
		int cnt = shm_.header()->client_count;
		sem_.up(SemaphoreManager::MUTEX);

		for (int i = 0; i < cnt - 1; ++i) {
			sem_.up(SemaphoreManager::MSGSEM);
		}
	}
}

int main() {
	auto* logger = Logger::Builder()
		.setName("ChatServer")
		.setLevel(Logger::Level::INFO)
		.addHandler(std::make_unique<StreamLoggerHandler>(std::cout))
		.addHandler(std::make_unique<FileLoggerHandler>("server.log"))
		.build();

	key_t key = ftok("/tmp", 65);
	ChatServer srv(key, logger);
	srv.run();

	delete logger;
	return 0;
}