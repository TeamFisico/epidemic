#include "cluster.hpp"
#include "parameters.hpp"
#include "random.hpp"
#include <cassert>
#include <random>

namespace smooth_sim
{

//////////////////////////////////////////////
///////      CLUSTER CONSTRUCTOR        //////
//////////////////////////////////////////////
Cluster::Cluster(unsigned S, unsigned E, unsigned I, unsigned R, int number_of_location, Rectangle Area,
                 Zone cluster_zone, int cluster_label, double cluster_LATP_parameter)
    : Area{Area},
      zone{cluster_zone},
      label{cluster_label},
      LATP_alpha{cluster_LATP_parameter},
      cl_engine{}
{
    int N = S + E + I + R;
    people.clear();
    people.reserve(N); // reserve space for the people vector
    // generate the people vector; initialize all person as susceptible and at home
    int index = 0;
    while (index < N)
    {
        int home_pop = cl_engine.int_uniform(1, 5); // number of people in the current home
        Location current_home =
            rand_loc(Area.get_blh_corner(), Area.get_trh_corner(), HOME_RADIUS, cluster_label, cl_engine);
        for (int j = 0; j < home_pop && index < N; ++j)
        {
            Person curr{Status::Susceptible, current_home.get_pos(), Status::Susceptible, current_home, cluster_label};
            people.emplace_back(curr, 0, HOME_PROBABILITY, true);
            people[index].recall_home(); // set target location as person's home
            ++index;
        }
    }
    // change person's condition and next condition to have the right number of Statuss
    for (unsigned i = 0; i < E + I + R; ++i)
    {
        if (i < E)
        {
            people[i].person_ref().set_current_status(Status::Exposed);
            people[i].person_ref().set_new_status(Status::Exposed);
        }
        else if (i < E + I)
        {
            people[i].person_ref().set_current_status(Status::Infected);
            people[i].person_ref().set_new_status(Status::Infected);
        }
        else if (i < E + I + R)
        {
            people[i].person_ref().set_current_status(Status::Recovered);
            people[i].person_ref().set_new_status(Status::Recovered);
        }
    }
    // Determine the number of groups
    int number_of_groups = cl_engine.rounded_gauss(number_of_location / 8, number_of_location / 32);
    // Create a Vector which will have the partition of locations in group, every group has at least one location
    std::vector<int> loc_num(number_of_groups, 1);
    // std::uniform_int_distribution<int> rand_index(0, number_of_groups - 1);
    int loc_left = number_of_location - number_of_groups;
    for (int i = 0; i < number_of_groups; ++i)
    {
        if (loc_left == 0)
        {
            i = number_of_groups; // end the loop
        }
        else if (loc_left == 1)
        {
            ++loc_num[i];
            i = number_of_groups; // end the loop
        }
        else
        {
            std::uniform_int_distribution<> rand_num(1, nearbyint(loc_left / 2));
            int rnum = cl_engine.int_uniform(1, nearbyint(loc_left / 2));
            loc_num[i] += rnum;
            loc_left -= rnum;
        }
    }

    groups.clear();
    groups.reserve(number_of_groups);
    // fill the group vector
    for (int i = 0; i < number_of_groups; ++i)
    {
        groups.emplace_back(loc_num[i], gen_group_center(loc_num[i]), label);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////           PRIVATE METHODS           /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////           PUBLIC METHODS            /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double Cluster::base()
{
    return Area.get_trh_corner().get_x() - Area.get_blh_corner().get_x();
}
double Cluster::height()
{
    return Area.get_trh_corner().get_y() - Area.get_blh_corner().get_y();
}
unsigned Cluster::locations_num()
{
    auto add_op = [&](unsigned int a, Group b) { return a + b.size(); };
    return std::accumulate(std::begin(groups), std::end(groups), 0, add_op);
}

unsigned Cluster::people_num()
{
    return people.size();
}

Position Cluster::gen_group_center(int num_of_loc)
{
    bool end_loop = false;
    Position new_center{};
    while (!end_loop)
    {
        end_loop = true;
        new_center =
            rand_pos(Area.get_blh_corner(), Area.get_trh_corner(), cl_engine); // generate random center position
        for (auto& a : groups)
        { // check if this center is far enough from other groups center
            if (a.get_center().distance_to(new_center) <= (a.size() + num_of_loc) * TRANSMISSION_RANGE / 10)
            {
                end_loop = false;
                break;
            }
        }
    }
    return new_center;
}

void Cluster::generate_path(int to_visit, std::vector<Location*>& path, Random& cl_engine)
{
    int last_index = -1;
    for (auto& a : groups)
    {
        last_index += a.size();
    }
    std::vector<int> result_indexes;
    for (int i = 0; i < to_visit; ++i)
    { // select the random indexes
        bool continue_loop = true;
        int curr_index;
        while (continue_loop)
        {
            continue_loop = false;
            curr_index = cl_engine.int_uniform(0, last_index);
            for (auto& a : result_indexes)
            {
                if (curr_index == a)
                {
                    continue_loop = true;
                    break;
                }
            }
        }
        result_indexes.push_back(curr_index);
    }
    for (int i = 0; i < to_visit; ++i)
    {
        path.push_back(select_location(result_indexes[i]));
    }
}

Location* Cluster::select_location(unsigned n)
{
    assert(n >= 0 && n < locations_num());
    int g_size = groups.size();
    int group_index;
    for (int j = 0; j < g_size; ++j)
    {
        int size = groups[j].size();
        if (n < size)
        {
            group_index = j;
            j = g_size;
        }
        else
        {
            n -= size;
        }
    }
    return &groups[group_index].Locations()[n];
}

// unused
// std::vector<Location *> Cluster::Location_list()
//{
//    std::vector<Location *> result;
//    result.clear();
//    for (unsigned int i = 0; i < groups.size(); ++i)
//    {
//        std::vector<Location *> temporary = groups[i].Location_list();
//        for (unsigned int j = 0; j < temporary.size(); ++j)
//        {
//            result.push_back(temporary[j]);
//        }
//    }
//    return result;
//}
//
// std::vector<Person *> Cluster::Person_list()
//{
//    std::vector<Person *> result;
//    result.clear();
//    for (unsigned int i = 0; i <= people.size(); ++i)
//    {
//        result.push_back(&people[i].Person_ref());
//    }
//    return result;
//}

} // namespace smooth_sim