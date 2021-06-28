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
    double alpha;
    double beta;
    double gamma;
    double kappa;  // death rate parameter
    Data data;     // Data of People status of the simulation
    Random engine; // marsenne twister eng 256 bit default seeded
  public:
    static std::array<Location, WAYPOINTS_SIZE> Waypoints; // array with waypoints
    static std::array<Cluster, CLUSTERS_SIZE> Clusters;    // array with Clusters in the simulation
    static std::array<Person, POPULATION_SIZE> People;     // People in the simulation
  private:
    void partition_in_clusters();
    Location first_group_step(int clust_lab);
    Location other_groups_step(Location const& prev_waypoint, int clust_lab);
    Location plot_nearby_waypoints(int clust_lab, int grp_lab, Location const& starting_waypoint);
    void plot_waypoints();
    void assign_cluster_to_people();
    void assign_home_to_people();
    void set_clusters_bounds_indeces();
    void close_people_fill(Person const& current_person, std::vector<int> close_people_i);
    void close_cluster_people_fill(Person const& current_person, std::vector<int> close_people_i);
    void world_generation();
    void initialise_people_status(int E, int I, int R);
    void update_zones(); // check the numbers of the epidemic and change Zones color
    void update_data();  // update simulation data
    void update_people_status();

  public:
    Simulation(double side, double spread_radius, double alpha, double beta, double gamma, double kappa, Data data);
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    void move();                      // move all the people
    void spread();                    // spread the disease
    void simulate();                  // simulate
    Data get_simulation_data() const; // return simulation data
};
// helper functions
double weight_function(double distance, double LATP_parameter);    // weight function of LATP algorithm
bool check_group(Group const& group, Position try_position);       // helper function for first_group_step()
bool check_cluster(Cluster const& cluster, Position try_position); // helper function for first_group_step()
bool check_labeled_clusters(int label, Position try_position);     // helper function for first_group_step()
} // namespace smooth_simulation

#endif // SIMULATION_HPP
