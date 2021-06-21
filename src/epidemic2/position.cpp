#include "position.hpp"
#include <cassert>
#include <cmath>
#include "random.hpp"

using namespace sim;
// Constructor
Position::Position(double x, double y) : x{x}, y{y}
{
}
// Default Constructor
Position::Position() : x{0}, y{0}
{
}

bool Position::InRadius(Position other, double r)
{
    if ((x - other.x) * (x - other.x) <= r * r && (y - other.y) * (y - other.y) <= r * r) { return true; }
    return false;
}

void Position::move_toward(Position target, double speed)
{
    Random rng;
    double angle = (x - target.x) / (y - target.y);
    double delta_angle =
        rng.gauss(angle / 10, angle / 100); // random variation to the angle that follow the standard deviation
    double v_x = speed * std::cos(angle + delta_angle); // x component of velocity vector
    double v_y = speed * std::sin(angle + delta_angle); // y component of velocity vector
    x = target.x + v_x;
    y = target.y + v_y;
}

double Position::distance_to(Position &a)
{
    return std::sqrt((x - a.x) * (x - a.x) + (y - a.y) * (y - a.y));
}

double Position::get_x()
{
    return x;
}

double Position::get_y()
{
    return y;
}

Position sim::rand_pos(Position blh_corner, Position trh_corner)
{
    Random rng;
    Position result{rng.uniform(blh_corner.get_x(),trh_corner.get_x()), rng.uniform(blh_corner.get_y(),trh_corner.get_y())};
    return result;
}

