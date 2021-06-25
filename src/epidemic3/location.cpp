#include "location.hpp"
#include "parameters.hpp"

namespace smooth_simulation
{

/////////////////////////////////////////////////////
////////        LOCATION CONSTRUCTOR 1        ///////
/////////////////////////////////////////////////////
// construct a location with a randomly generated radius
Location::Location(Position const& loc_position) : position{loc_position}, radius{generate_radius()}
{
}
/////////////////////////////////////////////////////
////////        LOCATION CONSTRUCTOR 2        ///////
/////////////////////////////////////////////////////
// used when position == home
Location::Location(const Position& loc_position, double radius) : position{loc_position}, radius{radius}
{
}
/////////////////////////////////////////////////////
////////        LOCATION CONSTRUCTOR 3       ///////
/////////////////////////////////////////////////////
// used when setting label(waypoints plot)
Location::Location(const Position& loc_position, int label)
    : position{loc_position}, radius{generate_radius()}, label{label}
{
}
/////////////////////////////////////////////////////
////////    LOCATION DEFAULT CONSTRUCTOR      ///////
/////////////////////////////////////////////////////
Location::Location() : position{0.0, 0.0}, radius{generate_radius()}, label{0}
{
}
/////////////////////////////////////////////////////
////////          GET MINIMUM DISTANCE        ///////
/////////////////////////////////////////////////////
// get minimum distance between the position of this location and another location
//(radius considered). Note:Used when moving people
double Location::minimum_distance_to(Location const& other_loc) const
{
    return (position.distance_to(other_loc.position) - other_loc.radius);
}
// equality operator
bool operator==(const Location& l1, const Location& l2)
{
    return (l1.get_position() == l2.get_position() && l1.get_radius() == l2.get_radius());
}

/////////////////////////////////////////////////////
////////          RADIUS GENERATION           ///////
/////////////////////////////////////////////////////
double generate_radius()
{
    // TODO come up with a good generation -->must depend on Area,number of waypoints ecc
    return 0.5;
}

} // namespace smooth_simulation
