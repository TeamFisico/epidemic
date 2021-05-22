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
    int cluster_label; // which cluster this person is in
    Status status;
    Location home;
    Location current_location;
    double y; // percent of waypoints visited by a person in the cluster
  public:
    void upgrade_condition();

    void set_cluster(int n) { cluster_label = n; }
    void set_home(Location loc) { home = loc; }
    Location *get_home();
};

} // namespace SMOOTH

#endif // PERSON_HPP