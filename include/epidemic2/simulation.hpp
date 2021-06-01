#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "world.hpp"
#include "mobility_model.hpp"
#include <vector>
namespace sim
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
    World world;
    std::vector<mobility_model> people;
    double alpha;
    double beta;
    double gamma;
    double spread_radius;
    int time_in_minutes;
    int step_in_minutes;

  public:
    Simulation(int S, int E, int I, int R,int number_of_clusters, int number_of_Locations, double Side, double alpha, double gamma,
               double beta, double spread_radius, int time_in_days, int step_in_minutes);
    std::vector<Person *> Close_People(Person &current_person); // function that put in a vector pointers to all
                                                                // other inRadius Susceptible People
    Data get_data();                                            // get the summary data.
    std::vector<Location *> green_loc_list(); //list of locations of non_red clusters
    void spread();   // Function that, if *this is I, check close People and try
                     // to spread virus to close S if *this is E or I check if the
                     // State evolve
    void move();     // function that use mobility_model class to make a move step
    void simulate(); // function that call move, spread and Person::pass_condition for the necessary amount of time, WIP
                     // concept, will vary based on other addition(as curfew, quarantine, ...)
};

} // namespace sim

#endif