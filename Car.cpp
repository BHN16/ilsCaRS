//
// Created by thefo on 1/24/2025.
//

#include "Car.h"

#include <utility>
#include <iostream>

Car::Car(const int id_, std::vector<std::vector<int>> travelMatrix_) {
    this->id = id_;
    this->travelMatrix = std::move(travelMatrix_);
    this->used = false;
}
int Car::getId() const {
    return this->id;
}

bool Car::getUsed() const {
    return this->used;
}

void Car::setUsed(const bool _used) {
    this->used = _used;
}

std::vector<std::vector<int>> Car::getTravelMatrix() {
    return this->travelMatrix;
}

std::vector<std::vector<int>> Car::getReturnCostMatrix() {
    return this->returnCostMatrix;
}

void Car::setReturnCostMatrix(std::vector<std::vector<int> > returnCostMatrix_) {
    this->returnCostMatrix = std::move(returnCostMatrix_);
}

void Car::printTravelMatrix() const {
    std::cout << "Matriz de viaje del vehículo " << id << ":\n";
    for (const auto &row : travelMatrix) {
        for (int val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
}

void Car::printReturnCostMatrix() const {
    std::cout << "Matriz de costo de retorno del vehículo " << id << ":\n";
    for (const auto &row : returnCostMatrix) {
        for (int val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
}