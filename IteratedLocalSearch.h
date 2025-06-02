//
// Created by thefo on 1/24/2025.
//

#ifndef ITERATEDLOCALSEARCH_H
#define ITERATEDLOCALSEARCH_H



#include <vector>
#include <unordered_set>
#include <random>
#include "Solution.h"
#include "Car.h"

struct VectorHash {
    std::size_t operator()(const std::vector<int>& v) const {
        std::size_t hash = 0;
        for (int i : v) {
            hash ^= std::hash<int>{}(i) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
        return hash;
    }
};

void perturbacion(std::vector<int>& tour, std::unordered_set<std::vector<int>, VectorHash>& history);
Solution perturbacion_cars(Solution solucion, std::vector<Car>& cars, std::mt19937& rd, int& intentos_sin_mejora);
int pertubacion_aux(std::vector<int>& v, int current_car, int init_i);
void extend_route(Solution& solucion, int extender[]);
Solution PerturbacionCars(Solution solucion, std::mt19937& g);



#endif //ITERATEDLOCALSEARCH_H
