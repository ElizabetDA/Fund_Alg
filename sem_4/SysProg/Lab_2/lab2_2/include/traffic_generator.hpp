#pragma once

#include "blocking_queue.hpp"
#include "logger.hpp"
#include "traffic_types.hpp"
#include <random>
#include <thread>
#include <chrono>
#include <vector>
#include <atomic>
#include <arpa/inet.h>

class TrafficGenerator {
public:
    TrafficGenerator(BlockingQueue<tcp_traffic_pkg>& q, Logger* logger, int seed = 0)
      : queue_(q), log_(logger), rnd_(seed) {}

    // Запускаем N потоков-генераторов
    void start(int threads = 4) {
        stop_.store(false);
        for(int i = 0; i < threads; ++i) {
            workers_.emplace_back([this, i]{ run(i); });
        }
    }

    // Попросить остановиться
    void stop() {
        stop_.store(true);
    }

    // Ждём завершения всех потоков
    void join() {
        for(auto& t : workers_) if (t.joinable()) t.join();
        workers_.clear();
    }

private:
    void run(int id) {
        std::uniform_int_distribution<int> port_gen(1024, 65535);
        std::uniform_int_distribution<int> size_gen(1, 1500);
        std::uniform_int_distribution<int> event_gen(0, 3);
        std::uniform_int_distribution<uint32_t> ip_gen(1, 0xFFFFFFFF);

        while (!stop_.load()) {
            tcp_traffic_pkg p;
            p.ev        = static_cast<tcp_traffic_pkg::Event>(event_gen(rnd_));
            p.src_port  = port_gen(rnd_);
            p.dst_port  = port_gen(rnd_);
            p.dst_addr  = htonl(ip_gen(rnd_));
            p.sz        = size_gen(rnd_);

            // логируем
            in_addr a; a.s_addr = p.dst_addr;
            char buf[128];
            std::snprintf(buf, sizeof(buf),
                "Gen[%d] %s %s:%u -> port %u sz=%zu",
                id,
                (p.ev==tcp_traffic_pkg::Event::CONNECT?"CONN":
                 p.ev==tcp_traffic_pkg::Event::SEND?   "SEND":
                 p.ev==tcp_traffic_pkg::Event::RECV?   "RECV":"DISC"),
                inet_ntoa(a), ntohs(p.src_port), ntohs(p.dst_port), p.sz);
            log_->log(Logger::Level::DEBUG, buf);

            queue_.push(p);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    BlockingQueue<tcp_traffic_pkg>& queue_;
    Logger*                          log_;
    std::mt19937                     rnd_;
    std::vector<std::thread>         workers_;
    std::atomic<bool>                stop_{false};
};
