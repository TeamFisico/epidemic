#ifndef GROUP_HPP
#define GROUP_HPP
#include "location.hpp"
#include <vector>

namespace smooth_sim
{
class Group
{
  private:
    std::vector<Location> locations;
    Position center;
    Random grp_engine;

  public:
    Group(int number_of_locations, Position center, int cluster_index);
    Group() = default;
    // unused   Location* get_location(int i);          // get pointer to i_nth Location
    unsigned int locations_num(); // function that return pointer to all locations in the Group
    std::vector<Location>& Locations() { return locations; }
    unsigned int size() { return locations.size(); }
    Position get_center() { return center; }
};

} // namespace smooth_sim

#endif