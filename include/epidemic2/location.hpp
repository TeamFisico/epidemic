#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "position.hpp"

namespace sim{
class Location{
  private:
    Position location_pos;
    double location_Radius; //size of the location
  public:
    Location(Position& location_pos, double location_Radius);
    Location(); //default constructor
    Position get_pos(); //get location position
    double get_radius(); //get location area

};
Location generate_close_loc(Position& pos, double max_distance); //generate a new location in a random position in the circle that has pos as a centre and max_distance as radius
}

#endif // LOCATION_HPP