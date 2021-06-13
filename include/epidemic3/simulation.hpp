#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "cluster.hpp"
#include <array>
#include <vector>

namespace SMOOTH
{

class Simulation
{
  private:
    double side; // side of the simulation area
    double spread_radius;
    double alpha; // parameter of LATP algorithm
    Data data;    // Data of People status of the simulation
  public:
    static std::array<Location, WAYPOINTS_SIZE> Waypoints; // array with waypoints
    static std::array<Cluster, CLUSTERS_SIZE> Clusters;    // array with Clusters in the simulation
    static std::array<Person, POPULATION_SIZE> People;     // People in the simulation
  private:
    void partition_in_clusters();               // divide area in clusters
    Location first_group_step(int label) const; //
    Location other_groups_step(Location const& prev_waypoint) const;
    Location plot_nearby_waypoints(int cluster_label, int group_label, Location const& starting_waypoint) const;
    void plot_waypoints();
    void assign_to_cluster(); // assign each person to a Cluster
    void assign_home();       // assign each person a home
    void update_zones();      // check the numbers of the epidemic and change Zones color
  public:
    Simulation(double side,double spread_radius, double alpha, Data data); // constructor
    void check_zones(); // verify Clusters color
    void evolve();
    void move();
    void spread();
};

  double weight_function(double distance, double LATP_parameter); // weight function of LATP algorithm
  bool check_group(Group const& group,Location try_location);                            //helper function for first_group_step()
  bool check_cluster(Cluster const& cluster,Location try_location);     //helper function for first_group_step()
  bool check_labeled_clusters(int label, Location try_waypoint);  //helper function for first_group_step()

} // namespace SMOOTH

#endif // SIMULATION_HPP









