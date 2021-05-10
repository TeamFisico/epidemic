#include <cmath>
#include <position.hpp>
#include <random>

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
    if ((x - other.x) * (x - other.x) <= r * r && (y - other.y) * (y - other.y) <= r * r)
    {
        return true;
    }
    return false;
}

void Position::move_toward(Position target, double speed)
{
    double angle = (x - target.x) / (y - target.y);
    double delta_angle =
        rand_gauss(angle / 10, angle / 100); // random variation to the angle that follow the standard deviation
    double v_x = speed * std::cos(angle + delta_angle); // x component of velocity vector
    double v_y = speed * std::sin(angle + delta_angle); // y component of velocity vector
    x = target.x + v_x;
    y = target.y + v_y;
}

Position sim::rand_pos(double x_min, double x_max, double y_min, double y_max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution X(x_min, x_max);
    std::uniform_real_distribution Y(y_min, y_max);
    Position result{X(gen), Y(gen)};
    return result;
}

double sim::rand_gauss(double mean, double deviation)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> rand(mean, deviation);
    return rand(gen);
}
