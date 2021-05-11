#include "mobility_model.hpp"
#include <cassert>
#include <random>

using namespace sim;

// Constructor
mobility_model::mobility_model(Person *person, Location *target_location, double speed, int stay, int location_to_visit,
                               double home_probability, bool at_home)
    : person{person}, target_location{target_location}, speed{speed}, stay{stay}, location_to_visit{location_to_visit},
      home_probability{home_probability}, at_home{at_home}
{
}

// Default Constructor WIP
/*mobility_model::mobility_model()
{
}*/

//TODO Implement the distance based next location selector
void mobility_model::next_location(std::vector<Location*> *location_list)
{
    unsigned int last_index = location_list->size() - 1; // last vector index;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned int> rand(0,
                                                     last_index); // random number generator from 0 to last vector index
    Location *pointed_loc = target_location;                      // pointer to previous current location
    while (pointed_loc == target_location)
    {
        target_location = location_list->operator[](
            rand(gen)); // point to new random location from location_list, different from the previous
    }
}

void mobility_model::move()
{
    speed = rand_speed(0, 1);
}

double sim::rand_speed(double min, double max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> rand(min, max);
    return rand(gen);
}

int sim::rand_stay(int min, int max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> rand(min, max);
    return rand(gen);
}

