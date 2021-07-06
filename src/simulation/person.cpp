#include <person.hpp>

namespace smooth_sim
{
// Constructor
Person::Person(Status current_status, Position position, Status new_status, Location home, int cluster_index)
    : Current_status{current_status},
      New_status{new_status},
      Pos{position},
      Home{home},
      Label{cluster_index}
{
}

Location* Person::get_home_pointer()
{
    return &Home;
}

void Person::update_status()
{
    Current_status = New_status;
}

bool Person::at_location(Location* loc)
{
    return (Pos.InRadius(loc->get_pos(), loc->get_radius()));
}

} // namespace smooth_sim