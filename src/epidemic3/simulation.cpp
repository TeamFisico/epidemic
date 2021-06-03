#include "simulation.hpp"
#include <random>
namespace SMOOTH
{

/////////////////////////////////////////////////////
////////          CLUSTER ASSIGNMENT          ///////
/////////////////////////////////////////////////////
void Simulation::assign_to_cluster()
// assign a person to a cluster based on the cluster weight using piecewise-const-dist
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<double> weights(clusters_size); // weights

    for (int i = 0; i < clusters_size; ++i)
    {
        weights[i] = world.Clusters[i].weight();
    }

    std::discrete_distribution<int> dis(std::begin(weights), std::end(weights));

    for (auto& person : People)
    {
        int index = dis(gen);
        person.set_cluster(index);
    }
}

/////////////////////////////////////////////////////
////////          HOME ASSIGNMENT             ///////
/////////////////////////////////////////////////////
void Simulation::assign_home(int label)
// generate home address inside the relating cluster limits and generate groups of
// 1 to 5 people(families) which are gonna be assigned the same home location
{
    std::random_device rd1;
    std::mt19937 gen1(rd1());
    std::uniform_int_distribution<> family(1, 5);

    std::random_device rd2;
    std::mt19937 gen2(rd2());

    int people_left = population_size;
    int current_family = 0;
    double lw_x = 0.0;
    double up_x = 0.0;
    double lw_y = 0.0;
    double up_y = 0.0;

    // iterate over People array
    for (auto it1 = std::begin(People); it1 != std::end(People); ++it1)
    {
        // set the bounds based on the belonging cluster for home generation
        lw_x = world.Clusters[it1->cluster_label()].lower_x();
        up_x = world.Clusters[it1->cluster_label()].upper_x();
        lw_y = world.Clusters[it1->cluster_label()].lower_y();
        up_y = world.Clusters[it1->cluster_label()].upper_y();

        std::uniform_real_distribution<double> x_home(lw_x, up_x);
        std::uniform_real_distribution<double> y_home(lw_y, up_y);

        current_family = family(gen1);
        Location home{x_home(gen2), y_home(gen2)};

        if (people_left <= 5)
        {
            for (auto it2 = it1; it2 != std::end(People); ++it2)
            {
                it2->set_home(home);
            }
            return;
        }

        int i = 0;
        for (auto it2 = it1; i < current_family; ++it2)
        {
            it2->set_home(home);
            people_left -= current_family;
            ++i;
        }
    }
}
/////////////////////////////////////////////////////
////////            PATH ASSIGNMENT           ///////
/////////////////////////////////////////////////////
void Simulation::select_waypoints(Person* person)
{

    int belonging_cluster_size = world.Clusters[person->cluster_label()].size();

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
            starting_index += world.Clusters[i].size();
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
////////            TARGET UPDATE             ///////
/////////////////////////////////////////////////////
void Simulation::update_target(Person* person)
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
    for (int& index : person->Paths)
    {
        Location waypoint = Simulation::world.Waypoints[index];
        current_weight = weight_function(person_location.get_distance(waypoint), alpha);
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
    // TODO TESTING
}

double weight_function(double distance, double LATP_parameter)
{
    return 1 / std::pow(distance, LATP_parameter);
}

} // namespace SMOOTH
