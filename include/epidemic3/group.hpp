#ifndef GROUP_HPP
#define GROUP_HPP

#include "configuration.h"
#include <vector>
#include "location.hpp"
#include <cassert>
#include <array>

namespace SMOOTH {


    class Group {
      private:
          int sz; //num of waypoints
          int lbl;
      public:
          Location* group_ptr;

          Group(int size,int label); //constructor
          Group();  //default contructor

          //set the iterator to the n-th element on the locations array
          void set_to_waypoint(std::array<Location,WAYPOINTS>& wpts, int n);

          int size() const { return sz; }

          int& size() { return sz; } //return a ref to size

          bool is_plotted(); //determines if this group's waypoint have been plotted on the map

    };

}



#endif //GROUP_HPP