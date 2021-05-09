#include "person.hpp"
#include "location.hpp"
#include <vector>
namespace sim
{
class mobility_model
{
  private:
    Person* Person;
    Location* target_location;
    int speed;
    int stay; //How much time people stay at a location
    int location_to_visit; //number of location to visit before returning home
    double home_probability;
  public:
    mobility_model(Person* Person, Location* target_location, int speed, int stay, int location_to_visit, double home_probability); //Constructor
    mobility_model(); //Default constructor
    void next_location(std::vector<Location> *location_list); //Randomly select the new location to visit; or home if you do not have anymore location to visit
    void move(); //move The pointed Person to the pointed Location(or home)

};

}