// include/elevator.hpp
#pragma once
#include <vector>
#include <queue>
#include "passenger.hpp"
#include "logger.hpp"

class Elevator {
public:
	Elevator(int id, int capacity, int maxFloor);

	bool isIdle() const;
	int currentFloor() const;
	int id() const;
	int capacity() const;

	void call(const Passenger& p);
	void tick();

	// статистика
	int totalPassengers() const;
	int totalFloorsTraveled() const;

private:
	void openDoors();
	void loadPassengers();
	void closeDoors();
	void move();
	int chooseNextFloor();

private:
	int id_;
	int capacity_;
	int currentFloor_;
	int maxFloor_;

	std::vector<Passenger> passengers_;
	std::queue<Passenger> requests_;

	int servedCount_     = 0;
	int floorsTraveled_  = 0;
};
