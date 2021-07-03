#ifndef PERSON_HPP
#define PERSON_HPP
#include "location.hpp"
#include "position.hpp"

namespace smooth_sim
{
enum class Status
{
    Susceptible,
    Exposed,
    Infected,
    Recovered
};

class Person
{
  private:
    Status status;
    Status new_status;
    Position pos;
    Location home;
    int cluster_index;

  public:
    Person(Status current_status, Position pos, Status new_condition, Location home, int cluster_index); // Constructor
    Person();                                                                                     // Default Constructor
    Position& get_pos() { return pos; } // return reference to person position
    Status get_current_status() const { return status; }             // return person condition
    Status get_new_status() const { return new_status; }
    void set_new_status(Status new_person_status){ new_status = new_person_status; }
    void set_current_status(Status current_status){ status = current_status; }
    int get_cluster_index() { return cluster_index; }
    Location* get_home();  // return pointer to Location home
    void pass_condition(); // function that assign nex_condition to condition, to be called at the end of the day
                           // Simulation::spread function
    bool at_location(Location *loc); // tell if person is in radius of pointed location
};

} // namespace smooth_sim

#endif