#pragma once

#include "chat_common.hpp"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdexcept>

class SharedMemoryManager {
public:
	SharedMemoryManager(key_t key) {
		// резервируем ровно ShmHeader + BUF_SIZE сообщений
		std::size_t size = sizeof(ShmHeader) + CHAT_BUF_SIZE * sizeof(Message);
		shmid_ = shmget(key, size, IPC_CREAT | 0666);
		if (shmid_ < 0) throw std::runtime_error("shmget failed");
		base_ = shmat(shmid_, nullptr, 0);
		if (base_ == (void*)-1) throw std::runtime_error("shmat failed");
	}

	~SharedMemoryManager() {
		shmdt(base_);
	}

	// Заголовок мы оставляем без изменений
	ShmHeader* header() const {
		return reinterpret_cast<ShmHeader*>(base_);
	}

	// А буфер сообщений всегда сразу за ShmHeader
	Message* buffer() const {
		return reinterpret_cast<Message*>(
			static_cast<char*>(base_) + sizeof(ShmHeader)
		);
	}

private:
	int   shmid_;
	void* base_;
};
