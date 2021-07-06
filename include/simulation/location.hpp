#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "parameters.hpp"
#include "position.hpp"

namespace smooth_sim
{
class Location
{
  private:
    Position Location_pos;
    double Location_Radius; // size of the location
    int Cluster_index;

  public:
    Location(Position location_pos, double location_Radius, int cluster_index);

    // Get location position
    Position get_pos() { return Location_pos; }

    // Get location radius
    const double get_radius()  { return Location_Radius; }

    //get The index of the cluster in which the location is situated
    const int get_cluster_index() { return Cluster_index; }
};

// Generate a location in a random position in the circle that has pos as a centre and max_distance as radius
Location generate_close_loc(Position& pos, double min_distance, double max_distance, int cluster_index,
                            Random& group_engine);

// Generate a random location inside the specified rectangle
Location rand_loc(Position blh_corner, Position trh_corner, double radius, int cluster_index,
                  Random& cluster_engine); // generate a random location inside the specified rectangle
} // namespace smooth_sim

#endif // LOCATION_HPP