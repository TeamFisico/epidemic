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

    int selected_size = static_cast<int>(belonging_cluster_size * Simulation::y);
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

    assert(person->Paths.size() == 0);
    person->Paths.reserve(selected_size);
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
        person->Paths.push_back(random_index);
    }
}
/////////////////////////////////////////////////////
////////       TARGET ASSIGNMENT : LATP       ///////
/////////////////////////////////////////////////////
void MobilityModel::choose_target()
// use the Least action trip planning(LATP) algorithm to determine new person target
{
    // calculate distance between all the possibile paths
    // then calculate the probability to visit each waypoint
    // through the weight function and finally extract the next
    // target with the previous probabilities assigned to each wpt
    Location person_location = person->get_location();

    std::vector<double> distances;
    std::vector<double> probabilities;
    distances.reserve(person->Paths.size());     // allocate the space
    probabilities.reserve(person->Paths.size()); // allocate the space
    double current_weight = 0.0;
    // compute distances
    for (int &index : person->Paths)
    {
        Location waypoint = Simulation::world.Waypoints[index];
        current_weight = compute_weight(person_location.get_distance(waypoint));
        distances.push_back(current_weight);
    }
    double sum = std::accumulate(std::begin(distances), std::end(distances), 0.0);

    for (double curr_weight : distances)
    {
        probabilities.push_back(curr_weight / sum);
    }
    // ok now knowing all the probabilities for a person to target an indexed waypoint
    // extract an index based on this probabilities

    std::random_device rd;
    std::mt19937 gen(rd());
    // generate one index based on the previously determinated probabilities(weights)
    std::piecewise_constant_distribution<int> rand(std::begin(person->Paths), std::begin(person->Paths),
                                                   probabilities.begin());

    person->set_target(Simulation::world.Waypoints[rand(gen)]);
    //TODO TESTING
}
double MobilityModel::compute_weight(double distance)
{
    return 1 / std::pow(distance, alpha);
}
/////////////////////////////////////////////////////
////////    MOVE A PERSON TOWARD A TARGET     ///////
/////////////////////////////////////////////////////
void MobilityModel::move_toward_target(Location *target)
{

}

} // namespace SMOOTH













