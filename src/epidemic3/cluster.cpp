
#include "cluster.hpp"
#include "simulation.hpp"
#include <algorithm>
#include <math.h>
#include <random>

namespace SMOOTH
{
/////////////////////////////////////////////////////
////////          CLUSTER CONSTRUCTOR         ///////
/////////////////////////////////////////////////////
Cluster::Cluster(int size, int label, double weight, Zone zone/*, Data data*/, double x_low, double x_up, double y_low, double y_up)
        :sz{size},lbl{label},w{weight},zone{zone}/*,data{data}*/
{
    limits[0] = x_low;
    limits[1] = x_up;
    limits[2] = y_low;
    limits[3] = y_up;
}
/////////////////////////////////////////////////////
////////           DEFAULT CLUSTER            ///////
/////////////////////////////////////////////////////
const Cluster& default_cluster()
{
//    static Data dd {}
    static Cluster def_cl{0,0,0.0,Zone::White,0.0,0.0,0.0,0.0};
    return def_cl;
}
/////////////////////////////////////////////////////
////////    DEFAULT CLUSTER CONSTRUCTOR       ///////
/////////////////////////////////////////////////////
Cluster::Cluster() :sz{default_cluster().sz},lbl{default_cluster().lbl},w{default_cluster().w},zone{default_cluster().zone}
{
    limits[0] = default_cluster().limits[0];
    limits[1] = default_cluster().limits[1];
    limits[2] = default_cluster().limits[2];
    limits[3] = default_cluster().limits[3];
}
/////////////////////////////////////////////////////
////////      GROUP SIZES DETERMINATION       ///////
/////////////////////////////////////////////////////
void Cluster::determine_groups_sizes()
{
    const double maximum_group_size = sz / 2; // TODO the groups can be as big as 50% of the cluster size(makesense???)
    int all_groups_size = 0;
    int group_index = 0;

    std::random_device rd; // set the seed
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> rand(1, nearbyint(maximum_group_size));

    assert(Groups.size() == 0);

    Groups.reserve(sz / 2); // allocate space for half of cluster size-->considering the uniform distribution
                            // should avoid new allocation in the majority of the cases

    int current;
    for (int i = 0; i < sz; ++i)
    {

        current = rand(gen); // pick a rand size

        if (all_groups_size + current == sz)
        {
            Group g{current, group_index};
            Groups.push_back(g);
            break;
        }
        if (all_groups_size + current < sz)
        {
            Group g{current, group_index};
            Groups.push_back(g);
            ++group_index;
            all_groups_size += current;
        }

    } // end for

    // Fix the size if 1 waypoint is left out either from the 1st of from last group
    int total_size = 0;
    for (unsigned long int j = 0; j < Groups.size(); ++j)
    {
        total_size += Groups[j].size();
    }
    int difference = sz - total_size;
    Groups[Groups.size() - 1].size() += difference;
}
/////////////////////////////////////////////////////
////////        CLUSTER PARTITIONING          ///////
/////////////////////////////////////////////////////
void Cluster::partition_in_groups()
{
    Simulation::Clusters[lbl].determine_groups_sizes(); // filling vector<Groups>

    int already_setted_waypoints = 0;
    for (int i = 0; i < lbl; ++i)
    {
        already_setted_waypoints += Simulation::Clusters[i].size();
    }

    // set groups pointers to their index in the vector containing all waypoints(Locations)
    for (auto& group : Simulation::Clusters[lbl].Groups)
    {
        group.set_to_waypoint(Simulation::Waypoints, already_setted_waypoints);
        already_setted_waypoints += group.size();
    }

    // TODO Do some kind of check!!
}
/////////////////////////////////////////////////////
////////   CLUSTER X,Y LIMITS DETERMINATION   ///////
/////////////////////////////////////////////////////
//Look into waypoints of this cluster and determine the limits
void Cluster::set_limits()
{
    double x_min = Groups[0].pointed_waypoint()[0].X();
    double y_min = Groups[0].pointed_waypoint()[0].Y();
    double x_max = Groups[0].pointed_waypoint()[0].X();
    double y_max = Groups[0].pointed_waypoint()[0].Y();

    for (auto& group : Groups)
    {
        int i = 0;
        for (auto it = group.pointed_waypoint(); i < group.size(); ++it)
        {
            if (it->X() < x_min) x_min = it->X();
            if (it->X() > x_max) x_max = it->X();
            if (it->Y() < y_min) y_min = it->Y();
            if (it->Y() > y_max) y_max = it->Y();
            ++i;
        }
    }

    limits = {x_min, x_max, y_min, y_max};
}
/////////////////////////////////////////////////////
////////         GET CLUSTER DATA             ///////
/////////////////////////////////////////////////////
Data Cluster::get_data() const
{
}
/////////////////////////////////////////////////////
////////        DATA CONSTRUCTOR              ///////
/////////////////////////////////////////////////////
Data::Data(int susceptible, int latent, int infected, int recovered, int dead)
      :S{susceptible},E{latent},I{infected},R{recovered},D{dead}
{
}

} // namespace SMOOTH
