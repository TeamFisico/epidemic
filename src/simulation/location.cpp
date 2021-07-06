#include "location.hpp"
#include "parameters.hpp"
#include "random.hpp"
#include <cmath>

namespace smooth_sim
{

//////////////////////////////////////////////
///////      LOCATION CONSTRUCTOR       //////
//////////////////////////////////////////////
Location::Location(Position location_position, double location_radius, int cluster_index)
    : pos{location_position},
      radius{location_radius},
      cluster_index{cluster_index}
{
}
///////////////// LOCATION CENTRE POSITION /////////////////
Position Location::get_position() const
{
    return pos;
}
///////////////// LOCATION RADIUS /////////////////
double Location::get_radius() const
{
    return radius;
}
///////////////// LOCATION LABEL(CLUSTER INDEX) /////////////////
int Location::get_label() const
{
    return cluster_index;
}
///////////////// GENERATE A CLOSE LOCATION /////////////////
Location generate_close_loc(Position& pos, double min_distance, double max_distance, int cluster_index, Random& engine)
{
    double angle = engine.uniform(0, 2 * PI);
    double distance = engine.uniform(min_distance, max_distance);

    double v_x = distance * std::cos(angle); // x coordinate of the translation vector
    double v_y = distance * std::sin(angle); // y coordinate of the translation vector
    double x = pos.get_x() + v_x;
    double y = pos.get_y() + v_y;

    Position gen_pos{x, y};               // generated position
    double radius = engine.rand_radius(); // random location radius

    return {gen_pos, radius, cluster_index};
}
///////////////// GENERATE HOME LOCATION /////////////////
Location generate_home_loc(Position blh_corner, Position trh_corner, double radius, int cluster_index, Random& engine)
{
    return {rand_pos(blh_corner, trh_corner, engine), radius, cluster_index};
}

} // namespace smooth_sim