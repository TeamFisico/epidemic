#include "position.hpp"
#include "parameters.hpp"
#include "random.hpp"
#include <cmath>

namespace smooth_sim
{

//////////////////////////////////////////////
///////      POSITION CONSTRUCTOR       //////
//////////////////////////////////////////////
Position::Position(double X, double Y) : x{X}, y{Y}
{
}
//////////////////////////////////////////////
/////   POSITION DEFAULT CONSTRUCTOR    //////
//////////////////////////////////////////////
Position::Position() : x{0}, y{0}
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////           PUBLIC METHODS            /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////// GET X COORDINATE  /////////////////
double Position::get_x() const
{
    return x;
}
///////////////// GET Y COORDINATE  /////////////////
double Position::get_y() const
{
    return y;
}
///////////////// DISTANCE TO ANOTHER POSITION  /////////////////
double Position::distance_to(Position const& a) const
{
    return std::sqrt((x - a.x) * (x - a.x) + (y - a.y) * (y - a.y));
}
///////////////// IS IN RADIUS WITH OTHER POSITION?  /////////////////
bool Position::in_radius(Position other, double r) const
{
    if (((x - other.x) * (x - other.x)) + ((y - other.y) * (y - other.y)) <= r * r) { return true; }
    return false;
}
///////////////// MOVE POSITION TOWARD A SPECIFIC TARGET  /////////////////
void Position::move_toward(Position target, double speed, Random& engine)
{
    double dx = target.x - x;
    double dy = target.y - y;
    double angle = std::atan2(dy, dx);
    double delta_angle =
        engine.uniform(-1.0 * MAXIMUM_ANGLE_VARIATION, MAXIMUM_ANGLE_VARIATION); // random variation from -5° to 5°
    while (distance_to(target) < speed)
    {
        speed = engine.uniform(0, 2);
    }
    double v_x = speed * std::cos(angle + delta_angle); // x component of velocity vector
    double v_y = speed * std::sin(angle + delta_angle); // y component of velocity vector
    x += v_x;
    y += v_y;
}
///////////////// GENERATE A RANDOM POSITION IN RECTANGLE  /////////////////
Position rand_pos(Position blh_corner, Position trh_corner, Random& eng)
{
    return {eng.uniform(blh_corner.get_x(), trh_corner.get_x()), eng.uniform(blh_corner.get_y(), trh_corner.get_y())};
}

} // namespace smooth_sim