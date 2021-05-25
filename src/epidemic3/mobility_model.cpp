#include "mobility_model.hpp"
#include <cassert>
#include <random>
namespace SMOOTH
{

MobilityModel::MobilityModel(Person *person, Location *Waypoints_to_visit, int still_to_visit,
                             Location *target_location, double speed, double stay)
{
}
/////////////////////////////////////////////////////
////////            PATH ASSIGNMENT           ///////
/////////////////////////////////////////////////////
// select y% waypoints to put into Path among the available from the belonging cluster
void MobilityModel::select_waypoints()
{

    int belonging_cluster_size = Simulation::world.Clusters[person->cluster_label()].size();

    std::random_device rd;
    std::mt19937 gen(rd());

    int selected_size = static_cast<int>(belonging_cluster_size * y);
    // now determining the this cluster's indeces range inside Waypoints array

    int starting_index = 0;
    int ending_index = 0;

    if (person->cluster_label() > 0) // this is not the first clust
    {
        for (int i = 0; i < person->cluster_label(); ++i) // check the previous clusters
        {
            starting_index += Simulation::world.Clusters[i].size();
            ++i;
        }
        ending_index = starting_index + belonging_cluster_size;
    }
    else
    {
        ending_index = belonging_cluster_size;
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
/////////////////////////////////////////////////////
////////      TARGET ASSIGNMENT : LATP        ///////
/////////////////////////////////////////////////////
void MobilityModel::set_new_target()
// use the Least action trip planning(LATP) algorithm to determine new person target
{
}

} // namespace SMOOTH
