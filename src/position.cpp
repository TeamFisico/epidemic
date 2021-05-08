#include "../include/position.hpp"
#include <cmath>
//Constructor
Position::Position(double x, double y)
: x{x}, y{y}
{}
//Default Constructor
Position::Position()
: x{0}, y{0}
{}

bool Position::InRadius(Position &other, double r) {
    if (std::abs(x - other.x) <= r && std::abs(y - other.y) <= r){
        return true;
    }
    return false;
}
