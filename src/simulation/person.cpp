#include <person.hpp>

namespace smooth_sim
{
// Constructor
Person::Person(Status current_status, Position position, Status new_status, Location home, int cluster_index)
    : status{current_status},
      new_status{new_status},
      pos{position},
      home{home},
      label{cluster_index}
{
}

Location* Person::get_home()
{
    return &home;
}

void Person::update_status()
{
    status = new_status;
}

bool Person::at_location(Location* loc)
{
    return (pos.InRadius(loc->get_pos(), loc->get_radius()));
}

} // namespace smooth_sim