#include <person.hpp>

namespace smooth_sim
{
// Constructor
Person::Person(Status current_status, Position position, Status new_status, Location home, int cluster_index)
    : status{current_status},
      pos{position},
      new_status{new_status},
      home{home},
      cluster_index{cluster_index}
{
}
// Default constructor
/*Person::Person()
: condition{}, pos{}, newcondition{}, home{}
{}*/


Location *Person::get_home()
{
    return &home;
}

void Person::pass_condition()
{
    status = new_status;
}

bool Person::at_location(Location *loc)
{
    return (pos.InRadius(loc->get_pos(), loc->get_radius()));
}

} // namespace smooth_sim