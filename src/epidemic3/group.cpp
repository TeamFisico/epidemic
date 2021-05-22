#include "group.hpp"
#include <cassert>

namespace SMOOTH
{

// constructor
Group::Group(int size, int label) : sz{size}, lbl{label}
{
    assert(size > 0);
}

// default constructor
Group::Group() : sz{0}
{
}

// let this group's pointer point to the nth element of the input array
void Group::set_to_waypoint(std::array<Location, WAYPOINTS> &wpts, int n)
{
    assert(n < WAYPOINTS);
    group_ptr = &(wpts[n]);
    assert(group_ptr);
}

} // namespace SMOOTH