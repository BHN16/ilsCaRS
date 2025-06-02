//
// Created by thefo on 1/24/2025.
//

#ifndef SOLUTION_H
#define SOLUTION_H



#include <vector>
#include "Car.h"


class Solution {
public:
    std::vector<int> route;
    std::vector<int> vehicles;
    int totalCost;
    Solution();
    Solution(const std::vector<int> &route, const std::vector<int> &vehicles, int& totalCost);
    int calcularCosto(std::vector<Car>& cars);
    static int calcularCosto(std::vector<Car>& cars, const std::vector<int> &vehicles, const std::vector<int> &route);
    void print() const;
    static void print(const std::vector<int> &route, const std::vector<int> &vehicles);
};



#endif //SOLUTION_H
