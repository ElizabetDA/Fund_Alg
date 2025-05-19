#pragma once

#include "blocking_queue.hpp"
#include "traffic_types.hpp"
#include <map>
#include <mutex>
#include <thread>
#include <vector>

struct Stats {
    size_t total_sent   = 0;
    size_t total_recv   = 0;
    size_t conn_count   = 0;
    std::map<uint16_t, size_t> by_port;
};

class TrafficAnalyzer {
public:
    TrafficAnalyzer(BlockingQueue<tcp_traffic_pkg>& q)
      : queue_(q), num_threads_(0) {}

    // Запускаем N потоков-аналитиков
    void start(int threads = 4) {
        num_threads_ = threads;
        locals_.clear();
        for(int i = 0; i < threads; ++i) {
            workers_.emplace_back([this]{ run(); });
        }
    }

    // Отправить «poison-pill» каждому потоку, чтобы он вышел из pop()
    void stop() {
        tcp_traffic_pkg pill{};
        pill.ev = tcp_traffic_pkg::Event::STOP;
        for(int i = 0; i < num_threads_; ++i) {
            queue_.push(pill);
        }
    }

    // Ждём завершения
    void join() {
        for(auto& t : workers_) if (t.joinable()) t.join();
        workers_.clear();
    }

    // Получить все локальные статистики
    const std::vector<Stats>& get_locals() const { return locals_; }

    // Объединить в глобальную
    static void merge_all(const std::vector<Stats>& locals, Stats& global) {
        for(auto& st : locals) {
            global.total_sent += st.total_sent;
            global.total_recv += st.total_recv;
            global.conn_count += st.conn_count;
            for(auto& kv : st.by_port)
                global.by_port[kv.first] += kv.second;
        }
    }

private:
    void run() {
        Stats local;
        while (true) {
            auto pkg = queue_.pop();
            if (pkg.ev == tcp_traffic_pkg::Event::STOP)
                break;

            switch(pkg.ev) {
                case tcp_traffic_pkg::Event::CONNECT:
                    local.conn_count++;
                    break;
                case tcp_traffic_pkg::Event::SEND:
                    local.total_sent += pkg.sz;
                    local.by_port[ntohs(pkg.dst_port)] += pkg.sz;
                    break;
                case tcp_traffic_pkg::Event::RECV:
                    local.total_recv += pkg.sz;
                    local.by_port[ntohs(pkg.dst_port)] += pkg.sz;
                    break;
                case tcp_traffic_pkg::Event::DISCONNECT:
                    // можно отличать внезапные/запланированные, если нужно
                    break;
                default: break;
            }
        }

        // сохраняем свою локальную статистику
        std::lock_guard<std::mutex> lk(mutex_);
        locals_.push_back(std::move(local));
    }

    BlockingQueue<tcp_traffic_pkg>& queue_;
    int                              num_threads_;
    std::vector<std::thread>         workers_;
    std::vector<Stats>               locals_;
    std::mutex                       mutex_;
};
