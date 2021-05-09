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
}