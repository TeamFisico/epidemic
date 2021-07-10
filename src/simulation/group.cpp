#include "group.hpp"

namespace smooth_sim
{

//////////////////////////////////////////////
///////        GROUP CONSTRUCTOR        //////
//////////////////////////////////////////////
Group::Group(int number_of_locations, Position group_centre, int cluster_label) : centre{group_centre}, grp_engine{}
{
    ///////// Group Waypoints plot over the simulation area  /////////

    generate_group_waypoints(number_of_locations, cluster_label);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////           PRIVATE METHODS           /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////// GENERATE GROUPS WAYPOINTS /////////////////
void Group::generate_group_waypoints(int locations_num, int cluster_label)
{
    // Generate first location according to location plotting model
    Locations.push_back(generate_close_loc(centre, 0, TRANSMISSION_RANGE / 10, cluster_label, grp_engine));

    // Now generating other group locations
    for (int i = 1; i < locations_num; ++i)
    {
        bool is_ok = false;
        while (!is_ok)
        {
            is_ok = true;
            Location new_loc =
                generate_close_loc(centre, 0, (i + 1) * TRANSMISSION_RANGE / 10, cluster_label, grp_engine);
            for (auto a : Locations)
            {
                // Make sure the new location close enough to other locations, if not generate a new one
                if (a.get_position().distance_to(new_loc.get_position()) < TRANSMISSION_RANGE / 10)
                {
                    is_ok = false;
                    break;
                }
            }
            if (is_ok) { Locations.push_back(new_loc); }
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////           PUBLIC METHODS            /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////// CENTRE POSITION OF THIS GROUP  /////////////////
Position Group::get_centre() const
{
    return centre;
}
///////////////// NUMBER OF LOCATIONS IN THIS GROUP /////////////////
unsigned Group::locations_num() const
{
    return Locations.size();
}
///////////////// REFERENCE TO LOCATIONS VECTOR OF THIS GROUP  /////////////////
std::vector<Location>& Group::locations()
{
    return Locations;
}

} // namespace smooth_sim