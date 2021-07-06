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
    // constructor
    Person(Status current_status, Position pos, Status new_condition, Location home, int cluster_index);

    // revent the compiler from generating the default constructor since no use of it should be made
    Person() = delete;

    // Return person current status
    Status get_current_status() const;

    // Return person new Status
    Status get_new_status() const;

    // returns person position
    Position get_position() const;

    // Function that tell if person is in radius of pointed location
    bool at_location(Location* loc) const;

    // Return person label
    int get_label() const;

    // returns a  reference to person position
    Position& position();

    // returns a pointer to person home location
    Location* get_home();

    // Set person current status
    void set_current_status(Status current_person_status);

    // Set person new Status
    void set_new_status(Status new_person_status);

    // Function that assign nex_condition to condition, to be called at the end of the step(or cycle)
    void update_status();
};

} // namespace smooth_sim

#endif

//     Return a pointer to location home
//    Location* get_home_pointer(); // return pointer to Location home