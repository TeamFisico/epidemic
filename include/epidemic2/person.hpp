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
    // Position home;
  public:
    Person(State condition, Position pos, State new_condition); // Constructor
    Person();                                                   // Default Constructor
    Position *get_pos();                                        // return pointer to person position
    State get_condition();                                      // return person condition
    void next_condition();
};

}