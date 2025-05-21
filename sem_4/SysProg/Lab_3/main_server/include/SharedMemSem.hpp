#ifndef SHAREDMEMSEM_HPP
#define SHAREDMEMSEM_HPP

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdexcept>
#include <cstring>
#include <cerrno>

// структура задачи компиляции
struct CompileTask {
    int client_id;
    char filename[256];
};

class SharedMemSem {
public:
    SharedMemSem(key_t key, size_t size) {
        // создаём разделяемую память
        shmid_ = shmget(key, size, IPC_CREAT | 0666);
        if (shmid_ == -1)
            throw std::runtime_error(std::string("shmget failed: ") + std::strerror(errno));

        // создаём 2-семафорную группу: sem0 и sem1
        semid_ = semget(key, 2, IPC_CREAT | 0666);
        if (semid_ == -1)
            throw std::runtime_error(std::string("semget failed: ") + std::strerror(errno));

        // подключаем SHM
        mem_ = static_cast<char*>(shmat(shmid_, nullptr, 0));
        if (mem_ == reinterpret_cast<char*>(-1))
            throw std::runtime_error(std::string("shmat failed: ") + std::strerror(errno));

        // Инициализация:
        // sem0 = 1 (есть один «пустой» слот),
        // sem1 = 0 (пустых «полных» слотов нет)
        if (semctl(semid_, 0, SETVAL, 1) == -1 ||
            semctl(semid_, 1, SETVAL, 0) == -1)
        {
            throw std::runtime_error(std::string("semctl SETVAL failed: ") + std::strerror(errno));
        }
    }

    ~SharedMemSem() {
        shmdt(mem_);
    }

    // запись задачи (сервер)
    void write(const CompileTask& task) {
        // ждем sem0 > 0 (пустой слот)
        struct sembuf p{0, -1, 0};
        semop(semid_, &p, 1);
        // пишем задачу
        std::memcpy(mem_, &task, sizeof(task));
        // сигналим sem1 (теперь есть «полный» слот)
        struct sembuf v{1, +1, 0};
        semop(semid_, &v, 1);
    }

    // чтение задачи (подсервер)
    CompileTask read() {
        // ждём sem1 > 0 (появился «полный» слот)
        struct sembuf p{1, -1, 0};
        semop(semid_, &p, 1);
        // читаем задачу
        CompileTask t;
        std::memcpy(&t, mem_, sizeof(t));
        // сигналим sem0 (освободился «пустой» слот)
        struct sembuf v{0, +1, 0};
        semop(semid_, &v, 1);
        return t;
    }

private:
    int shmid_{-1}, semid_{-1};
    char* mem_{nullptr};
};

#endif // SHAREDMEMSEM_HPP
