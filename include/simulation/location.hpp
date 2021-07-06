#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "parameters.hpp"
#include "position.hpp"

namespace smooth_sim
{
class Location
{
  private:
    Position pos;      // centre position
    double radius;     // radius taken up by the location on the simulation area
    int cluster_index; // cluster to which the Location belongs

  public:
    // Constructor
    Location(Position location_pos, double location_Radius, int cluster_index);
    Location() = delete;

    // returns centre position of this location
    Position get_position() const;

    // returns radius of this location
    double get_radius() const;

    // returns index of the cluster to which this location belongs
    int get_label() const;
};

///////////////// HELPER FUNCTIONS /////////////////

// returns a randomly generated location within a distance d, min_distance <= d < max_distance, from the input position
Location generate_close_loc(Position& pos, double min_distance, double max_distance, int cluster_index, Random& engine);

// returns a randomly selected location inside the rectangular area of a specific cluster
Location generate_home_loc(Position blh_corner, Position trh_corner, double radius, int cluster_index, Random& engine);

} // namespace smooth_sim

#endif // LOCATION_HPP