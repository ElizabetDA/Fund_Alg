#include "../include/chat_common.hpp"
#include "../include/shared_memory_manager.hpp"
#include "../include/semaphore_manager.hpp"
#include "../include/logger.hpp"

#include <thread>
#include <atomic>
#include <iostream>
#include <cstring>

int main() {
    auto* logger = Logger::Builder()
        .setName("ChatClient")
        .setLevel(Logger::Level::INFO)
        .addHandler(std::make_unique<StreamLoggerHandler>(std::cout))
        .addHandler(std::make_unique<FileLoggerHandler>("client.log"))
        .build();

    key_t key = ftok("/tmp", 65);
    SharedMemoryManager shm(key);
    SemaphoreManager   sem(key);

    // — Регистрация —
    sem.down(SemaphoreManager::MUTEX);
    auto* hdr       = shm.header();
    int   client_id = ++hdr->client_count;
    int   hist_end  = hdr->write_index;  // сколько уже сообщений в буфере
    sem.up(SemaphoreManager::MUTEX);

    logger->log(Logger::Level::INFO,
                "Connected as client #" + std::to_string(client_id));

    // — Выводим историю (все m от 0 до hist_end) —
    {
        auto* buf = shm.buffer();
        for (int i = 0; i < hist_end; ++i) {
            const Message& m = buf[i % CHAT_BUF_SIZE];
            if (m.from_id != client_id) {
                std::cout << "[History] Client " << m.from_id
                          << ": " << m.text << "\n";
                logger->log(Logger::Level::INFO,
                            "History Received: " + std::string(m.text));
            }
        }
    }

    // Теперь ставим last_read в конец истории
    std::atomic<int> last_read{ hist_end };
    std::atomic<bool> running{ true };

    // — Приёмник —
    std::thread receiver([&](){
        while (running) {
            sem.down(SemaphoreManager::MSGSEM);

            sem.down(SemaphoreManager::MUTEX);
            int curr_write = shm.header()->write_index;
            sem.up(SemaphoreManager::MUTEX);

            while (last_read < curr_write) {
                const Message& m = shm.buffer()[(last_read++) % CHAT_BUF_SIZE];
                if (m.from_id != client_id) {
                    std::cout << "Client " << m.from_id
                              << ": " << m.text << "\n";
                    logger->log(Logger::Level::INFO,
                                "Received: " + std::string(m.text));
                }
            }
        }
    });

    // — Отправитель —
    std::thread sender([&](){
        std::string line;
        while (running && std::getline(std::cin, line)) {
            sem.down(SemaphoreManager::MUTEX);
            auto* hdr = shm.header();
            int idx = hdr->write_index % CHAT_BUF_SIZE;

            auto* buf = shm.buffer();
            buf[idx].from_id = client_id;
            std::strncpy(buf[idx].text, line.c_str(), CHAT_TEXT_SIZE-1);
            buf[idx].text[CHAT_TEXT_SIZE-1] = '\0';

            hdr->write_index++;
            sem.up(SemaphoreManager::MUTEX);

            // Уведомить всех остальных
            sem.down(SemaphoreManager::MUTEX);
            int cnt = hdr->client_count;
            sem.up(SemaphoreManager::MUTEX);
            for (int i = 0; i < cnt - 1; ++i) {
                sem.up(SemaphoreManager::MSGSEM);
            }

            logger->log(Logger::Level::INFO, "Sent: " + line);
        }
        running = false;
    });

    sender.join();
    receiver.join();
    delete logger;
    return 0;
}