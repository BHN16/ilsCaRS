//
// Created by thefo on 1/24/2025.
//

#include "IteratedLocalSearch.h"
#include <unordered_set>
#include <algorithm>
#include <iostream>
#include <ostream>
#include <random>
#include <vector>
#include <ctime>
#include "Solution.h"
#include "Car.h"


void perturbacion(std::vector<int>& tour, std::unordered_set<std::vector<int>, VectorHash>& history) {
    tour.pop_back();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, tour.size()-1);
    std::vector<int> posiciones;

    posiciones.push_back(distr(gen));
    posiciones.push_back(distr(gen));
    posiciones.push_back(distr(gen));
    std::sort(posiciones.begin(), posiciones.end());

    std::vector<int> newTour;
    newTour.insert(newTour.end(), tour.begin(), tour.begin() + posiciones[0]);                    // Parte 1
    newTour.insert(newTour.end(), tour.begin() + posiciones[2], tour.end());                      // Parte 4
    newTour.insert(newTour.end(), tour.begin() + posiciones[1], tour.begin() + posiciones[2]);    // Parte 3
    newTour.insert(newTour.end(), tour.begin() + posiciones[0], tour.begin() + posiciones[1]);    // Parte 2

    // Asegurar que el tour sigue siendo un ciclo
    newTour.push_back(newTour[0]);

    // Verificar si se cumple el caso
    if (history.find(newTour) != history.end()) {
        perturbacion(tour, history);
    } else {
        tour = newTour;
    }
}

int pertubacion_aux(std::vector<int>& v, int current_car, int init_i) {
    for(int i = init_i;  i < v.size(); i++) {
        if (v[i] != current_car) {
            return i;
        }
    }
    return -1;
}

void extend_route(Solution& solucion, int extender[]) {
    for (int i = 0; i < solucion.vehicles.size(); i++) {
        if (solucion.vehicles[i] == extender[0]) {
            int cont = extender[1];
            while (cont >= 0) {
                solucion.vehicles[i] = extender[0];
                i++;
                cont--;
            }
        }
    }
}

std::vector<std::pair<int, std::vector<int>>> find_sequences(const std::vector<int>& vec) {
    std::vector<std::pair<int, std::vector<int>>> sequences;
    int n = vec.size();

    int start = 0;
    while (start < n) {
        int current_value = vec[start];
        std::vector<int> indices;
        indices.push_back(start);
        while (start + 1 < n && vec[start + 1] == current_value) {
            start++;
            indices.push_back(start);
        }
        sequences.emplace_back(current_value, indices);
        start++;
    }

    return sequences;
}

std::vector<int> swap_random_sequence(std::vector<int>& vec, std::mt19937& g) {
    auto sequences = find_sequences(vec);
    std::vector<std::vector<int>> new_sequences;
    for (const auto& sequence : sequences) {
        std::vector<int> new_sequence(sequence.second.size(), sequence.first);
        new_sequences.emplace_back(new_sequence);
    }
    for (int i = 0; i < 10; i++) {
        std::shuffle(new_sequences.begin(), new_sequences.end(), g);
    }
    std::vector<int> concatenated;
    for (const auto& sequence : new_sequences) {
        concatenated.insert(concatenated.end(), sequence.begin(), sequence.end());
    }
    return concatenated;
}

int get_unused_car(std::vector<Car>& cars) {
    for (auto& car : cars) {
        if (!car.getUsed()) {
            car.setUsed(true);
            return car.getId();
        }
    }
    return -1;
}

void add_car(std::vector<int>& vehicles, const int car_id) {
    bool changed = false;
    const int n = vehicles.size();
    for(int i = 1; i < n-1; i++){
        if(vehicles[i] != vehicles[i+1] && vehicles[i-1] == vehicles[i]) {
            vehicles[i] = car_id;
            changed = true;
            break;
        }
    }
    if(!changed && vehicles[n-1] == vehicles[n-2]) {
        vehicles[n-1] = car_id;
    }
}

void remove_car(std::vector<int>& vehicles, std::vector<Car>& cars) {
    const int n = vehicles.size();
    for(int i = 0; i < n-1; i++){
        if(vehicles[i] != vehicles[i+1]) {
            int curren_vehicle = vehicles[i];
            int remove_vehicle = vehicles[i+1];
            i++;
            vehicles[i] = curren_vehicle;
            while(i < n-1 && remove_vehicle == vehicles[i+1]) {
                i++;
                vehicles[i] = curren_vehicle;
            }
            for (auto& car : cars) {
                if (car.getId() == remove_vehicle) {
                    car.setUsed(false);
                    break;
                }
            }
            break;
        }
    }
}

Solution perturbacion_cars(Solution solucion, std::vector<Car>& cars, std::mt19937& rd, int& intentos_sin_mejora) {
    if (intentos_sin_mejora > 10) {
        int carro_extra = get_unused_car(cars);
        if (carro_extra >= 0) {
            add_car(solucion.vehicles, carro_extra);
        } else {
            remove_car(solucion.vehicles, cars);
        }
        solucion.calcularCosto(cars);
        return solucion;
    }
    auto swapped_vehicles = swap_random_sequence(solucion.vehicles, rd);
    solucion.vehicles = swapped_vehicles;
    solucion.calcularCosto(cars);
    return solucion;
    /*int current_car = solucion.vehicles[0];
    int init_i = 0;
    int perturbacion[3] = {current_car, 0, solucion.totalCost};
    while(init_i != -1) {
        Solution local = mejorSolucion;
        int perturbacion_optima[3] = {current_car, 0, solucion.totalCost};
        int cont = 0;
        for (int i = init_i; i < cars.size(); i++) {
            local.vehicles[i] = current_car;
            if (local.calcularCosto(cars) < perturbacion_optima[2]) {
                perturbacion_optima[1] = cont;
                perturbacion_optima[2] = local.calcularCosto(cars);
            }
            cont++;
        }
        if (perturbacion_optima[2] < perturbacion[2]) {
            perturbacion[0] = perturbacion_optima[0];
            perturbacion[1] = perturbacion_optima[1];
            perturbacion[2] = perturbacion_optima[2];
        }
        init_i = pertubacion_aux(mejorSolucion.vehicles, current_car, init_i);
        current_car = solucion.vehicles[init_i];
    }
    extend_route(mejorSolucion, perturbacion);
    mejorSolucion.totalCost = mejorSolucion.calcularCosto(cars);*/
}

Solution PerturbacionCars(Solution solucion, std::mt19937& g) {
    auto route = solucion.route;
    int partSize = route.size() / 4;
    // Definir los subvectores
    std::vector<int> v1(route.begin(), route.begin() + partSize);
    std::vector<int> v2(route.begin() + partSize, route.begin() + 2 * partSize);
    std::vector<int> v3(route.begin() + 2 * partSize, route.begin() + 3 * partSize);
    std::vector<int> v4(route.begin() + 3 * partSize, route.end());
    route.clear();
    std::vector vectores = {&v1, &v2, &v3, &v4};
    std::shuffle(vectores.begin(), vectores.end(), g);
    for (auto v : vectores) {
        route.insert(route.end(), v->begin(), v->end());
    }
    solucion.route = route;
    solucion.vehicles = swap_random_sequence(solucion.vehicles, g);
    return solucion;
}