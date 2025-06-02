//
// Created by thefo on 1/24/2025.
//

#include "LocalSearch.h"
#include <algorithm>
#include <vector>
#include <limits>
#include <map>
#include <random>
#include "Solution.h"
#include "Car.h"


std::vector<int> solucion_inicial(const std::vector<std::vector<double>>& graph) {
    std::vector<int> camino;
    auto visited = std::vector<int>(graph.size(), false) ;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, graph.size() - 1);

    int inicio = dis(gen);
    camino.push_back(inicio);
    visited[inicio] = true;
    int numVisitados = 1;

    while (numVisitados < graph.size()) {
        int destino = dis(gen);
        if (!visited[destino]) {
            camino.push_back(destino);
            visited[destino] = true;
            numVisitados++;
        }
    }
    camino.push_back(inicio);
    return camino;
}


double calcular_costo(const std::vector<int>& tour, const std::vector<std::vector<double>>& graph) {
    double totalCost = 0;
    for (size_t i = 0; i < tour.size() - 1; ++i) {
        totalCost += graph[tour[i]][tour[i + 1]];
    }
    totalCost += graph[tour.back()][tour[0]];
    return totalCost;
}


void dos_opt_swap(std::vector<int>& tour, const int i, const int k) {
    std::reverse(tour.begin() + i, tour.begin() + k + 1);
}


void swap_random_sequence2(std::vector<int>& vec) {
    int partSize = vec.size() / 4;
    // Definir los subvectores
    std::vector<int> part0(vec.begin(), vec.begin() + partSize);
    std::vector<int> part1(vec.begin() + partSize, vec.begin() + 2 * partSize);
    std::vector<int> part2(vec.begin() + 2 * partSize, vec.begin() + 3 * partSize);
    std::vector<int> part3(vec.begin() + 3 * partSize, vec.end());
    vec.clear();
    std::copy(part2.begin(), part2.end(), std::back_inserter(vec));
    std::copy(part3.begin(), part3.end(), std::back_inserter(vec));
    std::copy(part0.begin(), part0.end(), std::back_inserter(vec));
    std::copy(part1.begin(), part1.end(), std::back_inserter(vec));
}


std::vector<int> local_search(const std::vector<std::vector<double>>& graph, std::vector<int> tour) {
    const int n = graph.size();

    bool mejorado = true;
    int iteraciones = 0;
    int max_iteraciones = 1000;

    while (mejorado && iteraciones < max_iteraciones) {
        mejorado = false;
        double mejorCosto = calcular_costo(tour, graph);

        for (int i = 0; i < n - 1; ++i) {
            for (int k = i + 1; k < n; ++k) {
                dos_opt_swap(tour, i, k);
                double nuevoCosto = calcular_costo(tour, graph);
                if (nuevoCosto < mejorCosto) {
                    mejorCosto = nuevoCosto;
                    mejorado = true;
                } else {
                    dos_opt_swap(tour, i, k); // Revertir si no es mejor
                }
            }
        }
        iteraciones++;
    }
    // Completar el ciclo
    tour.push_back(tour[0]);
    return tour;
}


std::vector<std::pair<int, int>> encontrarSecuencias(const std::vector<int>& arr) {
    std::vector<std::pair<int, int>> secuencias; // Contendrá pares (valor, longitud de la secuencia)
    if (arr.empty()) return secuencias;

    int inicio = 0;
    for (int i = 1; i <= arr.size(); ++i) {
        if (i == arr.size() || arr[i] != arr[i - 1]) {
            secuencias.push_back({arr[inicio], i - inicio});
            inicio = i;
        }
    }

    return secuencias;
}

void intercambiarSecuencias(std::vector<int>& arr) {
    std::vector<std::pair<int, int>> secuencias = encontrarSecuencias(arr);
    if (secuencias.size() < 2) return;

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> dis(0, secuencias.size() - 1);

    int idx1 = dis(gen);
    int idx2 = dis(gen);
    while (idx2 == idx1) {
        idx2 = dis(gen);
    }

    int valor1 = secuencias[idx1].first;
    int valor2 = secuencias[idx2].first;
    int longitud1 = secuencias[idx1].second;
    int longitud2 = secuencias[idx2].second;

    int start1 = 0, start2 = 0;
    int currentIndex = 0;

    for (int i = 0; i < secuencias.size(); ++i) {
        if (i == idx1) {
            start1 = currentIndex;
        }
        if (i == idx2) {
            start2 = currentIndex;
        }
        currentIndex += secuencias[i].second;
    }
    for (int i = 0; i < longitud1; ++i) {
        arr[start1 + i] = valor2;
    }

    for (int i = 0; i < longitud2; ++i) {
        arr[start2 + i] = valor1;
    }
}

int pertubacion_aux2(std::vector<int>& v, int current_car, int init_i) {
    for(int i = init_i;  i < v.size(); i++) {
        if (v[i] != current_car) {
            return i;
        }
    }
    return -1;
}

void extend_route2(Solution& solucion, int extender[]) {
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

std::vector<std::pair<int, int>> identifySequences(const std::vector<int>& vec) {
    std::vector<std::pair<int, int>> sequences; // {valor, tamaño}
    int n = vec.size();

    if (n == 0) return sequences;

    int current = vec[0];
    int count = 1;

    for (int i = 1; i <= n; ++i) {
        if (i < n && vec[i] == current) {
            ++count;
        } else {
            sequences.emplace_back(current, count); // Añadir secuencia {valor, tamaño}
            if (i < n) {
                current = vec[i];
                count = 1;
            }
        }
    }

    return sequences;
}

// Función para generar combinaciones de extensiones/reducciones
void generateCombinations(const std::vector<std::pair<int, int>>& sequences, int totalSize,
                          std::vector<std::vector<int>>& results, std::vector<int>& temp, int idx) {
    // Si hemos procesado todas las secuencias
    if (idx == sequences.size()) {
        if (std::accumulate(temp.begin(), temp.end(), 0) == totalSize) {
            results.push_back(temp); // Guardar una combinación válida
        }
        return;
    }

    // Procesar la secuencia actual
    int originalSize = sequences[idx].second;
    int remainingSize = totalSize - std::accumulate(temp.begin(), temp.end(), 0);

    // Extender o reducir la secuencia actual dentro del rango permitido
    for (int newSize = 1; newSize <= remainingSize; ++newSize) {
        temp.push_back(newSize);
        generateCombinations(sequences, totalSize, results, temp, idx + 1);
        temp.pop_back(); // Deshacer para probar otra combinación
    }
}

// Función para reconstruir un vector desde las secuencias modificadas
std::vector<int> reconstructVector(const std::vector<std::pair<int, int>>& sequences, const std::vector<int>& sizes) {
    std::vector<int> result;
    for (size_t i = 0; i < sequences.size(); ++i) {
        result.insert(result.end(), sizes[i], sequences[i].first);
    }
    return result;
}

// Funcion que devuelve un vector con los pairs de los vehiculos y el indice donde empieza a usarse
std::vector<std::pair<int, std::pair<int, int>>> find_sequences_cars(const std::vector<int> &vehicles) {
    std::vector<std::pair<int, std::pair<int, int>>> sequence;
    int current_vehicle = vehicles[0];
    int start_index = 0;
    int end_index = 0;
    for (int i = 0; i < vehicles.size(); i++) {
        if (vehicles[i] != current_vehicle) {
            end_index = i-1;
            sequence.push_back({current_vehicle, {start_index, end_index}});
            current_vehicle = vehicles[i];
            start_index = i;
        }
    }
    sequence.push_back({current_vehicle, {start_index, vehicles.size()-1}});
    return sequence;
}

Solution intercambioSecuencias(Solution solucion, std::vector<Car>& cars) {
    auto sequences = identifySequences(solucion.vehicles);
    std::vector<std::vector<int>> combinations;
    std::vector<int> temp;
    generateCombinations(sequences, solucion.vehicles.size(), combinations, temp, 0);
    for (const auto& comb : combinations) {
        auto modifiedVec = reconstructVector(sequences, comb);
        int solucion_cost = Solution::calcularCosto(cars, modifiedVec, solucion.route);
        if (solucion_cost < solucion.totalCost) {
            solucion.totalCost = solucion_cost;
            solucion.route = modifiedVec;
        }
    }
    return solucion;
}

Solution local_search_cars(std::vector<Car>& cars, int dimension, Solution local_solution) {
    int iteraciones = 0;
    int max_iteraciones = 1000;
    auto route_0 = local_solution.route;

    while (iteraciones < max_iteraciones) {
        int mejorCosto = local_solution.totalCost;
        auto route_revertir = local_solution.route;
        auto cambiar_carros = intercambioSecuencias(local_solution, cars).vehicles;
        swap_random_sequence2(route_revertir);
        int menor_costo_1 = Solution::calcularCosto(cars, local_solution.vehicles, route_revertir);
        int menor_costo_2 = Solution::calcularCosto(cars, cambiar_carros, route_revertir);
        int menor_costo_3 = Solution::calcularCosto(cars, cambiar_carros, local_solution.route);
        if (menor_costo_1 < mejorCosto) {
            local_solution.route = route_revertir;
            mejorCosto = menor_costo_1;
        }
        if (menor_costo_2 < mejorCosto) {
            local_solution.vehicles = cambiar_carros;
            mejorCosto = menor_costo_2;
        }
        if (menor_costo_3 < mejorCosto) {
            local_solution.route = route_0;
            mejorCosto = menor_costo_3;
        }
        local_solution.totalCost = mejorCosto;
        route_0 = local_solution.route;

        iteraciones++;
    }
    return local_solution;
}

std::vector<std::pair<int, std::pair<int, int>>> FindSequences(std::vector<int> vehicles) {
    // std::cout << "FindSequences" << std::endl;
    std::vector<std::pair<int, std::pair<int, int>>> combinations;
    combinations.emplace_back(vehicles[0], std::make_pair(0, 0));
    for (int i = 1; i < vehicles.size(); i++) {
        if (vehicles[i] != vehicles[i-1]) {
            combinations.back().second.second = i-1;
            combinations.emplace_back(vehicles[i], std::make_pair(i, 0));
        }
    }
    combinations.back().second.second = vehicles.size()-1;
    return combinations;
}

std::pair<std::vector<int>, std::vector<int>> RemoveCar(std::vector<Car>& cars, std::vector<std::pair<int, std::pair<int, int>>> sequences, Solution solucion) {
    int mejorCosto = solucion.totalCost;
    std::vector<int> bestVehicles;
    auto bestRoute = solucion.route;
    auto reversedRoute = solucion.route;
    std::reverse(reversedRoute.begin(), reversedRoute.end());
    for (auto sequence : sequences) {
        if (sequence.second.first > 0) {
            auto vehiclesSolution = solucion.vehicles;
            int i = sequence.second.first-1;
            int carToRemove = vehiclesSolution[i];
            while (vehiclesSolution[i] == carToRemove) {
                vehiclesSolution[i] = sequence.first;
                i--;
            }
            int costo = Solution::calcularCosto(cars, vehiclesSolution, solucion.route);
            int reverseCosto = Solution::calcularCosto(cars, vehiclesSolution, reversedRoute);
            int newCosto = std::min(costo, reverseCosto);
            if (newCosto < mejorCosto) {
                if (costo < reverseCosto) {
                    bestRoute = solucion.route;
                } else {
                    bestRoute = reversedRoute;
                }
                // std::cout << "Costo mejorado: " << costo << std::endl;
                // Solution::print(bestRoute, vehiclesSolution);
                bestVehicles = vehiclesSolution;
                mejorCosto = newCosto;
            }
        }
        if (sequence.second.second < solucion.vehicles.size()-1) {
            auto vehiclesSolution = solucion.vehicles;
            int i = sequence.second.second+1;
            int carToRemove = vehiclesSolution[i];
            while (vehiclesSolution[i] == carToRemove) {
                vehiclesSolution[i] = sequence.first;
                i++;
            }
            int costo = Solution::calcularCosto(cars, vehiclesSolution, solucion.route);
            int reverseCosto = Solution::calcularCosto(cars, vehiclesSolution, reversedRoute);
            int newCosto = std::min(costo, reverseCosto);
            if (newCosto < mejorCosto) {
                if (costo < reverseCosto) {
                    bestRoute = solucion.route;
                } else {
                    bestRoute = reversedRoute;
                }
                // std::cout << "Costo mejorado: " << costo << std::endl;
                // Solution::print(bestRoute, vehiclesSolution);
                bestVehicles = vehiclesSolution;
                mejorCosto = newCosto;
            }
        }
    }
    if (!bestVehicles.empty()) {
        return std::make_pair(bestVehicles, bestRoute);
    }
    return std::make_pair(solucion.vehicles, solucion.route);
}

std::pair<std::vector<int>, std::vector<int>> InsertNewCar(std::vector<Car>& cars, int dimension, Solution solucion, int startPos, int endPos, int totalCost) {
    // std::cout << "Empieza funcion InsertNewCar.\nVehiculo y rutas usados:\n";
    std::vector<int> vehicles(solucion.vehicles.begin() + startPos, solucion.vehicles.begin() + endPos);
    std::vector<int> route(solucion.route.begin() + startPos, solucion.route.begin() + endPos);
    std::vector<int> reversedRoute = route;
    std::reverse(reversedRoute.begin(), reversedRoute.end());
    std::vector<int> routeAux = route;
    int cost = totalCost;
    int startIndex = -1;
    int endIndex = -1;
    int newCar = -1;
    for (auto& car : cars) {
        if (!car.getUsed()) {
            // std::cout << "Auto no usado: " << car.getUsed() << std::endl;
            auto vehiclesAux = vehicles;
            for (int i = 0; i < dimension-1; i++) {
                int initVehicle = vehiclesAux[i];
                vehiclesAux[i] = car.getId();
                for (int j = i+1; j < dimension; j++) {
                    vehiclesAux[j] = car.getId();
                    int newCostRegular = Solution::calcularCosto(cars, vehiclesAux, route);
                    int newCostReverse = Solution::calcularCosto(cars, vehiclesAux, reversedRoute);
                    int newCost = std::min(newCostRegular, newCostReverse);
                    // std::cout << "Costo de incluir vehiculo: " << newCost << std::endl;
                    // Solution::print(vehiclesAux, routeAux);
                    if (newCost < cost) {
                        if (newCostRegular < newCostReverse) {
                            routeAux = route;
                        } else {
                            routeAux = reversedRoute;
                        }
                        cost = newCost;
                        startIndex = i;
                        endIndex = j;
                        newCar = car.getId();
                    }
                }
                vehiclesAux[i] = initVehicle;
            }
        }
    }
    if (newCar != -1) {
        auto vehiclesAux = solucion.vehicles;
        for (int i = startPos+startIndex; i <= startPos+endIndex; i++) {
            vehiclesAux[i] = newCar;
        }
        for (int i = startPos, j = 0; i < endPos; i++, j++) {
            route[i] = routeAux[j];
        }
        return std::make_pair(vehiclesAux, route);
    }
    // std::cout << "Buscando eliminar vehiculo.\n";
    auto sequences = FindSequences(solucion.vehicles);
    auto removed_car = RemoveCar(cars, sequences, solucion);
    return removed_car;
}

std::pair<std::vector<int>, std::vector<int>> ExtendRoute(std::vector<Car>& cars, int dimension, Solution solucion, int startPos, int endPos, int totalCost) {
    std::vector<int> vehicles(solucion.vehicles.begin() + startPos, solucion.vehicles.begin() + endPos);
    std::vector<int> route(solucion.route.begin() + startPos, solucion.route.begin() + endPos);
    std::vector<int> reversedRoute = route;
    std::reverse(reversedRoute.begin(), reversedRoute.end());
    auto bestRoute = route;
    int bestCost = totalCost;
    /*std::cout << "ExtendRoute" << std::endl;
    for (int i = 0; i < route.size(); i++) {
        std::cout << route[i] << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < vehicles.size(); i++) {
        std::cout << vehicles[i] << " ";
    }
    std::cout << std::endl;*/
    std::vector <int> bestVehicles;
    std::vector<std::vector<int>> combinations;
    std::vector<int> temp;
    auto carSequences = identifySequences(vehicles);
    generateCombinations(carSequences, solucion.vehicles.size(), combinations, temp, 0);
    /*std::cout << "Vehicles usados para extender la ruta: " << std::endl;
    for (const auto v : vehicles) {
        std::cout << v << " ";
    }
    std::cout << std::endl;*/
    for (const auto& comb : combinations) {
        auto modifiedVec = reconstructVector(carSequences, comb);
        int solucion_cost = Solution::calcularCosto(cars, modifiedVec, route);
        int solucionCostReversed = Solution::calcularCosto(cars, modifiedVec, reversedRoute);
        int cost = std::min(solucion_cost, solucionCostReversed);
        if (cost < bestCost) {
            if (solucion_cost < solucionCostReversed) {
                bestRoute = route;
            } else {
                bestRoute = reversedRoute;
            }
            bestCost = cost;
            bestVehicles = modifiedVec;
        }
    }
    // std::cout << "Fin de busqueda a mejor" << std::endl;
    std::vector<int> vehiclesAux = solucion.vehicles;
    auto routeAux = solucion.route;
    if (!bestVehicles.empty()) {
        for (int i = startPos, j = 0; i < endPos; i++, j++) {
            vehiclesAux[i] = bestVehicles[j];
            routeAux[i] = bestRoute[j];
        }
        return std::make_pair(vehiclesAux, routeAux);
    }
    return std::make_pair(vehiclesAux, routeAux);
}

void ReverseRoute(std::vector<int> route) {
    return std::reverse(route.begin(), route.end());
}

std::pair<int, std::pair<int, int>> GetLowCost(std::vector<Car>& cars, std::vector<std::vector<int>> vehicles, std::vector<std::vector<int>> routes) {
    std::map<int, std::pair<int, int>> costos;
    for (int i = 0; i < vehicles.size(); i++) {
        for (int j = 0; j < routes.size(); j++) {
            int costo = Solution::calcularCosto(cars, vehicles[i], routes[j]);
            costos[costo] = std::make_pair(i, j);
        }
    }
    for (auto it: costos) {
        std::cout << it.first << " ";
    }
    std::cout << "\n";
    return {costos.begin()->first, {costos.begin()->second.first, costos.begin()->second.second}};
}

Solution LocalSearchCars(std::vector<Car>& cars, int dimension, Solution solucion_inicial) {
    // std::cout << "Empieza el local search.\n";
    std::vector<std::vector<int>> vehicles;
    std::vector<std::vector<int>> routes;

    int size = solucion_inicial.route.size();
    int fraccion = 4;
    int rango = (size/fraccion)*3;
    int startPos = rand() % (size - rango + 1);
    int endPos = startPos + rango;
    if (size < 22) {
        startPos = 0;
        endPos = size;
        rango = size;
    }

    auto bestCost = solucion_inicial.totalCost;
    auto currentRoute = solucion_inicial.route;
    auto currentVehicles = solucion_inicial.vehicles;
    auto reverseRoute = solucion_inicial.route;
    std::reverse(reverseRoute.begin(), reverseRoute.end());

    vehicles.push_back(currentVehicles);
    routes.push_back(currentRoute);

    auto newCar = InsertNewCar(cars, rango, solucion_inicial, startPos, endPos, bestCost);
    int newCarCost = Solution::calcularCosto(cars, newCar.first, newCar.second);
    auto extendVehicleRoute = ExtendRoute(cars, dimension, solucion_inicial, startPos, endPos, bestCost);
    int extendVehicleRouteCost = Solution::calcularCosto(cars, extendVehicleRoute.first, extendVehicleRoute.second);
    /*std::cout << "Mejor costo de add/remove auto: " << newCarCost << std::endl;
    Solution::print(newCar.second, newCar.first);
    std::cout << "Mejor costo de extender ruta: " << extendVehicleRouteCost << std::endl;
    Solution::print(extendVehicleRoute.second, extendVehicleRoute.first);*/
    if (bestCost > extendVehicleRouteCost || bestCost > newCarCost) {
        if (extendVehicleRouteCost < newCarCost) {
            solucion_inicial.vehicles = extendVehicleRoute.first;
            solucion_inicial.route = extendVehicleRoute.second;
            solucion_inicial.totalCost = extendVehicleRouteCost;
        } else {
            solucion_inicial.vehicles = newCar.first;
            solucion_inicial.route = newCar.second;
            solucion_inicial.totalCost = newCarCost;
        }
    }
    /*vehicles.push_back(newCar);
    vehicles.push_back(extendVehicleRoute);
    std::cout << "Ruta extendida:\n";
    Solution::print(currentRoute, extendVehicleRoute);
    std::cout << Solution::calcularCosto(cars, extendVehicleRoute, solucion_inicial.route) << std::endl;
    routes.push_back(reverseRoute);
    auto bestSolution = GetLowCost(cars, vehicles, routes);
    solucion_inicial.vehicles = vehicles[bestSolution.second.first];
    solucion_inicial.route = routes[bestSolution.second.second];
    solucion_inicial.totalCost = bestSolution.first;*/
    return solucion_inicial;
}