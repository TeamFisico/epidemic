#include "mobility_model.hpp"
#include <cassert>
#include <random>
namespace SMOOTH
{

MobilityModel::MobilityModel(Person *person, Location *Waypoints_to_visit, int still_to_visit,
                             Location *target_location, double speed, double stay)
{
}

void MobilityModel::select_waypoints()
// select y% waypoints to put into *person target from the available from the cluster
// this function will fill Paths vector with indexes of waypoints referring to Waypoints array
{
    std::random_device rd;
    std::mt19937 gen(rd());

    int selected_size = static_cast<int>(person->cluster()->size() * y);
    // now determining the this cluster's indeces range inside Waypoints array

    int starting_index = 0;
    int ending_index = 0;

    if (person->cluster_label() > 0) // this is not the first clust
    {
        int i = 0;
        for (auto it = person->cluster(); i < person->cluster_label(); ++it)
        {
            assert(it); // it is actually pointing to a cluster
            starting_index += it->size();
            ++i;
        }
        ending_index = starting_index + person->cluster()->size();
    }
    else
    {
        ending_index = person->cluster()->size();
    }

    std::uniform_int_distribution<> rand(starting_index, ending_index);

    int random_index = 0;
    std::vector<int> already_chosen; // keep track of already chosen waypoints indeces

    assert(Paths.size() == 0);
    Paths.reserve(selected_size);
    already_chosen.reserve(selected_size);
    for (int i = 0; i < selected_size; ++i)
    {
        random_index = rand(gen);
        for (int i = 0; i < already_chosen.size(); ++i)
        {
            if (already_chosen[i] == random_index)
            {
                random_index = rand(gen); // try a new one
                i = -1;                   // restart then from 0
            }
        }
        already_chosen.push_back(random_index);
        Paths.push_back(random_index);
    }
}

} // namespace SMOOTH
