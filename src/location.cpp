#include "location.hpp"

using namespace sim;
//Constructor
Location::Location(Position &location_pos, double location_Radius)
: location_pos{location_pos}, location_Radius{location_Radius}
{
}
//Default Constructor WIP
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
