#include "person.hpp"
#include "location.hpp"
#include <vector>
namespace sim
{
class mobility_model
{
  private:
    Person* person;
    Location* target_location;
    double speed;
    int stay; //How much time people stay at a location
    int location_to_visit; //number of location to visit before returning home
    double home_probability;
    bool at_home; //true if the pointed person is at home, false otherwise
  public:
    mobility_model(Person *person, Location* target_location, double speed, int stay, int location_to_visit, double home_probability, bool at_home); //Constructor
    mobility_model(); //Default constructor
    void next_location(std::vector<Location*> *location_list); //Randomly select the new location to visit; or home if you do not have anymore location to visit
    void move(); //move The pointed Person to the pointed Location(or home)

};

double rand_speed(double min, double max);
int rand_stay(int min, int max);
}