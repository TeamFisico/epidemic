#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "mobility_model.hpp"
#include "world.hpp"
#include <vector>
namespace smooth_sim
{

struct Data
{
    unsigned int S;
    unsigned int E;
    unsigned int I;
    unsigned int R;
};

class Simulation
{
  private:
    Random sim_engine;    // Random engine used for random generation in this class
    World world;          // World where the simulation takes place,made up of Clusters,Groups,People
    double alpha;         // epidemic parameter: see
    double beta;          // epidemic parameter: see
    double gamma;         // epidemic parameter: see
    double spread_radius; // epidemic parameter: maximum distance for two people to be considered close
    int time_in_minutes;  // simulation time(m)
  public:
    Simulation(int S, int E, int I, int R, int number_of_clusters, int number_of_Locations, double Side, double alpha,
               double gamma, double beta, double spread_radius, int time_in_days);
    Simulation() = delete;

  private:
    // fill an input vector with the current close people to a person (under spread_radius) from all over the World
    void close_people(Person& current_person, std::vector<Person*>& close_people_v);

    // fill an input vector with the current close people to a person (under spread_radius) from the person Cluster
    void close_cluster_people(Person& current_person, std::vector<Person*>& close_people_v);

    // clean person's path, removing all location that are not in green cluster
    void clean_path(Mobility_model& person);

    // move algorithm in the case of a Green Cluster
    void move_white(Cluster& cluster, std::vector<double>& weights_v);

    // move algorithm in the case of a Yellow Cluster
    void move_yellow(Cluster& cluster);

    // move algorithm in the case of a Red Cluster
    void move_red(Cluster& cluster);

    // updates people status to the next one determined by spread
    void update_people_status();

  public:
    // return a reference to the current world
    World& world_ref();

    // get the summary data from i-nth cluster
    Data get_Cluster_data(int i);

    // get the summary data.
    Data get_data();

    // Update Clusters Color based on infected population
    void update_zones();

    // spread the epidemic across world
    void spread();

    void move();
    void simulate(); // function that call move, spread and Person::pass_condition for the necessary amount of time, WIP
                     // concept, will vary based on other addition(as curfew, quarantine, ...)
    Position person_pos(int cluster_index, int person_index);
    bool at_home(int cluster_index, int person_index);
    // unused   std::vector<Location *> green_loc_list(); // list of locations of non_red clusters
};

} // namespace smooth_sim

#endif
