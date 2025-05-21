#pragma once

#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdexcept>

class SemaphoreManager {
public:
	enum Sem { MUTEX = 0, MSGSEM = 1, LOGSEM = 2 };

	explicit SemaphoreManager(key_t key) {
		semid_ = semget(key, 3, IPC_CREAT | 0666);
		if (semid_ < 0) throw std::runtime_error("semget failed");
		semctl(semid_, MUTEX,  SETVAL, 1);
		semctl(semid_, MSGSEM, SETVAL, 0);
		semctl(semid_, LOGSEM, SETVAL, 1);
	}

	void down(Sem s) {
		struct sembuf op;
		op.sem_num = static_cast<unsigned short>(s);
		op.sem_op  = static_cast<short>(-1);
		op.sem_flg = 0;
		semop(semid_, &op, 1);
	}

	void up(Sem s) {
		struct sembuf op;
		op.sem_num = static_cast<unsigned short>(s);
		op.sem_op  = static_cast<short>(+1);
		op.sem_flg = 0;
		semop(semid_, &op, 1);
	}

private:
	int semid_;
};
