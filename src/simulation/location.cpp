#include "location.hpp"
#include "parameters.hpp"
#include "random.hpp"
#include <cmath>

namespace smooth_sim
{
// Constructor
Location::Location(Position location_pos, double location_Radius, int cluster_index)
    : location_pos{location_pos},
      location_Radius{location_Radius},
      cluster_index{cluster_index}
{
}
// Default Constructor WIP
/*Location::Location()
{
}*/

Location generate_close_loc(Position &pos, double min_distance, double max_distance, int cluster_index,Random& group_engine)
{
    double rangle = group_engine.uniform(0, 2 * PI);
    double rdistance = group_engine.uniform(min_distance, max_distance);
    double v_x = rdistance * std::cos(rangle); // x coordinate of the traslation vector
    double v_y = rdistance * std::sin(rangle); // y coordinate of the traslation vector
    double x = pos.get_x() + v_x;
    double y = pos.get_y() + v_y;
    Position res{x, y};
    double radius =
        group_engine.gauss(LOCATION_RADIUS_MEAN, LOCATION_RADIUS_DEV); // arbitrary way to generate Locatio radius, still WIP
    return {res, radius, cluster_index};
}

Location rand_loc(Position blh_corner, Position trh_corner, double radius, int cluster_index,Random& cluster_engine)
{
    return {rand_pos(blh_corner, trh_corner,cluster_engine), radius, cluster_index};
}
} // namespace smooth_sim