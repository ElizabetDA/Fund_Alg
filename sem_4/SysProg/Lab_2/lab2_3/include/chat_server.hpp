#pragma once

#include "chat_common.hpp"
#include "shared_memory_manager.hpp"
#include "semaphore_manager.hpp"
#include "logger.hpp"

class ChatServer {
public:
	ChatServer(key_t key, Logger* logger);
	~ChatServer();

	// Запустить сервер: инициализировать SHM и обрабатывать сообщения
	void run();

private:
	void initHeader();

	SharedMemoryManager shm_;
	SemaphoreManager   sem_;
	Logger*            logger_;
};
