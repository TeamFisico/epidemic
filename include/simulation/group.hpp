#ifndef GROUP_HPP
#define GROUP_HPP
#include "location.hpp"
#include <vector>

namespace smooth_sim
{
class Group
{
  private:
    std::vector<Location> Locations; // Vector of locations
    Position centre;                 // Centre of the Group
    Random grp_engine;

  public:
    // Constructor
    Group(int number_of_locations, Position group_centre, int cluster_label);
    // Default constructor
    Group() = default;

  private:
    // Plot the waypoints(locations) over the simulation area according to the followed statistical model
    void generate_group_waypoints(int locations_num, int cluster_label);

  public:
    // Returns centre position of this group
    Position get_centre() const;

    // Returns number of locations belonging to this group
    unsigned locations_num() const;

    // Returns a reference to the locations vector of this group
    std::vector<Location>& locations();
};

} // namespace smooth_sim

#endif
