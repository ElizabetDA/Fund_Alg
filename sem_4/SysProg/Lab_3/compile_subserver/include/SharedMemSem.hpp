#ifndef SHAREDMEMSEM_HPP
#define SHAREDMEMSEM_HPP

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdexcept>
#include <cstring>
#include <cerrno>

// Структура задачи компиляции
struct CompileTask {
	int client_id;
	char filename[256];
};

class SharedMemSem {
public:
	SharedMemSem(key_t key, size_t size) {
		shmid_ = shmget(key, size, IPC_CREAT | 0666);
		if (shmid_ == -1) throw std::runtime_error("shmget failed");
		semid_ = semget(key, 2, IPC_CREAT | 0666);
		if (semid_ == -1) throw std::runtime_error("semget failed");
		// подключаем SHM
		mem_ = (char*)shmat(shmid_, nullptr, 0);
		if (mem_ == (char*)-1) throw std::runtime_error("shmat failed");
		// ИНИЦИАЛИЗАЦИЯ СЕМАФОРОВ:
		// sem 0 = empty slots = 1
		semctl(semid_, 0, SETVAL, 1);
		// sem 1 = full slots = 0
		semctl(semid_, 1, SETVAL, 0);
	}

	~SharedMemSem() {
		shmdt(mem_);
	}

	void write(const CompileTask& task) {
		// дождаться sem0>0
		struct sembuf op{0, -1, 0};
		semop(semid_, &op, 1);
		std::memcpy(mem_, &task, sizeof(task));
		// сигнал sem1
		op = {1, 1, 0};
		semop(semid_, &op, 1);
	}

	CompileTask read() {
		// дождаться sem1>0
		struct sembuf op{1, -1, 0};
		semop(semid_, &op, 1);
		CompileTask t;
		std::memcpy(&t, mem_, sizeof(t));
		// сигнал sem0
		op = {0, 1, 0};
		semop(semid_, &op, 1);
		return t;
	}

private:
	int shmid_, semid_;
	char* mem_;
};

#endif // SHAREDMEMSEM_HPP
