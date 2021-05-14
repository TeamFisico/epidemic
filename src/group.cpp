#include "group.hpp"

using namespace sim;

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