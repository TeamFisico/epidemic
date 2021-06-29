#include "position.hpp"
#include "../random.hpp"
#include "parameters.hpp"
#include <cassert>
#include <cmath>
namespace smooth_simulation
{

/////////////////////////////////////////////////////
////////        POSITION CONSTRUCTOR          ///////
/////////////////////////////////////////////////////
Position::Position(double X, double Y)
       : x{X},
         y{Y}
{
    check_bounds_and_resize(SIMULATION_SIDE);
}
Position::Position() : x{0.0}, y{0.0}
{
}
double Position::distance_to(Position const& other_pos) const
{
    return sqrt((x - other_pos.x) * (x - other_pos.x) + (y - other_pos.y) * (y - other_pos.y));
}
bool Position::in_radius(const Position& other_pos, double r) const
{
    return (this->distance_to(other_pos) <= r);
}

/////////////////////////////////////////////////////
/// CHECK IF A POSITION IS OUT OF BOUNDS-->RESIZE ///
/////////////////////////////////////////////////////
void Position::check_bounds_and_resize(double area_side)
{
    if (x < 0) { x = 0; }
    if (x > area_side ){ x = area_side; }
    if (y < 0) { y = 0; }
    if (y > area_side ){ y = area_side; }
}

bool operator==(const Position& p1, const Position& p2)
{
    return (p1.get_X() == p2.get_X() && p1.get_Y() == p2.get_Y());
}

/////////////////////////////////////////////////////
////////      CLOSE LOCATION GENERATION       ///////
/////////////////////////////////////////////////////
// generate a location within a maximum distance from a center position
Position generate_close_position(Position const& center, double max_distance,Random& engine)
{
    double angle = engine.uniform(0, 2 * PI);
    double distance = engine.uniform(0, max_distance);
    double v_x = distance * cos(angle); // x coordinate of the traslation vector
    double v_y = distance * sin(angle); // y coordinate of the traslation vector
    double x = center.get_X() + v_x;
    double y = center.get_Y() + v_y;

    Position gen_pos{x, y};
    return gen_pos;
}

} // namespace smooth_simulation