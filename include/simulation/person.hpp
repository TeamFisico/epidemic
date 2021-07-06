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
    Status Current_status;
    Status New_status;
    Position Pos;
    Location Home;
    int Label;

  public:
    Person(Status current_status, Position pos, Status new_condition, Location home, int cluster_index); // Constructor
    Person() = delete;// Default Constructor

    // Return reference to person position
    Position& pos() { return Pos; }

    // Return person new Status
    Status get_new_status() const { return New_status; } //Never Used

    // Return person current status
    Status get_current_status() const { return Current_status; }

    //Return person label
    int get_label() const { return Label; }

    // Set person new Status
    void set_new_status(Status new_person_status) { New_status = new_person_status; }

    // Set person current status
    void set_current_status(Status current_status) { Current_status = current_status; }

    // Return a pointer to location home
    Location* get_home_pointer(); // return pointer to Location home

    // Function that assign nex_condition to condition, to be called at the end of the step(or cycle)
    void update_status();

    // Function that tell if person is in radius of pointed location
    bool at_location(Location* loc);
};

} // namespace smooth_sim

#endif