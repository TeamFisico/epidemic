#include <person.hpp>

namespace smooth_sim
{
// Constructor
Person::Person(State condition, Position pos, State new_condition, Location home, int cluster_index)
    : condition{condition},
      pos{pos},
      new_condition{new_condition},
      home{home},
      cluster_index{cluster_index}
{
}
// Default constructor
/*Person::Person()
: condition{}, pos{}, newcondition{}, home{}
{}*/

State &Person::get_condition()
{
    return condition;
}

void Person::set_conditions(State cond)
{
    condition = cond;
    new_condition = cond;
}

Location *Person::get_home()
{
    return &home;
}

void Person::pass_condition()
{
    condition = new_condition;
}

bool Person::at_location(Location *loc)
{
    return (pos.InRadius(loc->get_pos(), loc->get_radius()));
}

} // namespace smooth_sim