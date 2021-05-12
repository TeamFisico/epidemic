#include "person.hpp"
#include "location.hpp"
#include <vector>
namespace sim
{
class mobility_model
{
  private:
    Person* person;
    std::vector<Location*> Path; //Vector with pointer to the location the person need to visit, in no particular order
    Location* target_location; // current target_location
    double speed;
    int stay; //How much time people stay at a location
    double home_probability;
    bool at_home; //true if the pointed person is at home, false otherwise
    double alpha; //exponent of the LATP algorithm, used to calculate the next location
  public:
    mobility_model(Person *person, Location* target_location, double speed, int stay, double home_probability, bool at_home); //Constructor
    mobility_model(); //Default constructor
    void next_location(); //Randomly select the new location to visit; or home if you do not have anymore location to visit
    void move(); //move The pointed Person to the pointed Location(or home)

};

double rand_speed(double min, double max);
int rand_stay(int min, int max);
}