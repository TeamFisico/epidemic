#ifndef MOBILITY_MODEL_HPP
#define MOBILITY_MODEL_HPP
#include "configuration.h"
#include "person.hpp"
#include <array>


namespace SMOOTH
{

class MobilityModel
{
  private:
    Person *person;
    Location* Waypoints_to_visit;
    int still_to_visit;
    Location *target_location;
    double speed;
    double y; // percent_waypoint input of the program
    double alpha;         // parameter of LATP algorithm
    double stay;   //time the person will stay at a location
  public:

    MobilityModel(Person* person,Location* Waypoints_to_visit,int still_to_visit,Location* target_location,
                  double speed,double stay)
    void move();
};

} //namespace SMOOTH

#endif //MOBILITY_MODEL_HPP