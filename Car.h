//
// Created by thefo on 1/24/2025.
//

#ifndef CAR_H
#define CAR_H

#include <vector>

class Car {
    int id;
    std::vector<std::vector<int>> travelMatrix;
    std::vector<std::vector<int>> returnCostMatrix;
    bool used;
public:
    Car(int id_, std::vector<std::vector<int>> travelMatrix_);
    int getId() const;
    bool getUsed() const;
    void setUsed(bool _used);
    std::vector<std::vector<int>> getTravelMatrix();
    void setReturnCostMatrix(std::vector<std::vector<int>> returnCostMatrix_);
    std::vector<std::vector<int>> getReturnCostMatrix();
    void printTravelMatrix() const;
    void printReturnCostMatrix() const;
};



#endif //CAR_H
