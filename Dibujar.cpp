//
// Created by thefo on 4/3/2025.
//

#include "Dibujar.h"
#include <SFML/Graphics.hpp>
#include <random>
#include <fstream>
#include <iostream>

sf::Color getColor(int vehiculo) {
    static std::map<int, sf::Color> colores;
    static std::vector<sf::Color> paleta = {
        sf::Color::Red, sf::Color::Green, sf::Color::Blue,
        sf::Color::Yellow, sf::Color::Magenta, sf::Color::Cyan
    };

    if (colores.find(vehiculo) == colores.end()) {
        colores[vehiculo] = paleta[colores.size() % paleta.size()];
    }
    return colores[vehiculo];
}

bool esPosicionValida(float x, float y, const std::vector<Ciudad>& ciudades, float distancia_min) {
    for (const auto& ciudad : ciudades) {
        float dx = ciudad.x - x;
        float dy = ciudad.y - y;
        if (std::sqrt(dx * dx + dy * dy) < distancia_min) {
            return false;  // La ciudad está demasiado cerca de otra
        }
    }
    return true;
}

void dibujar(Solution &solucion) {
    sf::RenderWindow window(sf::VideoMode(1800, 900), "Rutas de Vehículos");
    std::srand(std::time(nullptr));

    // Fuente para mostrar el texto en pantalla
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error: No se pudo cargar la fuente.\n";
        return;
    }

    int num_ciudades = solucion.route.size();
    // Distancia minima entre ciudades
    float distancia_min = 90.0f;

    // Generar ciudades en el orden de recorrido con su id y su posicion y guardarlas en el vector ciudades
    std::vector<Ciudad> ciudades;
    for (int i = 0; i < num_ciudades; i++) {
        float x, y;
        // Se vuelve a generar la posicion si no es válida(dentro de la ventana y con la distancia minima entre estas)
        do {
            x = 50 + std::rand() % 1700;
            y = 50 + std::rand() % 800;
        } while (!esPosicionValida(x, y, ciudades, distancia_min));

        ciudades.push_back({solucion.route[i], x, y});
        std::cout << solucion.route[i] << " ";
    }
    std::cout << std::endl;

    // Copia de los vehiculos usados en la ruta
    std::vector<int> carros = solucion.vehicles;

    // Mapa de recorridos por vehículo
    std::map<int, std::vector<Ciudad>> rutas;
    int current_car = carros[0];
    std::map<int, sf::Color> vehiculoColores; // Almacenar los colores usados en la leyenda
    for (size_t i = 0; i < ciudades.size(); ++i) {
        vehiculoColores[carros[i]] = getColor(carros[i]);
        if (carros[i] != current_car) {
            rutas[current_car].push_back(ciudades[i]);
            current_car = carros[i];
        }
        rutas[carros[i]].push_back(ciudades[i]);
    }
    rutas[current_car].push_back(ciudades[0]);


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);

        // Dibujar rutas de cada vehículo
        for (const auto& ruta : rutas) {
            sf::Color color = getColor(ruta.first);
            const std::vector<Ciudad>& recorrido = ruta.second;

            for (size_t i = 0; i < recorrido.size() - 1; ++i) {
                sf::Vertex line[] = {
                    sf::Vertex(sf::Vector2f(recorrido[i].x + 8, recorrido[i].y + 10), color),
                    sf::Vertex(sf::Vector2f(recorrido[i + 1].x + 8, recorrido[i + 1].y + 10), color)
                };
                window.draw(line, 2, sf::Lines);
            }
        }

        // Dibujar ciudades como puntos
        for (const auto& ciudad : ciudades) {
            // Dibujar círculo
            sf::CircleShape circle(15);
            circle.setFillColor(sf::Color::Black);
            circle.setPosition(ciudad.x - 5, ciudad.y - 5); // Ajustar para centrar

            // Dibujar texto con el ID de la ciudad
            sf::Text text;
            text.setFont(font);
            text.setString(std::to_string(ciudad.id));
            text.setCharacterSize(15);
            text.setFillColor(sf::Color::White);
            text.setPosition(ciudad.x + 6, ciudad.y + 1); // Ajustar la posición

            window.draw(circle);
            window.draw(text);
        }

        // Dibujar la leyenda en la esquina superior izquierda
        int offsetY = 10; // Espaciado entre líneas en la leyenda
        int startX = 20, startY = 20; // Posición inicial de la leyenda

        for (const auto& vehiculo : vehiculoColores) {
            int vehiculoID = vehiculo.first;
            sf::Color color = vehiculo.second;

            // Dibujar un rectángulo de color
            sf::RectangleShape colorBox(sf::Vector2f(20, 20));
            colorBox.setFillColor(color);
            colorBox.setPosition(startX, startY + offsetY);

            // Dibujar texto con el ID del vehículo
            sf::Text legendText;
            legendText.setFont(font);
            legendText.setString("Vehiculo " + std::to_string(vehiculoID));
            legendText.setCharacterSize(14);
            legendText.setFillColor(sf::Color::Black);
            legendText.setPosition(startX + 30, startY + offsetY);

            window.draw(colorBox);
            window.draw(legendText);

            offsetY += 30; // Espaciado entre cada vehículo en la leyenda
        }

        sf::Text legendText;
        legendText.setFont(font);
        legendText.setString("Ruta inicial: " + std::to_string(solucion.route[0]) + "-" + std::to_string(solucion.route[1]));
        legendText.setCharacterSize(14);
        legendText.setFillColor(sf::Color::Black);
        legendText.setPosition(startX, startY + offsetY);
        window.draw(legendText);
        sf::Text legendTextcosto;
        legendTextcosto.setFont(font);
        legendTextcosto.setString("Costo del tour: " + std::to_string(solucion.totalCost));
        legendTextcosto.setCharacterSize(14);
        legendTextcosto.setFillColor(sf::Color::Black);
        legendTextcosto.setPosition(startX, startY + offsetY + 30);
        window.draw(legendTextcosto);

        window.display();
    }

}