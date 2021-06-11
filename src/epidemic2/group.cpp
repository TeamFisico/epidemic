#include "group.hpp"

using namespace sim;

Group::Group(int number_of_locations, Position center) // TODO add TRANSMISSION_RANGE as a macro
{
    locations.clear();
    // generate first locations
    locations.push_back(generate_close_loc(center, 0, TRANSMISSION_RANGE / 10));
    // generate other locations with a loop
    for (int i = 1; i < number_of_locations; ++i)
    {
        bool is_ok = false;
        while (!is_ok)
        {
            Location new_loc = generate_close_loc(center, 0, (i + 1) * TRANSMISSION_RANGE / 10);
            for (auto &a : locations)
            {
                if (a.get_pos().distance_to(new_loc.get_pos()) < TRANSMISSION_RANGE / 10)
                {
                    ; // make sure the new location close enough to other locations
                    is_ok = true;
                    break;
                }
            }
            if (is_ok) { locations.push_back(new_loc); }
        }
    }
}

Location *Group::get_location(int i)
{
    return &locations.operator[](i);
}
std::vector<Location *> Group::Location_list()
{
    std::vector<Location *> result;
    result.clear();
    for (unsigned int i = 0; i <= locations.size(); ++i)
    {
        result.push_back(&locations.operator[](i));
    }
    return result;
}