
#include "cluster.hpp"
#include "simulation.hpp"
#include <algorithm>
#include <cmath>
#include <random>

namespace smooth_simulation
{
/////////////////////////////////////////////////////
////////          CLUSTER CONSTRUCTOR         ///////
/////////////////////////////////////////////////////
Cluster::Cluster(int size, int label, double weight, Zone zone, double alpha, double x_low, double x_up, double y_low,
                 double y_up, Data cluster_data)
    : sz{size}, lbl{label}, w{weight}, zone{zone}, alpha{alpha}, data{cluster_data},limits{x_low,x_up,y_low,y_up}
{
}
/////////////////////////////////////////////////////
////////           DEFAULT CLUSTER            ///////
/////////////////////////////////////////////////////
const Cluster& default_cluster()
{
    static Data dd{0, 0, 0, 0, 0, 0};
    static Cluster def_cl{0, 0, 0.0, Zone::White, WHITE_LATP_PARAMETER, 0.0, 0.0, 0.0, 0.0, dd};
    return def_cl;
}
/////////////////////////////////////////////////////
////////    DEFAULT CLUSTER CONSTRUCTOR       ///////
/////////////////////////////////////////////////////
Cluster::Cluster()
    : sz{default_cluster().sz}, lbl{default_cluster().lbl}, w{default_cluster().w}, zone{default_cluster().zone},
      data{default_cluster().data}, limits{default_cluster().limits[0], default_cluster().limits[1],
                                           default_cluster().limits[2], default_cluster().limits[3]}
{
}
/////////////////////////////////////////////////////
////////      GROUP SIZES DETERMINATION       ///////
/////////////////////////////////////////////////////
void Cluster::generate_groups(Random& engine)
{
    int const max_size = MAXIMUM_GROUP_PROBABILITY * sz;
    int all_groups_size = 0;
    int group_index = 0;

    assert(Groups.size() == 0);

    // TODO consider reserving space for cluster.Groups
    int current;
    for (int i = 0; i < sz; ++i)
    {
        current = engine.int_uniform(1, max_size); // pick a rand size

        if (all_groups_size + current == sz)
        {
            Group g{current};
            Groups.push_back(g);
            break;
        }
        if (all_groups_size + current < sz)
        {
            Group g{current};
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
void Cluster::partition_in_groups(Random& engine)
{
    auto& cluster_ref = Simulation::Clusters[lbl]; // reference to this cluster in Clusters array

    cluster_ref.generate_groups(engine); // filling vector<Groups>

    int already_setted_waypoints = 0;
    for (int i = 0; i < lbl; ++i)
    {
        already_setted_waypoints += Simulation::Clusters[i].size();
    }

    // set groups pointers to their index in the vector containing all waypoints(Locations)
    for (auto& group : cluster_ref.Groups)
    {
        group.set_to_waypoint(Simulation::Waypoints, already_setted_waypoints);
        already_setted_waypoints += group.size();
    }
    // TODO Do some kind of check!!
}
/////////////////////////////////////////////////////
////////   CLUSTER X,Y LIMITS DETERMINATION   ///////
/////////////////////////////////////////////////////
// Look into waypoints of this cluster and determine the limits
void Cluster::set_limits()
{
    double x_min = Groups[0].pointed_waypoint()[0].get_X();
    double y_min = Groups[0].pointed_waypoint()[0].get_Y();
    double x_max = Groups[0].pointed_waypoint()[0].get_X();
    double y_max = Groups[0].pointed_waypoint()[0].get_Y();

    for (auto& group : Groups)
    {
        int i = 0;
        for (auto it = group.pointed_waypoint(); i < group.size(); ++it)
        {
            if (it->get_X() < x_min) x_min = it->get_X();
            if (it->get_X() > x_max) x_max = it->get_X();
            if (it->get_Y() < y_min) y_min = it->get_Y();
            if (it->get_Y() > y_max) y_max = it->get_Y();
            ++i;
        }
    }

    limits = {x_min, x_max, y_min, y_max};
}
/////////////////////////////////////////////////////
////////      MOVE PEOPLE IN THIS CLUSTER     ///////
/////////////////////////////////////////////////////
void Cluster::move()
{
    for (auto& i : People_i)
    {
        Person& p = Simulation::People[i]; // current person
        p.move();
    }
}
/////////////////////////////////////////////////////
/////    REMOVE DEAD PEOPLE FROM THIS CLUSTER   /////
/////////////////////////////////////////////////////
void Cluster::clear_dead_people()
{
    for (int& person_i : People_i)
    {
        Status const& stat = Simulation::People[person_i].current_status();
        if (stat == Status::Dead)
        {
            std::swap(person_i, People_i.back()); // swap the last element with the one to be removed
            People_i.pop_back();                  // erase the last element of the vector
        }
    }
}
/////////////////////////////////////////////////////
/////    UPDATE EPIDEMICDATA FOR THIS CLUSTER   /////
/////////////////////////////////////////////////////
void Cluster::update_data()
{
    for (auto& person_i : People_i)
    {
        Status const& status = Simulation::People[person_i].current_status();
        switch (status)
        {
        case Status::Susceptible:
            ++data.S;
            break;
        case Status::Exposed:
            ++data.E;
            break;
        case Status::Infected:
            ++data.I;
            break;
        case Status::Recovered:
            ++data.R;
            break;
        case Status::Dead:
            ++data.D;
            break;
        }
    }
}
/////////////////////////////////////////////////////
////////        DATA CONSTRUCTOR              ///////
/////////////////////////////////////////////////////
Data::Data(unsigned int susceptible, unsigned int latent, unsigned int infected, unsigned int recovered,
           unsigned int dead, unsigned int capacity)
    : S{susceptible}, E{latent}, I{infected}, R{recovered}, D{dead}, ICU_capacity{capacity}
{
}

// returns a vector with white clusters labels except the one taken as argument
std::vector<int> available_white_clusters(int lbl)
{
    std::vector<int> labels;
    for (auto& cl : Simulation::Clusters)
    {
        if (cl.zone_type() == Zone::White && cl.label() != lbl) labels.push_back(cl.label());
    }
    return labels;
}

} // namespace smooth_simulation
