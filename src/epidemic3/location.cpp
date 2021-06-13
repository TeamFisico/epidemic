#include "location.hpp"
#include "parameters.hpp"
#include <cassert>
#include <cmath>

namespace SMOOTH
{

/////////////////////////////////////////////////////
////////        LOCATION CONSTRUCTOR          ///////
/////////////////////////////////////////////////////
Location::Location(double X, double Y) : x{X}, y{Y}
{
    assert((x >= 0.0000000 && y >= 0.00000000));
}

// default constructor
Location::Location() : x{0.0}, y{0.0}
{
}

double Location::distance(Location loc2) const
{
    double dx = (x - loc2.x) * (x - loc2.x);
    double dy = (y - loc2.y) * (y - loc2.y);
    return sqrt(dx + dy);
}

/////////////////////////////////////////////////////
////////             IN RADIUS                ///////
/////////////////////////////////////////////////////
bool Location::in_radius(Location loc2, double r) const
{
    double dx = std::abs(loc2.x - x);
    double dy = std::abs(loc2.y - y);
    return (dx <= r && dy <= r);
}
/////////////////////////////////////////////////////
////////      UNDER TRANSMISSION RANGE        ///////
/////////////////////////////////////////////////////
bool Location::within_transmission_range(Location loc2) const
{
      return distance(loc2) <= TRANSMISSION_RANGE;
}

bool operator==(const Location& l1, const Location& l2)
{
    return (l1.X() == l2.X() && l1.Y() == l2.Y());
}

} // namespace SMOOTH








