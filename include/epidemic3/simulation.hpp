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
    double side;          // side of the simulation area
    double spread_radius; // radius under which 2 people can infect each other
    Data data;            // Data of People status of the simulation
  public:
    static std::array<Location, WAYPOINTS_SIZE> Waypoints; // array with waypoints
    static std::array<Cluster, CLUSTERS_SIZE> Clusters;    // array with Clusters in the simulation
    static std::array<Person, POPULATION_SIZE> People;     // People in the simulation
  private:
    void partition_in_clusters(Random& engine);                   // divide area in clusters
    Location first_group_step(int clust_lab,Random& engine) const; //
    Location other_groups_step(Location const& prev_waypoint, int clust_lab,Random& engine) const;
    Location plot_nearby_waypoints(int clust_lab, int grp_lab, Location const& starting_waypoint,Random& engine) const;
    void plot_waypoints(Random& engine);
    void assign_cluster_to_people(Random& engine); // assign each person to a Cluster
    void assign_home_to_people(Random& engine);    // assign each person a home
    void set_clusters_bounds_indeces(); // set wpts range of each cindeces referred to Waypoints array
    void world_generation();
    void update_zones();             // check the numbers of the epidemic and change Zones color
  public:
    Simulation(double side, double spread_radius, Data data); // constructor
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    void check_zones(); // verify Clusters color
    void simulate();    // simulate
    void move();        // move all the people
    void spread();      // spread the disease
};
// helper functions
double weight_function(double distance, double LATP_parameter);    // weight function of LATP algorithm
bool check_group(Group const& group, Position try_position);       // helper function for first_group_step()
bool check_cluster(Cluster const& cluster, Position try_position); // helper function for first_group_step()
bool check_labeled_clusters(int label, Position try_position);     // helper function for first_group_step()
} // namespace smooth_simulation

#endif // SIMULATION_HPP
