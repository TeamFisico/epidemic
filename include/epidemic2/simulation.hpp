// TODO need to add ifdef guard

#include "location.hpp"
#include "person.hpp"
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
    std::vector<Person> Population;
    std::vector<Location> Locations;
    double alpha;
    double beta;
    double gamma;
    double spread_radius;
    int time_in_days;

  public:
    Simulation(int S, int E, int I, int R, int Location_number, double people_per_home, double alpha, double gamma,
               double beta, double spread_radius, int time_in_days);
    Simulation();
    std::vector<Person *> Close_People(Person &current_person); // function that put in a vector pointers to all
                                                                // other inRadius Susceptible People
    Data get_data();                                            // get the summary data.
    void spread();   // Function that, if *this is I, check close People and try
                     // to spread virus to close S if *this is E or I check if the
                     // State evolve
    void move();     // function that use mobility_model class to make a move step
    void simulate(); // function that call move, spread and Person::pass_condition for the necessary amount of time, WIP
                     // concept, will vary based on other addition(as curfew, quarantine, ...)
};

} // namespace sim
