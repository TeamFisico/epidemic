#ifndef GROUP_HPP
#define GROUP_HPP

#include "location.hpp"
#include "parameters.hpp"
#include <array>
#include <cassert>

namespace smooth_simulation
{

class Group
{
  private:
    int sz; // num of waypoints
    Location* group_ptr;

  public:
    explicit Group(int size); // constructor
    Group();                    // default contructor
    Location* pointed_waypoint() const { return group_ptr; }

    // set the iterator to the n-th element on the locations array
    void set_to_waypoint(std::array<Location, WAYPOINTS>& wpts, int n);

    int size() const { return sz; }
    int& size() { return sz; } // return a ref to size
};

} // namespace smooth_simulation

#endif // GROUP_HPP