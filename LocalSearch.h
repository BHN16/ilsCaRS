//
// Created by thefo on 1/24/2025.
//

#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H



#include <vector>
#include <iostream>
#include "Solution.h"
#include "Car.h"

std::vector<int> solucion_inicial(const std::vector<std::vector<double>>& dist);
double calcular_costo(const std::vector<int>& tour, const std::vector<std::vector<double>>& graph);
void dos_opt_swap(std::vector<int>& path, int i, int k);
Solution local_search_cars(std::vector<Car>& cars, int dimension, Solution solucion_inicial);
Solution LocalSearchCars(std::vector<Car>& cars, int dimension, Solution solucion_inicial);
std::vector<int> local_search(const std::vector<std::vector<double>>& graph, std::vector<int> path);

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    os << "Size: " << v.size() << " - [";
    for (int i = 0; i < v.size(); ++i) {
        os << v[i];
        if (i != v.size() - 1)
            os << ", ";
    }
    os << "]\n";
    return os;
}



#endif //LOCALSEARCH_H
