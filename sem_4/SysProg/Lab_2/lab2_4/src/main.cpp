// main.cpp
#include "../include/logger.hpp"
#include "../include/elevator_system.hpp"
#include <memory>

int main() {
	// настраиваем логгер
	LoggerBuilder builder;
	//builder.setLevel(Logger::Level::WARNING);
	builder.setName("ElevatorSim")
		   .setLevel(Logger::Level::INFO)
		   .addHandler(std::make_unique<StreamLoggerHandler>(std::cout));
	auto* logger = builder.build();

	// параметры симуляции
	int numElevators = 3;
	int maxFloor     = 10;
	int ticks        = 20;

	ElevatorSystem system(numElevators, maxFloor);
	system.simulate(ticks);
	system.printStatistics();

	// завершаем логирование
	g_logger->close();
	delete logger;
	return 0;
}