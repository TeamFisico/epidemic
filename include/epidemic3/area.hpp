#ifndef AREA_HPP
#define AREA_HPP

#include "cluster.hpp"
#include "configuration.h"
#include <array> //using STL array (provides optional bound checking, iterators, ecc to low performance loss)
#include <stdexcept>

namespace SMOOTH
{

// TODO try-catch for checking
// getting some program inputs from Cmake Variables and doing type check
// so that the program won't compile in case of wrong input
int constexpr CLUSTERS_SIZE = CLUSTERS;
int constexpr WAYPOINTS_SIZE = WAYPOINTS;

class Area
{
  private:
    double sd; // side of the simulation area
    double R;  // transmission Range
  public:
    static std::array<Location, WAYPOINTS_SIZE> Waypoints; // array with waypoints
    static std::array<Cluster, CLUSTERS_SIZE> Clusters;    // array with Clusters in the simulation
  private:
    void partition_in_clusters();               // divide area in clusters
    void partition_in_groups(int label);        // split a cluster into groups
    Location first_group_step(int label) const; //
    Location other_groups_step(Location const& prev_waypoint) const;
    Location plot_nearby_waypoints(int cluster_label, int group_label, Location const& starting_waypoint) const;

  public:
    Area(double side, double transmission_range); // constructor

    int side() const { return sd; }

    void partition();
    void plot_waypoints();

    friend class MobilityModel; // needed to allow Clusters and Waypoints access

}; // class Area

} // namespace SMOOTH

#endif // AREA_HPP
