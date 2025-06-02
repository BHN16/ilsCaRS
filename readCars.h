//
// Created by thefo on 1/24/2025.
//

#ifndef READCARS_H
#define READCARS_H


#include <iostream>
#include <vector>
#include <string>
#include <random>
#include "Car.h"
#include "Solution.h"

std::vector<std::vector<int>> readMatrix(std::ifstream &file, int dimension);
std::pair<int, int> findNextCity(std::vector<Car> &cars, int currentCity, int currentVehicle, std::vector<bool> &visited, int cityToReturn);
void read_graphs(std::vector<Car> &cars, int& dimension, std::string instancia);
void initial_solution(std::vector<Car> &cars, int &dimension, Solution &solution, std::mt19937& g);


#endif //READCARS_H
