#include "location.hpp"
#include <cmath>
#include "random.hpp"

#define PI 3.14159265358979323846 /* pi */

using namespace sim;
// Constructor
Location::Location(Position location_pos, double location_Radius)
    : location_pos{location_pos}, location_Radius{location_Radius}
{
}
// Default Constructor WIP
/*Location::Location()
{
}*/

Location sim::generate_close_loc(Position &pos, double min_distance, double max_distance)
{
    Random rng;
    double rangle = rng.uniform(0, 2 * PI);
    double rdistance = rng.uniform(min_distance, max_distance);
    double v_x = rdistance * std::cos(rangle); // x coordinate of the traslation vector
    double v_y = rdistance * std::sin(rangle); // y coordinate of the traslation vector
    double x = pos.get_x() + v_x;
    double y = pos.get_y() + v_y;
    Position res{x, y};
    double radius =
        rng.gauss(max_distance / 4, max_distance / 10); // arbitrary way to generate Locatio radius, still WIP
    return {res, radius};
}

Location sim::rand_loc(Position blh_corner, Position trh_corner, double radius)
{
    return {sim::rand_pos(blh_corner, trh_corner),radius};
}