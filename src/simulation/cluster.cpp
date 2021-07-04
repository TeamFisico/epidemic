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
Cluster::Cluster(int S, int E, int I, int R, int cluster_locations_num, Rectangle Area,
                 Zone cluster_zone, int cluster_label, double cluster_LATP_parameter)
    : Area{Area},
      zone{cluster_zone},
      index{cluster_label},
      LATP_alpha{cluster_LATP_parameter},
      cl_engine{}
{
    ///////// Assigning home to families and set everyone to be at home and Status::Susceptible /////////

    int N = S + E + I + R;  //total number of People
    generate_people(N,cluster_label);

    ///////// Setting the starting Exposed,Infected,Recovered individuals /////////

    set_E_I_R_individuals(E,I,R);

    ///////// Generate and construct groups into this cluster /////////

    generate_groups(cluster_locations_num);

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////           PRIVATE METHODS           /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////// GENERATE People IN THIS CLUSTER /////////////////
void Cluster::generate_people(int People_num,int cluster_label)
{
    assert(People.empty());
    People.reserve(People_num);
    int index = 0;
    while (index < People_num)
    {
        int family_size = cl_engine.rounded_gauss(MEAN_FAMILY_COMPONENTS,MEAN_FAMILY_STDDEV); //determine family_size
        if (family_size < 1 ) { family_size = 1; }

        Location current_home =
            rand_loc(Area.get_blh_corner(), Area.get_trh_corner(), HOME_RADIUS, cluster_label, cl_engine);
        for (int j = 0; j < family_size && index < People_num; ++j)
        {
            Person curr{Status::Susceptible, current_home.get_pos(), Status::Susceptible, current_home, cluster_label};
            People.emplace_back(curr, 0, HOME_PROBABILITY, true);
            People[index].recall_home(); // set target location as person's home
            ++index;
        }
    }
}
///////////////// GENERATE GROUPS /////////////////
void Cluster::generate_groups(int locations_num)
{
    // Determine groups number
    int number_of_groups = cl_engine.rounded_gauss(locations_num * MEAN_GROUP_SIZE, locations_num * GROUP_SIZE_STDDEV);
    // Create a Vector which will have the partition of locations in group,with every group having at least one location
    std::vector<int> loc_num(number_of_groups, 1);

    int loc_left = locations_num - number_of_groups;
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
    assert(groups.empty());
    groups.reserve(number_of_groups);
    for (int i = 0; i < number_of_groups; ++i) //construct groups vector element by element
    {
        groups.emplace_back(loc_num[i], gen_group_center(loc_num[i]), label);
    }
}
///////////////// GENERATE GROUP CENTER /////////////////
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
///////////////// SET E,I,R INDIVIDUALS /////////////////
void Cluster::set_E_I_R_individuals(int E, int I, int R)
{
    // change person's condition and next condition to have the right number of Statuss
    for (int i = 0; i < E + I + R; ++i)
    {
        if (i < E)
        {
            People[i].person_ref().set_current_status(Status::Exposed);
            People[i].person_ref().set_new_status(Status::Exposed);
        }
        else if (i < E + I)
        {
            People[i].person_ref().set_current_status(Status::Infected);
            People[i].person_ref().set_new_status(Status::Infected);
        }
        else if (i < E + I + R)
        {
            People[i].person_ref().set_current_status(Status::Recovered);
            People[i].person_ref().set_new_status(Status::Recovered);
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////           PUBLIC METHODS            /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////// SIZE OF THE CLUSTER  /////////////////
unsigned Cluster::size() const
{
    return groups.size();
}
///////////////// AREA OF THE CLUSTER /////////////////
Rectangle& Cluster::area()
{
    return Area;
}
///////////////// BASE OF CLUSTER RECTANGULAR AREA /////////////////
double Cluster::base()
{
    return Area.get_trh_corner().get_x() - Area.get_blh_corner().get_x();
}
///////////////// HEIGHT OF CLUSTER RECTANGULAR AREA /////////////////
double Cluster::height()
{
    return Area.get_trh_corner().get_y() - Area.get_blh_corner().get_y();
}

int Cluster::locations_num() const
{
    auto add_op = [&](unsigned int a, Group b) { return a + b.size(); };
    return std::accumulate(std::begin(groups), std::end(groups), 0, add_op);
}

int Cluster::people_num() const
{
    return People.size();
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

Location* Cluster::select_location(int n)
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
//    for (unsigned int i = 0; i <= People.size(); ++i)
//    {
//        result.push_back(&People[i].Person_ref());
//    }
//    return result;
//}

} // namespace smooth_sim