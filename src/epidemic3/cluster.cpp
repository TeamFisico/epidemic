
#include "cluster.hpp"
#include <algorithm>
#include <math.h>
#include <random>

namespace SMOOTH
{

Cluster::Cluster(int size) : sz{size}
{
    //        partition_in_groups();
}
// default constructor
Cluster::Cluster() : sz{0}, lbl{0}, w{0.0}, zone{Zone::white}
{
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

    int current = 0;
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
////////   CLUSTER X,Y LIMITS DETERMINATION   ///////
/////////////////////////////////////////////////////
void Cluster::determine_limits()
{
    double x_min = Groups[0].group_ptr[0].X();
    double y_min = Groups[0].group_ptr[0].Y();
    double x_max = Groups[0].group_ptr[0].X();
    double y_max = Groups[0].group_ptr[0].Y();

    for (auto& group : Groups)
    {
        int i = 0;
        for (auto it = group.group_ptr; i< group.size();++it)
        {
            if (it->X() < x_min) x_min = it->X();
            if (it->X() > x_max) x_max = it->X();
            if (it->Y() < y_min) y_min = it->Y();
            if (it->Y() > y_max) y_max = it->Y();
            ++i;
        }
    }

    limits = {x_min,x_max,y_min,y_max};
}

} // namespace SMOOTH











