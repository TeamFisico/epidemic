#include "position.hpp"
#include "random.hpp"
#include <cassert>
#include <cmath>

namespace smooth_sim
{
// Constructor
Position::Position(double x, double y) : x{x}, y{y}
{
}
// Default Constructor
Position::Position() : x{0}, y{0}
{
}

bool Position::InRadius(Position other, double r) const
{
    if (((x - other.x) * (x - other.x)) + ((y - other.y) * (y - other.y)) <= r * r) { return true; }
    return false;
}

void Position::move_toward(Position target, double speed, Random& rng)
{
    double dx = target.x - x;
    double dy = target.y - y;
    double angle = std::atan2(dy, dx);
    double delta_angle = rng.uniform(-0.0872665, 0.0872665); // random variation from -5° to 5°
    while (distance_to(target) < speed)
    {
        speed = rng.uniform(0, 2);
    }
    double v_x = speed * std::cos(angle + delta_angle); // x component of velocity vector
    double v_y = speed * std::sin(angle + delta_angle); // y component of velocity vector
    x += v_x;
    y += v_y;
}

double Position::distance_to(Position& a) const
{
    return std::sqrt((x - a.x) * (x - a.x) + (y - a.y) * (y - a.y));
}

double Position::get_x() const
{
    return x;
}

double Position::get_y() const
{
    return y;
}

Position rand_pos(Position blh_corner, Position trh_corner)
{
    Random rng;
    Position result{rng.uniform(blh_corner.get_x(), trh_corner.get_x()),
                    rng.uniform(blh_corner.get_y(), trh_corner.get_y())};
    return result;
}

} //namespace smooth_sim