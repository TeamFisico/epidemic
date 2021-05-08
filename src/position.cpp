#include <position.hpp>
#include <cmath>
#include <random>

using namespace sim;
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
Position sim::rand_pos(double x_limit, double y_limit){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution X(0.0, x_limit);
    std::uniform_real_distribution Y(0.0, y_limit);
    Position result{X(gen),Y(gen)};
    return result;
}
