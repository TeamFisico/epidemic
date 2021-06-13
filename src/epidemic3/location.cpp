#include "location.hpp"
#include "parameters.hpp"


namespace smooth_simulation
{

/////////////////////////////////////////////////////
////////        LOCATION CONSTRUCTOR 1        ///////
/////////////////////////////////////////////////////
//construct a location with a randomly generated radius
Location::Location(Position const& loc_position)
         : position{loc_position},radius{generate_radius()}
{
}
/////////////////////////////////////////////////////
////////        LOCATION CONSTRUCTOR 2        ///////
/////////////////////////////////////////////////////
//used when position == home
Location::Location(const Position& loc_position, double radius)
         :position{loc_position},radius{radius}
{
}
/////////////////////////////////////////////////////
////////    LOCATION DEFAULT CONSTRUCTOR      ///////
/////////////////////////////////////////////////////
Location::Location()
         :position{0.0,0.0},radius{generate_radius()}
{
}

//equality operator
bool operator==(const Location& l1, const Location& l2)
{
    return (l1.get_position() == l2.get_position() &&
            l1.get_radius() == l2.get_radius());
}

/////////////////////////////////////////////////////
////////          RADIUS GENERATION           ///////
/////////////////////////////////////////////////////
//TODO come up with a good generation -->must depend on Area,number of waypoints ecc
double generate_radius()
{



}




} // namespace smooth_simulation
