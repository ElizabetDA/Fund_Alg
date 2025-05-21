// main.cpp
#include "../include/blocking_queue.hpp"
#include "../include/traffic_generator.hpp"
#include "../include/traffic_analyzer.hpp"
#include "../include/logger.hpp"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
	// 1) Создаём логгер
	Logger* log = Logger::Builder()
					  .setName("netgen")
					  .setLevel(Logger::Level::DEBUG)
					  .addHandler(
						  std::unique_ptr<LogHandler>(
							  new FileLoggerHandler("traffic.log")))
					  .build();

	// 2) Общая очередь
	BlockingQueue<tcp_traffic_pkg> queue;

	// 3) Генератор и анализатор
	TrafficGenerator generator(queue, log, /*seed=*/42);
	TrafficAnalyzer analyzer(queue);

	// 4) Стартуем
	constexpr int GEN_THREADS = 4;
	constexpr int ANZ_THREADS = 4;
	generator.start(GEN_THREADS);
	analyzer.start(ANZ_THREADS);

	// 5) Пусть поработают 10 секунд
	std::this_thread::sleep_for(std::chrono::seconds(10));

	// 6) Останавливаем генератор (флаги), ждём его завершения
	generator.stop();
	generator.join();

	// 7) Отправляем «заглушку» для аналитиков и ждём их
	analyzer.stop();
	analyzer.join();

	// 8) Собираем и выводим финальную статистику
	auto locals = analyzer.get_locals();
	Stats global;
	TrafficAnalyzer::merge_all(locals, global);

	std::cout << "=== GLOBAL STATS ===\n"
			  << "Total sent:      " << global.total_sent  << "\n"
			  << "Total received:  " << global.total_recv  << "\n"
			  << "Connections:     " << global.conn_count  << "\n";
	for(auto& kv : global.by_port)
		std::cout << " Port " << kv.first << ": " << kv.second << " bytes\n";

	log->close();
	delete log;
	return 0;
}
