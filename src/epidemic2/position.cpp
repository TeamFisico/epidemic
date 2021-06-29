#include "position.hpp"
#include <cassert>
#include <cmath>
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
    if ((x - other.x) * (x - other.x) <= r * r && (y - other.y) * (y - other.y) <= r * r) { return true; }
    return false;
}

void Position::move_toward(Position target, double speed)
{
    double angle = (x - target.x) / (y - target.y);
    double delta_angle = rand_gauss(angle / 10, angle / 100); // random variation to the angle that follow the standard deviation
    double v_x = speed * std::cos(angle + delta_angle); // x component of velocity vector
    double v_y = speed * std::sin(angle + delta_angle); // y component of velocity vector
    Position new_pos {target.x + v_x,target.y + v_y};
    x = new_pos.x;
    y = new_pos.y;
}

double Position::distance_to(const Position &a)
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
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution X(blh_corner.get_x(), trh_corner.get_x());
    std::uniform_real_distribution Y(blh_corner.get_y(), blh_corner.get_y());
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

int sim::rounded_norm(double mean, double deviation)
{
    double num = rand_gauss(mean, deviation);
    int trunc_num = static_cast<double>(num);
    if (trunc_num < 0)
    { // make sure that it never return a negative number, as negative number will give problem where this function is
      // used
        return 0;
    }
    if (num - trunc_num <= 0.5) { return trunc_num; }
    else
    {
        return trunc_num + 1;
    }
}

bool sim::try_event(double probability)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    assert(probability >= 0.0 && probability <= 1.0);
    std::uniform_real_distribution<double> rnum(0.0, 1.0);
    return rnum(gen) <= probability;
}
