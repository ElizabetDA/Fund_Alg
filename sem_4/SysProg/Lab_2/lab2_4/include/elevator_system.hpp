// include/elevator_system.hpp
#pragma once
#include <vector>
#include "elevator.hpp"

class ElevatorSystem {
public:
	ElevatorSystem(int numElevators, int maxFloor);
	void simulate(int ticks);
	void printStatistics() const;

private:
	Elevator& getLeastBusyElevator();

	std::vector<Elevator> elevators_;
	int maxFloor_;
	int nextId_ = 0;
};
