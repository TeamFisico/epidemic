#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "cluster.hpp"
#include <array>
#include <vector>

namespace smooth_simulation
{

class Simulation
{
  private:
    double side; // side of the simulation area
    double spread_radius;
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
    void world_generation();
    void assign_cluster_to_people(); // assign each person to a Cluster
    void assign_home_to_people();       // assign each person a home
    void update_zones();      // check the numbers of the epidemic and change Zones color
  public:
    Simulation(double side, double spread_radius,Data data); // constructor
    void check_zones();                                                     // verify Clusters color
    void evolve();
    void move();
    void spread();
};

double weight_function(double distance, double LATP_parameter);    // weight function of LATP algorithm
bool check_group(Group const& group, Position try_position);       // helper function for first_group_step()
bool check_cluster(Cluster const& cluster, Position try_position); // helper function for first_group_step()
bool check_labeled_clusters(int label, Position try_position);     // helper function for first_group_step()

} // namespace smooth_simulation

#endif // SIMULATION_HPP
