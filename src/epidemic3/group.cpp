#include "group.hpp"
#include <cassert>

namespace SMOOTH{


    //constructor
    Group::Group(int size,int label)
          :sz{size},lbl{label}
    {
          assert(size > 0);
    }

    //default constructor
    Group::Group()
          :sz{0},lbl{0}
    {}

    void Group::set_to_waypoint(std::array<Location, WAYPOINTS> &wpts, int n)
    {
         assert(n<WAYPOINTS);
         group_ptr = &(wpts[n]);  //valuta se usare .at(n)
        assert(group_ptr);
    }
    //check group size waypoints to see if the group is done with plotting
    bool Group::is_plotted()
    {
        int i = 0;
        for (auto it = group_ptr; i < sz ;++it) {
            if (it->X() !=0 && it->Y() != 0)
            { //find an already set value
                return true;
            }
            ++i;
        }
        return false;
    }



}//namespace SMOOTH