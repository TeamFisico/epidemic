#include "location.hpp"
#include "position.hpp"

namespace sim
{
enum class State
{
    S,
    E,
    I,
    R
};

class Person
{
  private:
    State condition;
    Position pos;
    State new_condition;
    Location home;

  public:
    Person(State condition, Position pos, State new_condition, Location home); // Constructor
    Person();                                                                  // Default Constructor
    Position *get_pos();                                                       // return pointer to person position
    State get_condition();                                                     // return person condition
    void set_conditions(
        State cond); // function that set both condition and next_condition as cond; used for Cluster class constructor
    void next_condition(); // put the next condition in new_condition
    Location *get_home();  // return pointer to Location home
    void pass_condition(); // function that assign nex_condition to condition, to be called at the end of the day
                           // Simulation::spread function
    bool at_location(Location *loc); // tell if person is in radius of pointed location
};

} // namespace sim