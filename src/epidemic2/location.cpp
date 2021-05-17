#include "location.hpp"
#include <cmath>
#include <numeric>
#include <random>

#define PI 3.14159265358979323846 /* pi */

using namespace sim;
// Constructor
Location::Location(Position &location_pos, double location_Radius)
    : location_pos{location_pos}, location_Radius{location_Radius}
{
}
// Default Constructor WIP
/*Location::Location()
{
}*/

Position Location::get_pos()
{
    return location_pos;
}

double Location::get_radius()
{
    return location_Radius;
}

Location sim::generate_close_loc(Position &pos, double max_distance)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> angle(0, 2 * PI);
    std::uniform_real_distribution<> distance(0, max_distance);
    double rangle = angle(gen);
    double rdistance = distance(gen);
    double v_x = rdistance * std::cos(rangle); // x coordinate of the traslation vector
    double v_y = rdistance * std::sin(rangle); // y coordinate of the traslation vector
    double x = pos.get_x() + v_x;
    double y = pos.get_y() + v_y;
    Position res{x, y};
    double radius =
        sim::rand_gauss(max_distance / 4, max_distance / 10); // arbitrary way to generate Locatio radius, still WIP
    return {res, radius};
}
