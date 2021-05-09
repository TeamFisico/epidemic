#include "mobility_model.hpp"
#include <random>

using namespace sim;

//Constructor
mobility_model::mobility_model(class Person *Person, Location *target_location, int speed, int stay, int location_to_visit, double home_probability)
:Person{Person}, target_location{target_location}, speed{speed}, stay{stay}, location_to_visit{location_to_visit}, home_probability{home_probability}
{
}

//Default Constructor WIP
/*mobility_model::mobility_model()
{
}*/

void mobility_model::next_location(std::vector<Location> *location_list)
{
    unsigned int last_index = location_list->size() - 1; //last vector index;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> rand(0, last_index); //random number generator from 0 to last vector index
    Location* pointed_loc = target_location; //pointer to previous current location
    while(pointed_loc == target_location)
    {
        target_location = &location_list->operator[](rand(gen)); //point to new random location from location_list, different from the previous
    }
}