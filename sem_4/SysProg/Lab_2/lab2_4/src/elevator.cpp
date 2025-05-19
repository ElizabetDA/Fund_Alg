// elevator.cpp
#include "../include/elevator.hpp"
#include <thread>
#include <chrono>
#include <string>

using namespace std;

Elevator::Elevator(int id, int capacity, int maxFloor)
  : id_(id)
  , capacity_(capacity)
  , currentFloor_(0)
  , maxFloor_(maxFloor)
{}

bool Elevator::isIdle() const {
    return passengers_.empty() && requests_.empty();
}

int Elevator::currentFloor() const {
    return currentFloor_;
}

int Elevator::id() const {
    return id_;
}

int Elevator::capacity() const {
    return capacity_;
}

void Elevator::call(const Passenger& p) {
    g_logger->log(Logger::Level::INFO,
        string("Elevator ") + to_string(id_) +
        ": call from " + to_string(p.from) +
        " to "       + to_string(p.to));
    requests_.push(p);
}

void Elevator::tick() {
    if (passengers_.empty() && requests_.empty()) return;

    openDoors();
    loadPassengers();
    closeDoors();
    move();
}

void Elevator::openDoors() {
    g_logger->log(Logger::Level::INFO,
        string("Elevator ") + to_string(id_) +
        " opening doors at floor " + to_string(currentFloor_));
    this_thread::sleep_for(chrono::milliseconds(100)); // 1 тик
}

void Elevator::loadPassengers() {
    // высадка
    for (auto it = passengers_.begin(); it != passengers_.end();) {
        if (it->to == currentFloor_) {
            g_logger->log(Logger::Level::INFO,
                string("Elevator ") + to_string(id_) +
                ": passenger " + to_string(it->id) +
                " leaves at " + to_string(currentFloor_));
            it = passengers_.erase(it);
            ++servedCount_;
        } else ++it;
    }
    // посадка
    queue<Passenger> leftover;
    while (!requests_.empty()) {
        const auto& p = requests_.front();
        if (p.from == currentFloor_ && int(passengers_.size()) < capacity_) {
            passengers_.push_back(p);
            g_logger->log(Logger::Level::INFO,
                string("Elevator ") + to_string(id_) +
                ": passenger " + to_string(p.id) +
                " enters to " + to_string(p.to));
        } else {
            leftover.push(p);
        }
        requests_.pop();
    }
    requests_ = std::move(leftover);
    this_thread::sleep_for(chrono::milliseconds(100)); // 1 тик
}

void Elevator::closeDoors() {
    g_logger->log(Logger::Level::INFO,
        string("Elevator ") + to_string(id_) +
        " closing doors at floor " + to_string(currentFloor_));
    this_thread::sleep_for(chrono::milliseconds(100)); // 1 тик
}

void Elevator::move() {
    int target = chooseNextFloor();
    if (target == currentFloor_) return;

    this_thread::sleep_for(chrono::milliseconds(200)); // 2 тика
    if (target > currentFloor_) {
        ++currentFloor_;
        g_logger->log(Logger::Level::INFO,
            string("Elevator ") + to_string(id_) +
            " up to " + to_string(currentFloor_));
    } else {
        --currentFloor_;
        g_logger->log(Logger::Level::INFO,
            string("Elevator ") + to_string(id_) +
            " down to " + to_string(currentFloor_));
    }
    ++floorsTraveled_;
}

int Elevator::chooseNextFloor() {
    if (!passengers_.empty())
        return passengers_.front().to;
    if (!requests_.empty())
        return requests_.front().from;
    return currentFloor_;
}

int Elevator::totalPassengers() const {
    return servedCount_;
}

int Elevator::totalFloorsTraveled() const {
    return floorsTraveled_;
}
