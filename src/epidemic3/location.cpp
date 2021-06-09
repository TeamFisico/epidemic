#include "location.hpp"

#include <cassert>
#include <cmath>
namespace SMOOTH
{

// constructor
Location::Location(double X, double Y) : x{X}, y{Y}
{
    assert((x > 0.0000000 && y > 0.00000000));
}

// default constructor
Location::Location() : x{0.0}, y{0.0}
{
}

double Location::get_distance(Location loc2) const
{
    double dx = (x - loc2.x) * (x - loc2.x);
    double dy = (y - loc2.y) * (y - loc2.y);
    return sqrt(dx + dy);
}

// return true if loc2 is in a neighbourhood of *this
bool Location::in_radius(Location loc2, double r) const
{
    double dx = std::abs(loc2.x - x);
    double dy = std::abs(loc2.y - y);
    return (dx <= r && dy <= r);
}

} // namespace SMOOTH
