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

    Random sim_engine;     //Random engine used for random generation in this class
    World world;           // World where the simulation takes place,made up of Clusters,Groups,People
    double alpha;          // epidemic parameter: see
    double beta;           // epidemic parameter: see
    double gamma;          // epidemic parameter: see
    double spread_radius;  // epidemic parameter: maximum distance for two people to be considered close
    int time_in_minutes;
    int step_in_minutes;

  public:
    Simulation(int S, int E, int I, int R, int number_of_clusters, int number_of_Locations, double Side, double alpha,
               double gamma, double beta, double spread_radius, int time_in_days, int step_in_minutes);
    Simulation() = delete;
  private:

    // fill an input vector with the current close people to a person (under spread_radius) from all over the World
    void Close_People(Person& current_person, std::vector<Person*>& close_people_v);

    // fill an input vector with the current close people to a person (under spread_radius) from the person Cluster
    void Close_Cluster_People(Person& current_person, std::vector<Person*>& close_people);

    // clean person's path, removing all location that are not in green cluster
    void clean_path(mobility_model& person);

    //move algorithm in the case of A Green Cluster
    void move_white(Cluster& cluster, std::vector<double>& weights);

    //move algorithm in the case of A Yellow Cluster
    void move_yellow(Cluster& cluster);

    //move algorithm in the case of A Red Cluster
    void move_red(Cluster& cluster);

  public:

    World& get_world() { return world; }

    Data get_Cluster_data(int i); // get the summary data from i-nth cluster
    Data get_data();              // get the summary data.
    // unused   std::vector<Location *> green_loc_list(); // list of locations of non_red clusters
    void update_Colors(); // Update Clusters Color based on infected population
    void spread();        // Function that, if *this is I, check close People and try
                          // to spread virus to close S if *this is E or I check if the
                          // State evolve
    void update_people_status();
    void move();
    void simulate(); // function that call move, spread and Person::pass_condition for the necessary amount of time, WIP
                     // concept, will vary based on other addition(as curfew, quarantine, ...)
    Position person_pos(int cluster_index, int person_index);
    bool at_home(int cluster_index, int person_index);
};

} // namespace smooth_sim

#endif












