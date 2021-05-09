#include "position.hpp"
#include "location.hpp"

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
    Person();                                                   // Default Constructor
    Position *get_pos();                                        // return pointer to person position
    State get_condition();                                      // return person condition
    void next_condition();
    Location* get_home(); //return pointer to Location home
};

}