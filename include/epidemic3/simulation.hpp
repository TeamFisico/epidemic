#include "person.hpp"
#include "configuration.h"
#include <vector>
namespace SMOOTH{

int constexpr population_size = POPULATION;
double constexpr TIME_STEP = 0.5; //seconds
double constexpr MAX_PAUSE = 0.0;
double constexpr MIN_PAUSE = 0.0;

class Simulation{
  private:
    static Area world;
    static std::array<Person, population_size> People;     // People in the simulation
    static double y;                                       //percent waypoint
    double alpha;                                          //parameter of LATP algorithm

  private:
    void select_waypoints(Person* person);
    void update_target(Person* person);
    void assign_to_cluster();
    void assign_home(int label);

  public:
    void evolve();


};

  double weight_function(double distance,double LATP_parameter); //weight function of LATP algorithm


}



