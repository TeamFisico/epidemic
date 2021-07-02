#ifndef PERSON_HPP
#define PERSON_HPP
#include "location.hpp"
#include "position.hpp"

namespace smooth_sim
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
    int cluster_index;

  public:
    Person(State condition, Position pos, State new_condition, Location home, int cluster_index); // Constructor
    Person();                                                                  // Default Constructor
    Position &get_pos(){ return pos; }                                                        // return reference to person position
    State& get_condition(); // return person condition
    State& get_new_condition(){ return new_condition; }
    int get_cluster_index(){ return cluster_index; }
    void set_conditions(State cond); //function that set both condition and next_condition as cond; used for Cluster class constructor
    Location *get_home();  // return pointer to Location home
    void pass_condition(); // function that assign nex_condition to condition, to be called at the end of the day
                           // Simulation::spread function
    bool at_location(Location *loc); // tell if person is in radius of pointed location
};

} // namespace sim

#endif