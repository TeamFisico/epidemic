#ifndef PERSON_HPP
#define PERSON_HPP

#include "area.hpp"
#include "location.hpp"
namespace SMOOTH
{

enum class Status
{
    Susceptible = 0,
    Latent,
    Infected,
    Recovered,
    Dead
};

class Person
{
  private:
    Status status;
    int label; // label of the cluster
    Location home;
    Location location;
    Location *target_location; // ptr to next target location
    double speed;              // person speed
    double stay;               // time the person will stay at a location (power law)
  public:
    std::vector<int> Paths; // vector with indeces to waypoints in Waypoints array
    Person();
    // non-modifying members
    int cluster_label() const { return label; }

    //   void upgrade_condition();
    Location get_location() const { return location; };
    Location get_home() const { return home; };
    void set_cluster(int n) { label = n; }
    void set_home(Location loc) { home = loc; }
    void set_target(Location loc) { target_location = &loc; }
};

} // namespace SMOOTH

#endif // PERSON_HPP
