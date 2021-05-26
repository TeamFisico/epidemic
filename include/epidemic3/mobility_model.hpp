#ifndef MOBILITY_MODEL_HPP
#define MOBILITY_MODEL_HPP
#include "configuration.h"
#include "person.hpp"
#include "simulation.hpp"
#include <array>
#include <cmath>
#include <vector>

namespace SMOOTH
{

class MobilityModel
{
  private:
    Person *person; // ptr to person subject of the modelling
    double alpha;   // parameter of LATP algorithm
    double compute_weight(double distance);

  public:
    MobilityModel(Person *person, Location *Waypoints_to_visit, int still_to_visit, Location *target_location,
                  double speed, double stay);

    void choose_target();
    void select_waypoints(); // select waypoints from the current person cluster
    void move();
};

} // namespace SMOOTH

#endif // MOBILITY_MODEL_HPP