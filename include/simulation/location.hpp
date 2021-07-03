#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "parameters.hpp"
#include "position.hpp"

namespace smooth_sim
{
class Location
{
  private:
    Position location_pos;
    double location_Radius; // size of the location
    int cluster_index;

  public:
    Location(Position location_pos, double location_Radius, int cluster_index);
    Location();                                     // default constructor
    Position& get_pos() { return location_pos; }    // get location position
    double get_radius() { return location_Radius; } // get location area
    const int& c_index() { return cluster_index; }
};
Location generate_close_loc(Position& pos, double min_distance, double max_distance,
                            int cluster_index); // generate a new location in a random position in the circle that has
                                                // pos as a centre and max_distance as radius
Location rand_loc(Position blh_corner, Position trh_corner, double radius,
                  int cluster_index); // generate a random location inside the specified rectangle
} // namespace smooth_sim

#endif // LOCATION_HPP