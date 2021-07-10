#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "mobility_model.hpp"
#include "world.hpp"
#include <vector>
namespace smooth_sim
{

struct Data
{
    int S;
    int E;
    int I;
    int R;
};

class Simulation
{
  private:
    Random sim_engine;    // Random engine used for random generation in this class
    World wrld;           // World where the simulation takes place,made up of Clusters,Groups,People
    double alpha;         // Epidemic parameter: governs the lag between infectious contact and showing symptoms
    double beta;          // Epidemic parameter: number of people an infective person infects each day
    double gamma;         // Epidemic parameter: probability to recover or die (accounts for two parameters)
    double spread_radius; // Epidemic parameter: maximum distance for two people to be considered close
  public:
    // Constructor
    Simulation(int S, int E, int I, int R, int number_of_clusters, int number_of_Locations, int side, double alpha,
               double beta, double gamma, double spread_radius);
    // Prevent the compiler from generating the default constructor since no use of it should be made
    Simulation() = delete;

  private:
    // Fill an input vector with the current close people to a person (under spread_radius) from all over the World
    void close_people(Person& current_person, std::vector<Person*>& close_people_v);

    // Fill an input vector with the current close people to a person (under spread_radius) from the person Cluster
    void close_cluster_people(Person& current_person, std::vector<Person*>& close_people_v);

    // Move algorithm in the case of a Green Cluster
    void move_white(Cluster& cluster, std::vector<double>& weights_v);

    // Move algorithm in the case of a Yellow Cluster
    void move_yellow(Cluster& cluster);

    // Move algorithm in the case of a Red Cluster
    void move_red(Cluster& cluster);

    // Updates people status to the next one determined by spread
    void update_people_status();

  public:
    // Returns a reference to the current world state
    World& world();

    // Get the summary data from i-nth cluster
    Data get_Cluster_data(int i);

    // Get the summary data.
    Data get_data();

    // Clean person's path, removing all target locations which are not in green clusters anymore
    void clean_path(Mobility_model& person);

    // Clean person's path, removing all location that are not in the person's cluster
    void clean_cluster_path(Mobility_model& person);

    // Update Clusters Color based on infected population
    void update_zones();

    // Spread the epidemic across world
    void spread();

    // Move step of the simulation
    void move();
};

} // namespace smooth_sim

#endif
