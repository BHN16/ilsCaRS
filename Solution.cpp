//
// Created by thefo on 1/24/2025.
//

#include "Solution.h"
#include <iostream>
#include <ostream>
#include <vector>
#include <iomanip>

Solution::Solution() {
    this->totalCost = 0;
    this->route = {};
    this->vehicles = {};
}

Solution::Solution(const std::vector<int> &route, const std::vector<int> &vehicles, int& totalCost) {
    this->route = route;
    this->vehicles = vehicles;
    this->totalCost = totalCost;
}

int Solution::calcularCosto(std::vector<Car>& cars)  {
    int costo = 0;
    int return_city = this->route[0];
    for(int i = 0; i < this->route.size()-1; i++) {
        int current_vehicle = this->vehicles[i];
        int next_vehicle = this->vehicles[i+1];
        int current_city = this->route[i];
        int next_city = this->route[i+1];
        costo += cars[current_vehicle].getTravelMatrix()[current_city][next_city];
        if (current_vehicle != next_vehicle) {
            costo += cars[current_vehicle].getReturnCostMatrix()[next_city][return_city];
            return_city = next_city;
        }
    }
    costo += cars[this->vehicles.back()].getTravelMatrix()[this->route.back()][this->route[0]];
    costo += cars[this->vehicles.back()].getReturnCostMatrix()[this->route[0]][return_city];
    this->totalCost = costo;
    return costo;
}

int Solution::calcularCosto(std::vector<Car>& cars, const std::vector<int> &vehicles, const std::vector<int> &route) {
    int costo = 0;
    int ciudad_retorno = route[0];
    for(int i = 0; i < route.size()-1; i++) {
        costo += cars[vehicles[i]].getTravelMatrix()[route[i]][route[i+1]];
        if (vehicles[i] != vehicles[i+1]) {
            costo += cars[vehicles[i]].getReturnCostMatrix()[route[i+1]][ciudad_retorno];
            ciudad_retorno = route[i+1];
        }
    }
    costo += cars[vehicles[route.size()-1]].getTravelMatrix()[route[route.size()-1]][route[0]];
    costo += cars[vehicles[route.size()-1]].getReturnCostMatrix()[route[0]][ciudad_retorno];
    return costo;
}

void Solution::print() const {
    for (const int i : this->route) {
        std::cout << std::setw(3) << i << " ";
    }
    std::cout << std::endl;
    for (const int vehicle : this->vehicles) {
        std::cout << std::setw(3) << vehicle << " ";
    }
    std::cout << std::endl;
}

void Solution::print(const std::vector<int> &route, const std::vector<int> &vehicles) {
    for (const int i : route) {
        std::cout << std::setw(3) << i << " ";
    }
    std::cout << std::endl;
    for (const int vehicle : vehicles) {
        std::cout << std::setw(3) << vehicle << " ";
    }
    std::cout << std::endl;
}