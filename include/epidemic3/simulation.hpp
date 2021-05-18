#include "person.hpp"
#include "configuration.h"
#include <vector>
namespace SMOOTH{

int constexpr population_size = POPULATION;


class Simulation{
    Area world;
    std::array<Person, population_size> People;     // People in the simulation

    void assign_home(int label);

};


}



