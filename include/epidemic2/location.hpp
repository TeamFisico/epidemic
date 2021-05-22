#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "position.hpp"

#define TRANSMISSION_RANGE 0.5
#define HOME_RADIUS 0.5

namespace sim{
class Location{
  private:
    Position location_pos;
    double location_Radius; //size of the location
  public:
    Location(Position& location_pos, double location_Radius);
    Location(); //default constructor
    Position get_pos(){return location_pos;} //get location position
    double get_radius(){return location_Radius;} //get location area

};
Location generate_close_loc(Position& pos,double min_distance, double max_distance); //generate a new location in a random position in the circle that has pos as a centre and max_distance as radius
Location rand_loc(Position& blh_corner, Position& trh_corner, double radius); //generate a random location inside the specified rectangle
}

#endif // LOCATION_HPP