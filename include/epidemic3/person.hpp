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
    Cluster* cluster;   //Cluster to which the person belongs
    int label;          //label of the cluster
    Status status;
    Location home;
    Location current_location;
  public:

    //non-modifying members
    int cluster_label() const { return label; }

    void upgrade_condition();

    void set_cluster(int n, Cluster* clust) { label = n; cluster = clust; }
    void set_home(Location loc) { home = loc; }
    Location *get_home();



};

} // namespace SMOOTH

#endif // PERSON_HPP
















