#include "person.hpp"
#include "configuration.h"
#include <vector>
namespace SMOOTH{

int constexpr population_size = POPULATION;


class Simulation{
  private:
    static Area world;
    static std::array<Person, population_size> People;     // People in the simulation
  public:


    void assign_to_cluster();
    void assign_home(int label);

    friend class MobilityModel;  //Mobility model has access to
};



}



