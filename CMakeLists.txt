cmake_minimum_required(VERSION 3.30)
project(CaRS_ILS)

set(CMAKE_CXX_STANDARD 20)

add_executable(CaRS_ILS main.cpp
        Car.cpp
        Car.h
        Solution.cpp
        Solution.h
        readCars.cpp
        readCars.h
        LocalSearch.cpp
        LocalSearch.h
        IteratedLocalSearch.cpp
        IteratedLocalSearch.h
        Dibujar.cpp
        Dibujar.h)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML/lib/cmake/SFML)
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)

include_directories(C:/SFML/include)
target_link_libraries(CaRS_ILS sfml-system sfml-window sfml-graphics sfml-audio)
