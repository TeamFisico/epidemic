#ifndef MOBILITY_MODEL_HPP
#define MOBILITY_MODEL_HPP
#include "configuration.h"
#include "person.hpp"
#include <array>
#include <vector>

namespace SMOOTH
{

class MobilityModel
{
  private:
    Person *person;               // ptr to person subject of the modelling
    Location *Waypoints_to_visit; // ptr to the locations array
    std::vector<int> Paths;
    double y;                     // percent_waypoint input of the program
    int still_to_visit;           // num of waypoints to visit from the Waypoints_to_visit
    Location *target_location;    // ptr to next target location
    double speed;                 // person speed
    double alpha;                 // parameter of LATP algorithm
    double stay;                  // time the person will stay at a location (power law)
  public:
    MobilityModel(Person *person, Location *Waypoints_to_visit, int still_to_visit, Location *target_location,
                  double speed, double stay);

    void select_waypoints();  //select waypoints from the current person cluster
    void move();
};

} // namespace SMOOTH

#endif // MOBILITY_MODEL_HPP