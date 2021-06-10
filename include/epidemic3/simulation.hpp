#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "cluster.hpp"
#include "configuration.h"
#include <array>
#include <vector>

namespace SMOOTH
{

int constexpr CLUSTERS_SIZE = CLUSTERS;
int constexpr WAYPOINTS_SIZE = WAYPOINTS;
int constexpr POPULATION_SIZE = POPULATION;
double constexpr TIME_STEP = 0.5; // seconds
double constexpr MAX_PAUSE = 0.0;
double constexpr MIN_PAUSE = 0.0;

class Simulation
{
  private:
    double side; // side of the simulation area
    double R;    // transmission range
    double spread_radius;
    double alpha;     // parameter of LATP algorithm
    double y;         // percent waypoint
    double lambda;    // exponent of the pause-time distribution
    double min_pause; // minimum stay at a place
    double max_pause; // maximum stay at a place
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
    void fill_path_current(Person& person); // select next visiting locations from the person cluster
    void fill_path_white(Person& person);   // select next visiting locations from green clusters
    void assign_to_cluster();               // assign each person to a Cluster
    void assign_home(int label);            // assign each person a home
  public:
    Simulation(double side, double transmission_range, double spread_radius, double alpha, double percent_waypoint,
               double minimum_pause, double maximum_pause); // constructor
    void check_zones();                                     // verify Clusters color
    void evolve();
    void move();
    void spread();
};

double weight_function(double distance, double LATP_parameter); // weight function of LATP algorithm

} // namespace SMOOTH

#endif // SIMULATION_HPP