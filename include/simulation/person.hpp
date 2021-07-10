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
    Status status;     // Person current status
    Status new_status; // Person new_status
    Position pos;      // Person current position
    Location home;     // Home location
    int cluster_index; // Index of the cluster where the person reside

  public:
    // Constructor
    Person(Status current_status, Position pos, Status new_condition, Location home, int cluster_index);

    // Prevent the compiler from generating the default constructor since it is not used
    Person() = delete;

    // Return person current status
    Status get_current_status() const;

    // Returns person position
    Position get_position() const;

    // Function that tell if person is in radius of pointed location
    bool at_location(Location* loc) const;

    // Return person's cluster_index
    int get_label() const;

    // Returns a reference to person position
    Position& position();

    // Returns a pointer to person home location
    Location* get_home();

    // Set person status
    void set_current_status(Status current_person_status);

    // Set person new Status
    void set_new_status(Status new_person_status);

    // Function that assign new_status to status, to be called at the end of every spread
    void update_status();
};

} // namespace smooth_sim

#endif