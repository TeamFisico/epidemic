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
    Location(Position const& loc_position, double location_radius = 0.0);
    Location();                   // default constructor

    Position get_position() const { return position; }
    double get_radius() const { return radius; }
    void set_radius(double rad) { radius = rad; }
    void set_position(Position pos) { position = pos; }

    bool in_radius(Location loc2, double r) const;
};
    bool operator==(const Location& l1, const Location& l2); // equality operator
    Location generate_close_location(Position const& center,double min_distance, double max_distance);

} // namespace SMOOTH

#endif // LOCATION_HPP