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
    Position centre;
    Random grp_engine;

  public:
    Group(int number_of_locations, Position group_centre, int cluster_label);
    Group() = default;

  private:
    // plot the waypoints(locations) over the simulation area according to the followed statistical model
    void generate_group_waypoints(int locations_num, int cluster_label);

  public:
    // returns centre position of this group
    Position get_centre() const;

    // returns number of locations belonging to this group
    unsigned locations_num() const;

    // returns a reference to the locations vector of this group
    std::vector<Location>& locations();
};

} // namespace smooth_sim

#endif

// unused   Location* get_location(int i);          // get pointer to i_nth Location
