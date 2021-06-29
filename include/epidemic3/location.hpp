#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "position.hpp"

namespace smooth_simulation
{

class Location
{
  private:
    Position position; //(x,y) position inside the World
    double radius;     // radius of this location
    int label;         // cluster this location is in
  public:
    explicit Location(Position const& loc_position);
    Location(Position const& loc_position, double radius);
    Location(Position const& loc_position, int label);
    Location(); // default constructor

    Position get_position() const { return position; }
    double get_X() const { return position.get_X(); }
    double get_Y() const { return position.get_Y(); }
    double get_radius() const { return radius; }
    int get_label() const { return label; }
    double minimum_distance_to(Location const& other_loc) const;
    void set_position(Position pos) { position = pos; }
    void set_label(int cluster_index) { label = cluster_index; }
    bool in_radius(Position pos);
};
bool operator==(const Location& l1, const Location& l2); // equality operator
double generate_radius();

} // namespace smooth_simulation

#endif // LOCATION_HPP