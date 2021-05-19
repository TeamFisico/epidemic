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
    Person *person;                  //ptr to person subject of the modelling
    Location *Waypoints_to_visit;    //ptr to the locations array
    int still_to_visit;              //num of waypoints to visit from the Waypoints_to_visit
    Location *target_location;       //ptr to next target location
    double speed;                    //person speed
    double y;                        // percent_waypoint input of the program
    double alpha;                    // parameter of LATP algorithm
    double stay;                     // time the person will stay at a location
  public:
    MobilityModel(Person *person, Location *Waypoints_to_visit, int still_to_visit, Location *target_location,
                  double speed, double stay);

    void move();
};

} // namespace SMOOTH

#endif // MOBILITY_MODEL_HPP