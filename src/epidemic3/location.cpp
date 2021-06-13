#include "location.hpp"
#include "parameters.hpp"


namespace smooth_simulation
{

/////////////////////////////////////////////////////
////////        LOCATION CONSTRUCTOR          ///////
/////////////////////////////////////////////////////
Location::Location(Position const& loc_position, double location_radius)
         : position{loc_position},radius{location_radius}
{
}
/////////////////////////////////////////////////////
////////    LOCATION DEFAULT CONSTRUCTOR      ///////
/////////////////////////////////////////////////////
Location::Location()
         :position{0.0,0.0},radius{0.0}
{
}

//equality operator
bool operator==(const Location& l1, const Location& l2)
{
    return (l1.get_position() == l2.get_position() &&
            l1.get_radius() == l2.get_radius());
}

Location generate_close_location(Position const& center,double min_distance, double max_distance)
{
    //TODO Define
}



} // namespace SMOOTH
