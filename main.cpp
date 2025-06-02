#include <utility>
#include <chrono>
#include <vector>
#include <fstream>
#include <random>
#include "LocalSearch.h"
#include "readCars.h"
#include "Solution.h"
#include "Car.h"
#include "IteratedLocalSearch.h"
#include "Dibujar.h"

// taskkill -im CaRS_ILS.exe -f
// Experimentos hasta 25 instancias
// Interfaz grafica

// Llegar a 167
int main() {
    std::vector<std::pair<std::string, int>> leer = {
        {"Instancias-CaRS/CaRS_NaoEuclidianas/BrasilRJ14n.car", 5},
    };
    auto seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::mt19937 g(seed);
    for(auto&[archivo, experimentos] : leer) {
        std::cout << "Archivo a leer: " << archivo << std::endl;
        auto inicio = std::chrono::high_resolution_clock::now();


        for (int i = 0; i < experimentos; i++) {

            // 1. Se lee el archivo y se obtienen los cars

            std::vector<Car> cars;
            int dimension = 0;
            read_graphs(cars, dimension, archivo);

            // 2. s_0: solucion inicial aleatoria

            Solution solucion_inicial;
            initial_solution(cars, dimension, solucion_inicial, g);
            solucion_inicial.calcularCosto(cars);

            // 3. s*: óptimo local basado en la solucion inicial

            // Solution mejor_tour = local_search_cars(cars, dimension, solucion_inicial);
            Solution mejor_tour = LocalSearchCars(cars, dimension, solucion_inicial);
            mejor_tour.calcularCosto(cars);

            // 4. Condición para terminar: n iteraciones
            int iteraciones = 0;
            int iteraciones_sin_mejora = 0;
            while (iteraciones < 1000) {
                // 5. s': Perturbacion a s*
                // auto nuevo_tour = perturbacion_cars(mejor_tour, cars, g, iteraciones_sin_mejora);
                auto nuevo_tour = PerturbacionCars(mejor_tour, g);
                /*std::cout << "Solucion perturbada: " << nuevo_tour.calcularCosto(cars) << std::endl;
                nuevo_tour.print();*/

                // 6. s*': local search sobre s'
                // auto tour_intermedio = local_search_cars(cars, dimension, nuevo_tour);
                auto tour_intermedio = LocalSearchCars(cars, dimension, nuevo_tour);
                /*std::cout << "Costo nuevo: " << tour_intermedio.totalCost << std::endl;
                tour_intermedio.print();*/

                // 7. s*: obtener el optimo local
                if (tour_intermedio.totalCost < mejor_tour.totalCost) {
                    mejor_tour = tour_intermedio;
                    iteraciones_sin_mejora = 0;
                }
                iteraciones++;
                iteraciones_sin_mejora++;
            }
            auto fin = std::chrono::high_resolution_clock::now();
            auto duracion = std::chrono::duration_cast<std::chrono::milliseconds>(fin - inicio);
            std::cout << "Experimento " << i << ": \n";
            std::cout << "Tiempo de ejecución: " << duracion.count() << " ms" << std::endl;
            std::cout << "Costo total: " << mejor_tour.totalCost << std::endl;
        }

    }
    return 0;
}