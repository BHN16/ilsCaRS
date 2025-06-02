//
// Created by thefo on 1/24/2025.
//

#include "readCars.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <cstdlib>
#include <algorithm>
#include "Solution.h"
#include "Car.h"

std::vector<std::vector<int>> readMatrix(std::ifstream &file, int dimension) {
    std::vector<std::vector<int>> matrix(dimension, std::vector<int>(dimension));
    for (int i = 0; i < dimension; ++i) {
        for (int j = 0; j < dimension; ++j) {
            file >> matrix[i][j];
        }
    }
    return matrix;
}
std::pair<int, int> findNextCity(std::vector<Car> &cars, int currentCity, int currentVehicle, std::vector<bool> &visited, int cityToReturn) {
    int minCost = std::numeric_limits<int>::max();
    int nextCity = -1;
    int bestVehicle = currentVehicle;

    for (int v = 0; v < cars.size(); ++v) {
        for (int city = 0; city < visited.size(); ++city) {
            if (!visited[city] && cars[v].getTravelMatrix()[currentCity][city] < minCost && !cars[v].getUsed()) {
                int totalCost = cars[v].getTravelMatrix()[currentCity][city];
                if (currentVehicle != v) {
                    int costToChange = cars[currentVehicle].getReturnCostMatrix()[currentCity][cityToReturn];
                    totalCost += costToChange;
                }
                if (totalCost < minCost) {
                    minCost = totalCost;
                    nextCity = city;
                    bestVehicle = v;
                }
            }
        }
    }

    return {nextCity, bestVehicle};
}

void read_graphs(std::vector<Car> &cars, int& dimension, std::string instancia) {
    std::ifstream file(instancia);
    if (!file.is_open()) {
        std::cout << "Error al abrir el archivo." << std::endl;
        return;
    }

    std::string line;
    int carsNumber = 0;
    while (getline(file, line)) {
        if (line.find("DIMENSION") != std::string::npos) {
            std::istringstream ss(line);
            std::string temp;
            ss >> temp >> temp >> dimension;
        } else if (line.find("CARS_NUMBER") != std::string::npos) {
            std::istringstream ss(line);
            std::string temp;
            ss >> temp >> temp >> carsNumber;
        } else if (line.find("EDGE_WEIGHT_SECTION") != std::string::npos) {
            break;
        }
    }
    for (int i = 0; i < carsNumber; ++i) {
        int matrixId;
        file >> matrixId;
        cars.emplace_back(i, readMatrix(file, dimension));
    }

    while (getline(file, line)) {
        if (line.find("RETURN_RATE_SECTION") != std::string::npos) {
            break;
        }
    }

    for (int i = 0; i < carsNumber; ++i) {
        int matrixId;
        file >> matrixId;
        cars[i].setReturnCostMatrix(readMatrix(file, dimension));
    }

    file.close();
}

std::pair<int, std::pair<int, int>> calcular_ruta_inicial(std::vector<Car> &cars, int car, int* ciudades) {
    int costo1 = cars[car].getTravelMatrix()[ciudades[0]][ciudades[1]];
    if (int costo2 = cars[car].getTravelMatrix()[ciudades[1]][ciudades[0]]; costo1 < costo2) {
        return {car, {ciudades[0], ciudades[1]}};
    } else {
        return {car, {ciudades[1], ciudades[0]}};
    }
}

std::pair<int, std::pair<int, int>> costoVehiculosRuta(std::vector<Car> &cars, int car, int vehiculos) {
    int min_costo = std::numeric_limits<int>::max();
    std::pair<int, std::pair<int, int>> ciudades_optimo;
    for (int ciudad1 = 0; ciudad1 < vehiculos; ++ciudad1) {
        for (int ciudad2 = 1; ciudad2 < vehiculos; ++ciudad2) {
            int ciudades[2] = {ciudad1, ciudad2};
            if (auto min_costo_ruta_inicial = calcular_ruta_inicial(cars, car, ciudades); min_costo_ruta_inicial.first < min_costo) {
                min_costo = min_costo_ruta_inicial.first;
                ciudades_optimo = min_costo_ruta_inicial;
            }
        }
    }
    return ciudades_optimo;
}

std::pair<int, std::pair<int, int>> ruta_inicial(std::vector<Car> &cars, int vehiculos) {
    int min_costo = std::numeric_limits<int>::max();
    std::pair<int, std::pair<int, int>> ruta_optimo;
    for (int i = 0; i < cars.size(); ++i) {
        if (std::pair<int, std::pair<int, int>> min_costo_vehiculos_ruta = costoVehiculosRuta(cars, i, vehiculos); min_costo_vehiculos_ruta.first < min_costo) {
            min_costo = min_costo_vehiculos_ruta.first;
            ruta_optimo = min_costo_vehiculos_ruta;
        }
    }
    return ruta_optimo;
}

void initial_solution(std::vector<Car> &cars, int &dimension, Solution& solucion, std::mt19937& g) {
    std::vector<bool> visited(dimension, false);
    std::uniform_int_distribution<int> distrib(0, cars.size() - 1);
    std::uniform_int_distribution<int> distrib_city(0, dimension-1);
    // Vehiculo con el que se empieza
    /*int currentVehicle = distrib(g);
    // Ciudad donde empieza la ruta del currentVehicle
    int startCity = distrib_city(g);
    // Ciudad donde se debe retornar para completar el ciclo
    int initialCity = startCity;
    // Segunda ciudad a visitar, el currentVehicle debe visitar almenos 2 ciudades
    int currentCity = distrib_city(g);
    do {
      currentCity = distrib_city(g);
    } while (currentCity == startCity);*/

    auto s0 = ruta_inicial(cars, dimension);
    int currentVehicle = s0.first;
    int startCity = s0.second.first;
    int initialCity = startCity;
    int currentCity = s0.second.second;
    
    // Primera y segunda ciudad marcadas como visitadas
    visited[startCity] = true;
    visited[currentCity] = true;
    // Se anhaden las primeras ciudades visitadas en la ruta
    solucion.route.push_back(startCity);
    solucion.route.push_back(currentCity);
    // Se anhade el primer vehiculo usado
    solucion.vehicles.push_back(currentVehicle);
    // Se suma al total cost el viaje entre la primera y segunda ciudad con el currentVehicle
    solucion.totalCost += cars[currentVehicle].getTravelMatrix()[startCity][currentCity];
    // Mientras todas las ciudades no sean visitadas
    while(!std::all_of(visited.begin(), visited.end(), [](bool val) { return val; })) {
        // Se obtiene la mejor ciudad a visitar y el mejor vehiculo para llegar
        std::pair<int, int> fnc = findNextCity(cars, currentCity, currentVehicle, visited, startCity);
        int nextCity = fnc.first;
        int bestVehicle = fnc.second;
        // Se suma el costo de ir con el mejor vehiculo desde la currentCity a la nextCity
        solucion.totalCost += cars[bestVehicle].getTravelMatrix()[currentCity][nextCity];
        // Si se cambia de vehiculo
        if (bestVehicle != currentVehicle) {
            // Sumar el costo de retorno del currentvehicle desde el currentcity hasta la startcity
            solucion.totalCost += cars[currentVehicle].getReturnCostMatrix()[currentCity][startCity];
            // Se cambia el startCity a la ciudad donde se alquila el bestVehicle
            startCity = currentCity;
            // Se cambia el estado del auto a usado, ya que no se puede usar el auto en mas de 1 ruta.
            cars[currentVehicle].setUsed(true);
            // Se cambia el currentVehicle al bestVehicle
            currentVehicle = bestVehicle;
        }
        // Se anhade la ciudad y el vehiculo a la ruta.
        solucion.route.push_back(nextCity);
        solucion.vehicles.push_back(currentVehicle);
        // El nextCity pasa estar visitado y se cambia la currentCity al nextCity
        visited[nextCity] = true;
        currentCity = nextCity;
    }
    // Se anhade el costo de viaje del currentVehicle a la initialCity para completar el ciclo hamiltoniano
    solucion.totalCost += cars[currentVehicle].getTravelMatrix()[currentCity][initialCity];
    // Se anhade el costo de retorno del currentVehicle a su ciudad de origen
    solucion.totalCost += cars[currentVehicle].getReturnCostMatrix()[initialCity][startCity];
    // Marcar como usado el auto con el que se retorna
    cars[currentVehicle].setUsed(true);
    solucion.vehicles.push_back(currentVehicle);
}