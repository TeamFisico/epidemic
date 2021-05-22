
#include "area.hpp"
#include "configuration.h"
#include <cassert>
#include <iostream>
#include <random>

namespace SMOOTH
{

// constructor
Area::Area(double side, double transmission_range) : sd{side}, R{transmission_range}
{
    assert(sd > 0 && R > 0);
}

//////////////// AREA PARTITIONING ALGORITHM /////////////////
void Area::partition_in_clusters()
{
    int total_sizes = 0;
    double wpts_left = waypoints_size;
    double still_to_choose = clusters_size;
    int index = 0;

    std::random_device rd;  // set the seed
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()

    for (int i = 0; i < 100; ++i)
    {

        double mean_size = wpts_left / still_to_choose;
        // 68% will be in a 25% change range
        std::normal_distribution rand(mean_size, mean_size / 4);
        int current = nearbyint(rand(gen));

        assert(current >= 0.0);
        if (still_to_choose == 1) // the last will take the remaining size --> avoid too many more loops
        {
            Clusters[clusters_size - 1].size() = (int)wpts_left;
            Clusters[clusters_size - 1].set_label(clusters_size - 1);
            Clusters[index].set_weight((double)wpts_left / waypoints_size);
            return;
        }

        if (total_sizes + current < waypoints_size)
        {
            Clusters[index].size() = current;
            Clusters[index].set_label(index); // label the group with its index(from 0 to C-1)
            Clusters[index].set_weight((double)current /
                                       waypoints_size); // set the weight to be be chosen by a random person
            total_sizes += current;                     // add this cluster's size to the counter
            --still_to_choose;
            wpts_left -= current;
            ++index;
        }
    }
}

void Area::partition_in_groups(int label)
{

    Clusters[label].determine_groups_sizes(); // filling vector<Groups>

    int already_setted_waypoints = 0;
    for (int i = 0; i < label; ++i)
    {
        already_setted_waypoints += Clusters[i].size();
    }

    int j = 0;
    std::cout << "Cluster[" << label << "]" << std::endl;

    // set groups pointers to their index in the vector containing all waypoints(Locations)
    for (auto &group : Clusters[label].Groups)
    {
        group.set_to_waypoint(Waypoints, already_setted_waypoints);
        already_setted_waypoints += group.size();
        ++j;
    }

    // TODO check with an assert absolutely!!!
}

void Area::partition()
{
    partition_in_clusters();
    for (int i = 0; i < clusters_size; ++i)
    {
        partition_in_groups(i);
    }
    std::cout << "Area::partition() complete\n";
}

// Input: label of the current cluster whose waypoints are to be plot
// return: reference to the plotted waypoint
Location Area::first_group_step(int label)
{
    /////////////////// PART 1 ////////////////////
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> rand(0.0, sd); // generate the waypoint coordinate in [0,size]

    double X = rand(gen);
    double Y = rand(gen);

    Location try_waypoint{X, Y};

    if (label == 0) // this is the first group of the first cluster
    {
        return try_waypoint;
    }
    else
    { // if 1 <= label <= C-1
        bool end_loop = false;
        bool change_wpt = false;

        int n_loops = 0;
        while (!end_loop)
        {
            int check_up_to = label; // check the other waypoints in the previous clusters

            for (int curr_cluster_index = 0; curr_cluster_index < check_up_to;
                 ++curr_cluster_index) // loop over Clusters
            {
                if (change_wpt)
                    break;
                for (auto &current_group : Clusters[curr_cluster_index].Groups) // loop over Groups
                {
                    if (change_wpt)
                        break;
                    int it_index = 0;
                    for (auto it = current_group.group_ptr; it_index < current_group.size();
                         ++it) // loop over group Waypoints
                    {

                        if (it->get_distance(try_waypoint) <= R)
                        {
                            change_wpt = true;
                            break;
                        }

                        if (it_index == current_group.size() - 1 && curr_cluster_index == check_up_to - 1)
                        {
                            end_loop = true;
                        }
                        ++it_index; // next Location
                    }               // end loop waypoints
                }                   // end loop Groups
            }                       // end loop Clusters

            if (!end_loop)
            {
                // retry with new location
                try_waypoint.X() = rand(gen);
                try_waypoint.Y() = rand(gen);
            }
        } // end while
        assert(try_waypoint.X() > 0.0 && try_waypoint.Y() > 0.0 && try_waypoint.X() <= sd && try_waypoint.X() <= sd);
        return try_waypoint;
    } // else
}

// set the nearby waypoints and return
Location Area::plot_nearby_waypoints(int cluster_label, int group_label, Location const &starting_waypoint)
{
    /////////////////// PART 2 ////////////////////
    // plot within 0.1 R from the other waypoint of this group
    // we do that by extracting the next waypoint from a gaussian distribution
    // with mean on the previous waypoint with a stddev of 0.1R/3 so that the 99.7%
    // will be in the 0.1R range

    std::random_device gs; // set the seed for gaussian generation
    std::mt19937 gen_gaus(gs());

    for (int i = 1; i < Clusters[cluster_label].Groups[group_label].size(); ++i)
    {
        std::normal_distribution<double> gaus1(starting_waypoint.X(), 0.1 * R / 3); // generate x of next waypoint
        std::normal_distribution<double> gaus2(starting_waypoint.Y(), 0.1 * R / 3); // generate y of next waypoint

        double current_waypoint_x = gaus1(gen_gaus);
        double current_waypoint_y = gaus2(gen_gaus);

        // handle cases with a possible negative extraction
        if (current_waypoint_x < 0.0)
            current_waypoint_x = 0.0;
        if (current_waypoint_y < 0.0)
            current_waypoint_y = 0.0;

        Location current_waypoint{current_waypoint_x, current_waypoint_y};

        Clusters[cluster_label].Groups[group_label].group_ptr[i] = current_waypoint;

    } // end for
    return Clusters[cluster_label]
        .Groups[group_label]
        .group_ptr[Clusters[cluster_label].Groups[group_label].size() - 1];
}

// algorithm applied from the second group of this cluster to the last
// plot
Location Area::other_groups_step(Location const &prev_group_waypoint) const
{
    double const Y = 2 * sd / clusters_size;

    std::random_device rd; // set the seed for gaussian generation
    std::mt19937 gen(rd());

    // generate waypoints in a square of side Y/3(if possible)
    double lower_x = prev_group_waypoint.X() - Y / 3;
    double upper_x = prev_group_waypoint.X() + Y / 3;
    double lower_y = prev_group_waypoint.Y() - Y / 3;
    double upper_y = prev_group_waypoint.Y() + Y / 3;

    if (lower_x < 0.0)
        lower_x = 0.0;
    if (lower_y < 0.0)
        lower_y = 0.0;
    if (upper_x > sd)
        upper_x = sd;
    if (upper_y > sd)
        upper_y = sd;

    std::uniform_real_distribution<> rand_x(lower_x, upper_x);
    std::uniform_real_distribution<> rand_y(lower_y, upper_y);

    int i = 1;
    while (1 > 0)
    {
        Location try_waypoint{rand_x(gen), rand_y(gen)};
        double distance = try_waypoint.get_distance(prev_group_waypoint);
        if (distance >= Y / 4 && distance <= Y / 3)
        {
            //                std::cout <<"location found in "<<i<<" loops\n";
            assert(try_waypoint.X() > 0.0 && try_waypoint.Y() > 0.0 && try_waypoint.X() <= sd &&
                   try_waypoint.Y() <= sd);
            return try_waypoint;
        }
        ++i;
    } // end while
}

void Area::plot_waypoints()
{
    for (int i = 0; i < clusters_size; ++i)
    {
        // set the first waypoint associated with the first group
        Clusters[i].Groups[0].group_ptr[0] = first_group_step(i); // TODO CAPIRE PERCHE C'E LOOP INFINITO QUI
        // plot all the waypoints of the group around this one and return a reference to the last one
        Location previous_group_last_waypoint = plot_nearby_waypoints(i, 0, Clusters[i].Groups[0].group_ptr[0]);

        for (unsigned int j = 1; j < Clusters[i].Groups.size(); ++j) // now set the other groups' wpts
        {
            // setting the first waypoint of this group
            Clusters[i].Groups[j].group_ptr[0] = other_groups_step(previous_group_last_waypoint);
            // plot the neighbourhood and set the la
            previous_group_last_waypoint = plot_nearby_waypoints(i, j, Clusters[i].Groups[j].group_ptr[0]);
        }
    }
}

} // namespace SMOOTH
