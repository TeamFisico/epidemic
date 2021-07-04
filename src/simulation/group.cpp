#include "group.hpp"

namespace smooth_sim
{

Group::Group(int number_of_locations, Position center, int cluster_index)
    : grp_engine{}
{
    locations.clear();
    // generate first locations
    locations.push_back(generate_close_loc(center, 0, TRANSMISSION_RANGE / 10, cluster_index, grp_engine));
    // generate other locations with a loop
    for (int i = 1; i < number_of_locations; ++i)
    {
        bool is_ok = false;
        while (!is_ok)
        {
            is_ok = true;
            Location new_loc =
                generate_close_loc(center, 0, (i + 1) * TRANSMISSION_RANGE / 10, cluster_index, grp_engine);
            for (auto a : locations)
            {
                if (a.get_pos().distance_to(new_loc.get_pos()) < TRANSMISSION_RANGE / 10)
                {
                    ; // make sure the new location close enough to other locations
                    is_ok = false;
                    break;
                }
            }
            if (is_ok) { locations.push_back(new_loc); }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////           PRIVATE METHODS           /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////           PUBLIC METHODS            /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int Group::locations_num()
{
    return locations.size();
}
// unused
// Location *Group::get_location(int i)
//{
//    return &locations[i];
//}
} // namespace smooth_sim