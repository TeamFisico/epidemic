#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "position.hpp"

namespace smooth_simulation
{

class Location
{
  private:
    Position position;
    double radius;

  public:
    explicit Location(Position const& loc_position);
    Location(Position const& loc_position, double radius);
    Location(); // default constructor

    Position get_position() const { return position; }
    double get_X() const { return position.get_X(); }
    double get_Y() const { return position.get_Y(); }
    double get_radius() const { return radius; }
    double minimum_distance_to(Location const& other_loc) const;
    void set_position(Position pos) { position = pos; }
    // void set_radius(double rad) { radius = rad; }
};
bool operator==(const Location& l1, const Location& l2); // equality operator
double generate_radius();

} // namespace smooth_simulation

#endif // LOCATION_HPP