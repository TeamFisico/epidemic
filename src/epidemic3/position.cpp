#include "position.hpp"
#include <cassert>
#include <cmath>
namespace smooth_simulation
{

/////////////////////////////////////////////////////
////////        POSITION CONSTRUCTOR          ///////
/////////////////////////////////////////////////////
Position::Position(double X, double Y)
         :x{X},y{Y}
{
    assert(x >= 0.0 && y >= 0.0);
}
Position::Position() :x{0.0},y{0.0}
{
}
double Position::distance_to(Position const& other_pos) const
{
    return sqrt((x - other_pos.x) * (x - other_pos.x) + (y - other_pos.y)*(y - other_pos.y));
}
bool Position::in_radius(const Position& other_pos, double r) const
{
    return ( this->distance_to(other_pos) <= r);
}

bool operator==(const Position& p1, const Position& p2)
{
    return (p1.get_x() == p2.get_y() && p1.get_y() == p2.get_y());
}


}