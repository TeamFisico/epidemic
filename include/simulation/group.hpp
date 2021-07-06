#ifndef GROUP_HPP
#define GROUP_HPP
#include "location.hpp"
#include <vector>

namespace smooth_sim
{
class Group
{
  private:
    std::vector<Location> Locations;
    Position center;
    Random grp_engine;

  public:
    Group(int number_of_locations, Position center, int cluster_index);
    Group() = default;

    // Return the total number of location in the group
    unsigned int locations_num();

    // Return reference to Locations vector
    std::vector<Location>& locations() { return Locations; }

    // Return the center of the Group
    Position get_center() { return center; }
};

} // namespace smooth_sim

#endif