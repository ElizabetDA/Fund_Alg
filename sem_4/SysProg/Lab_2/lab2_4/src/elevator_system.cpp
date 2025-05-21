// elevator_system.cpp
#include "../include/elevator_system.hpp"
#include "../include/logger.hpp"
#include <random>
#include <algorithm>
#include <iostream>

using namespace std;

ElevatorSystem::ElevatorSystem(int numElevators, int maxFloor)
  : maxFloor_(maxFloor)
{
	mt19937 gen(random_device{}());
	uniform_int_distribution<> capDist(2, 8);
	for (int i = 0; i < numElevators; ++i) {
		elevators_.emplace_back(i, capDist(gen), maxFloor_);
	}
}

void ElevatorSystem::simulate(int ticks) {
	mt19937 gen(random_device{}());
	uniform_int_distribution<> personDist(1, 5);
	uniform_int_distribution<> floorDist(0, maxFloor_);

	for (int t = 0; t < ticks; ++t) {
		int calls = personDist(gen);
		for (int i = 0; i < calls; ++i) {
			Passenger p{ nextId_++, floorDist(gen), floorDist(gen) };
			getLeastBusyElevator().call(p);
		}
		for (auto& e : elevators_) {
			e.tick();
		}
	}
}

Elevator& ElevatorSystem::getLeastBusyElevator() {
	return *std::min_element(
		elevators_.begin(), elevators_.end(),
		[](const Elevator& a, const Elevator& b) {
			return a.currentFloor() < b.currentFloor();
		}
	);
}

void ElevatorSystem::printStatistics() const {
	int totalP = 0, totalF = 0;
	std::cout << "=== Statistics ===\n";
	for (const auto& e : elevators_) {
		std::cout << "Elevator " << e.id()
				  << ": transported " << e.totalPassengers()
				  << " pax, traveled " << e.totalFloorsTraveled()
				  << " floors\n";
		totalP += e.totalPassengers();
		totalF += e.totalFloorsTraveled();
	}
	std::cout << "Total passengers: " << totalP << "\n"
			  << "Total floors:     " << totalF << "\n";
}