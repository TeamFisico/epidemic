#include "simulation.hpp"
#include <iostream>

namespace smooth_simulation
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////    WORLD CREATION  ///////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// initialising static data members
std::array<Location, WAYPOINTS_SIZE> Simulation::Waypoints;
std::array<Cluster, CLUSTERS_SIZE> Simulation::Clusters;
std::array<Person, POPULATION_SIZE> Simulation::People;
/////////////////////////////////////////////////////
////////          AREA PARTITIONING           ///////
/////////////////////////////////////////////////////
void Simulation::partition_in_clusters()
{
    int total_sizes = 0;
    double wpts_left = WAYPOINTS_SIZE;
    double still_to_choose = CLUSTERS_SIZE;
    int index = 0;

    Random rng{}; // seeded engine

    for (int i = 0; i < 100; ++i)
    {
        double mean_size = wpts_left / still_to_choose;
        // 68% will be in a 25% change range
        int current = rng.rounded_gauss(mean_size, mean_size / 4);

        assert(current >= 0.0);
        if (still_to_choose == 1) // the last will take the remaining size --> avoid too many more loops
        {
            Clusters[CLUSTERS_SIZE - 1].set_size((int)wpts_left);
            Clusters[CLUSTERS_SIZE - 1].set_label(CLUSTERS_SIZE - 1);
            Clusters[index].set_weight((double)wpts_left / WAYPOINTS_SIZE);
            return;
        }

        if (total_sizes + current < WAYPOINTS_SIZE)
        {
            Clusters[index].set_size(current);
            Clusters[index].set_label(index); // label the group with its index(from 0 to C-1)
            Clusters[index].set_weight((double)current /
                                       WAYPOINTS_SIZE); // set the weight to be be chosen by a random person
            total_sizes += current;                     // add this cluster's size to the counter
            --still_to_choose;
            wpts_left -= current;
            ++index;
        }
    }
}
/////////////////////////////////////////////////////
////////    1st WAYPOINT OF THE GROUP PLOT    ///////
/////////////////////////////////////////////////////
Location Simulation::first_group_step(int label) const
// Plot the 1st waypoint of the group so that it's not in the transmission range
// of any already plotted waypoint
{
    Random rng{}; // seeded engine

    Location try_waypoint{rng.uniform(0.0, side), rng.uniform(0.0, side)};

    if (label == 0) // this is the first group of the first cluster
    {
        return try_waypoint;
    }
    else
    { // if 1 <= label <= C-1
        while (true)
        {
            if (check_labeled_clusters(label, try_waypoint)) //no other wpts are under trans range
            {
                return try_waypoint;
            }
            else  //try with another one
            {
                try_waypoint = {rng.uniform(0.0, side), rng.uniform(0.0, side)};
            }
        }
    }
}

/////////////////////////////////////////////////////
////////     NEIGHBOURING WAYPOINTS PLOT      ///////
/////////////////////////////////////////////////////
//Plot the other waypoints of this group in such a way that their distance in never < TRANSMISSION_RANGE / 10
//the return value is the last plotted waypoint of this group
Location Simulation::plot_nearby_waypoints(int cluster_label, int group_label, Location const& starting_waypoint) const
{


    // return the last plotted waypoint of this group

}
/////////////////////////////////////////////////////
////////  1st WAYPOINT OF OTHER GROUPS PLOT   ///////
/////////////////////////////////////////////////////
// plot the first waypoint of the current group within a distance Y/4 <= d <= Y/3
Location Simulation::other_groups_step(Location const& prev_group_waypoint) const
{
    double const Y = 2 * side / CLUSTERS_SIZE;

    Random rng{};

    // generate waypoints in a square of side Y/3 (resize if out of bounds)
    double lower_x = prev_group_waypoint.X() - Y / 3;
    double upper_x = prev_group_waypoint.X() + Y / 3;
    double lower_y = prev_group_waypoint.Y() - Y / 3;
    double upper_y = prev_group_waypoint.Y() + Y / 3;

    if (lower_x < 0.0) lower_x = 0.0;
    if (lower_y < 0.0) lower_y = 0.0;
    if (upper_x > side) upper_x = side;
    if (upper_y > side) upper_y = side;

    while (true)
    {
        Location try_waypoint{rng.uniform(lower_x, upper_x), rng.uniform(lower_y, upper_y)};
        double distance = try_waypoint.distance(prev_group_waypoint);
        if (distance >= Y / 4 && distance <= Y / 3)
        {
            assert(try_waypoint.X() > 0.0 && try_waypoint.Y() > 0.0 && try_waypoint.X() <= side &&
                   try_waypoint.Y() <= side);
            return try_waypoint;
        }
    } // end while
}
/////////////////////////////////////////////////////
////////             FINAL PLOT              ////////
/////////////////////////////////////////////////////
void Simulation::plot_waypoints()
{
    for (int i = 0; i < CLUSTERS_SIZE; ++i)
    {
        // set the first waypoint associated with the first group
        Clusters[i].Groups[0].pointed_waypoint()[0] = first_group_step(i);
        // plot all the waypoints of the group around this one and return a reference to the last one
        Location previous_group_last_waypoint =
            plot_nearby_waypoints(i, 0, Clusters[i].Groups[0].pointed_waypoint()[0]);

        for (unsigned int j = 1; j < Clusters[i].Groups.size(); ++j) // now set the other groups' wpts
        {
            // setting the first waypoint of this group
            Clusters[i].Groups[j].pointed_waypoint()[0] = other_groups_step(previous_group_last_waypoint);
            // plot the neighbourhood and set the la
            previous_group_last_waypoint = plot_nearby_waypoints(i, j, Clusters[i].Groups[j].pointed_waypoint()[0]);
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////     SIMULATION     ///////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////
////////       SIMULATION CONSTRUCTOR         ///////
/////////////////////////////////////////////////////
Simulation::Simulation(double side, double spread_radius, double alpha, Data data)
    : side{side}, spread_radius{spread_radius}, alpha{alpha}, data{data}
{
    partition_in_clusters();
    for (Cluster& cluster : Clusters)
    {
        cluster.partition_in_groups();
    }
    plot_waypoints();
    std::cout << "World successfully constructed!" << std::endl;
}
/////////////////////////////////////////////////////
////////          CLUSTER ASSIGNMENT          ///////
/////////////////////////////////////////////////////
void Simulation::assign_to_cluster()
// assign a person to a cluster based on the cluster weight using piecewise-const-dist
{
    std::array<double, CLUSTERS_SIZE> weights; // cluster weights
    std::array<int, POPULATION_SIZE> labels;   // generated labels according to weights

    for (int i = 0; i < CLUSTERS_SIZE; ++i) // fill with clusters weight
    {
        weights[i] = Clusters[i].weight();
    }

    Random rng{}; // seeded random engine
    // fill labels array
    rng.engine().generate<std::discrete_distribution>(std::begin(labels), std::end(labels), std::begin(weights),
                                                      std::end(weights));
    // iterate over the labels array assigning the corresponding label to each person
    int j = 0;
    for (auto& label : labels)
    {
        People[j].set_cluster(label);
        Clusters[label].People_index.push_back(j); // add person's index into People array to the cluster
        ++j;
    }
}

/////////////////////////////////////////////////////
////////          HOME ASSIGNMENT             ///////
/////////////////////////////////////////////////////
void Simulation::assign_home()
// generate home address inside the relating cluster limits and generate groups of
// 1 to 5 people(families) which are gonna be assigned the same home location
{
    double lw_x = 0.0;
    double up_x = 0.0;
    double lw_y = 0.0;
    double up_y = 0.0;
    int family_size = 0;
    int people_left = POPULATION_SIZE;

    for (auto& person : People)
    {
        // set the bounds based on the belonging cluster for home generation
        int clust_index = person.home_cluster();
        lw_x = Clusters[clust_index].lower_x();
        up_x = Clusters[clust_index].upper_x();
        lw_y = Clusters[clust_index].lower_y();
        up_y = Clusters[clust_index].upper_y();

        Random rng{}; // create and seed the engine

        family_size = rng.int_uniform(1, 5);
        Location home{rng.uniform(lw_x, up_x), rng.uniform(lw_y, up_y)};

        if (people_left <= 5)
        {
            for (auto it2 = &person; it2 != std::end(People); ++it2)
            {
                it2->set_home(home);
            }
            return;
        }

        int i = 0; // take trace of the setted homes
        for (auto it2 = &person; i < family_size; ++it2)
        {
            it2->set_home(home);
            ++i;
        }
        people_left -= family_size;
    }
}
double weight_function(double distance, double LATP_parameter)
{
    return 1 / std::pow(distance, LATP_parameter);
}
// return true if no waypoints of this group and a trial location are under transmission range
// return false otherwise
bool check_group(Group const& group, Location try_location)
{
    for (int i = 0; i < group.size(); ++i)
    {
        if (group.pointed_waypoint()[i].within_transmission_range(try_location)) { return false; }
    }
    return true;
}

bool check_cluster(Cluster const& cluster, Location try_location)
{
    for (auto const& group : cluster.Groups)
    {
        if (!check_group(group, try_location)) return false;
    }
    return true;
}
// check up to Clusters[label-1] returning true if all the clusters are okay, false other wise
bool check_labeled_clusters(int label, Location try_location)
{
    for (int i = 0; i < label; ++i)
    {
        if (!check_cluster(Simulation::Clusters[i], try_location)) return false;
    }
    return true;
}

} // namespace SMOOTH
