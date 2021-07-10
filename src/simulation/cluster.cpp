#include "cluster.hpp"
#include "parameters.hpp"
#include "random.hpp"
#include <cassert>
#include <iostream>
#include <random>

namespace smooth_sim
{

//////////////////////////////////////////////
///////      CLUSTER CONSTRUCTOR        //////
//////////////////////////////////////////////
Cluster::Cluster(int S, int E, int I, int R, int cluster_locations_num, Rectangle Area, Zone cluster_zone,
                 int cluster_label, double cluster_LATP_parameter)
    : Area{Area},
      zone{cluster_zone},
      index{cluster_label},
      LATP_alpha{cluster_LATP_parameter},
      cl_engine{}
{
    ///////// Assigning home to families and set everyone to be at home and Status::Susceptible /////////

    // Total number of People
    int N = S + E + I + R;
    // Construct People vector
    generate_people(N, cluster_label);

    ///////// Setting the starting Exposed,Infected,Recovered individuals /////////

    set_E_I_R_individuals(E, I, R);

    ///////// Generate and construct groups into this cluster /////////

    generate_groups(cluster_locations_num);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////           PRIVATE METHODS           /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////// GENERATE PEOPLE IN THIS CLUSTER /////////////////
void Cluster::generate_people(int People_num, int cluster_label)
{
    assert(People.empty());
    People.reserve(People_num);
    int p_index = 0;
    while (p_index < People_num)
    {
        // Determine family_size
        int family_size = cl_engine.rounded_gauss(MEAN_FAMILY_COMPONENTS, MEAN_FAMILY_STDDEV);
        // Make sure the family_size is at least 1
        if (family_size < 1) { family_size = 1; }
        // Generate the home
        Location current_home =
            generate_home_loc(Area.get_blh_corner(), Area.get_trh_corner(), HOME_RADIUS, cluster_label, cl_engine);
        // Generate all people in the family
        for (int j = 0; j < family_size && p_index < People_num; ++j)
        {
            Person curr{Status::Susceptible, current_home.get_position(), Status::Susceptible, current_home,
                        cluster_label};
            People.emplace_back(curr, 0, HOME_PROBABILITY, true);
            // Set target location as person's home
            People[p_index].recall_home();
            ++p_index;
        }
    }
}
///////////////// GENERATE GROUPS /////////////////
void Cluster::generate_groups(int locations_num)
{
    // Determine groups number
    int number_of_groups = cl_engine.rounded_gauss(locations_num * MEAN_GROUP_SIZE, locations_num * GROUP_SIZE_STDDEV);

    // Make sure there is at least one group
    if (number_of_groups <= 0) { number_of_groups = 1; }
    // Make sure the number of Locations is >= than the number og groups
    if (number_of_groups > locations_num) { number_of_groups = locations_num; }

    assert(locations_num >= number_of_groups && number_of_groups >= 1);
    // Create a Vector which will have the partition of locations in group,with every group having at least one location
    std::vector<int> loc_num(number_of_groups, 1);

    int loc_left = locations_num - number_of_groups;
    for (int i = 0; i < number_of_groups; ++i)
    {
        if (loc_left == 0)
        {
            // End the loop
            i = number_of_groups;
        }
        else if (loc_left == 1)
        {
            ++loc_num[i];
            // End the loop
            i = number_of_groups;
        }
        // If it is the last group
        else if (i == number_of_groups - 1)
        {
            // Add the rest of locations to the last group
            loc_num[i] += loc_left;
        }
        else
        {
            // Generate the number of locations the current group have
            int rnum = cl_engine.int_uniform(1, nearbyint(loc_left / 2));
            loc_num[i] += rnum;
            loc_left -= rnum;
        }
    }
    assert(std::accumulate(loc_num.begin(), loc_num.end(), 0) == locations_num);
    assert(Groups.empty());
    Groups.reserve(number_of_groups);
    // Construct groups vector element by element
    for (int i = 0; i < number_of_groups; ++i)
    {
        Groups.emplace_back(loc_num[i], gen_group_center(loc_num[i]), index);
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
        // Generate random center position
        new_center = rand_pos(Area.get_blh_corner(), Area.get_trh_corner(), cl_engine);
        // Check if this center is far enough from other groups center, if not generate a new one
        for (auto& a : Groups)
        {
            if (a.get_centre().distance_to(new_center) <= (a.locations_num() + num_of_loc) * TRANSMISSION_RANGE / 10)
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
    // Change person's condition and next condition to have the right number of Statuses
    for (int i = 0; i < E + I + R; ++i)
    {
        if (i < E)
        {
            People[i].person().set_current_status(Status::Exposed);
            People[i].person().set_new_status(Status::Exposed);
        }
        else if (i < E + I)
        {
            People[i].person().set_current_status(Status::Infected);
            People[i].person().set_new_status(Status::Infected);
        }
        else if (i < E + I + R)
        {
            People[i].person().set_current_status(Status::Recovered);
            People[i].person().set_new_status(Status::Recovered);
        }
    }
}
///////////////// POINTER TO SELECTED LOCATION /////////////////
Location* Cluster::select_location(int n)
{
    assert(n >= 0 && n < locations_num());
    int g_size = Groups.size();
    int group_index;
    // Check in what group n-th location is
    for (int j = 0; j < g_size; ++j)
    {
        int size = Groups[j].locations_num();
        if (n < size)
        {
            // Group found
            group_index = j;
            // End the loop
            j = g_size;
        }
        else
        {
            n -= size;
        }
    }
    // Return the location pointer
    return &Groups[group_index].locations()[n];
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////           PUBLIC METHODS            /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////// PEOPLE IN THIS CLUSTER /////////////////
std::vector<Mobility_model> Cluster::get_people() const
{
    return People;
}
///////////////// NUMBER OF PEOPLE IN THIS CLUSTER /////////////////
int Cluster::people_num() const
{
    return People.size();
}
///////////////// SIZE OF THE CLUSTER  /////////////////
unsigned Cluster::size() const
{
    return Groups.size();
}
int Cluster::locations_num() const
{
    // Define the operation
    auto add_op = [&](unsigned int a, const Group& b) { return a + b.locations_num(); };
    // Sum using the defined operation
    return std::accumulate(std::begin(Groups), std::end(Groups), 0, add_op);
}
///////////////// CLUSTER ZONE /////////////////
Zone Cluster::get_zone() const
{
    return zone;
}
///////////////// CLUSTER INDEX IN CLUSTERS VECTOR(WORLD) /////////////////
int Cluster::get_label() const
{
    return index;
}
///////////////// CLUSTER LATP PARAMETER /////////////////
double Cluster::get_LATP() const
{
    return LATP_alpha;
}
///////////////// REFERENCE TO PEOPLE IN THIS CLUSTER /////////////////
std::vector<Mobility_model>& Cluster::people()
{
    return People;
}
///////////////// REFERENCE TO GROUPS OF THE CLUSTER /////////////////
std::vector<Group>& Cluster::groups()
{
    return Groups;
}
///////////////// REFERENCE TO AREA OF THE CLUSTER /////////////////
Rectangle& Cluster::area()
{
    return Area;
}
///////////////// SET ZONE OF THE CLUSTER /////////////////
void Cluster::set_zone(Zone cluster_zone)
{
    zone = cluster_zone;
}
///////////////// SET LATP PARAMETER OF THE CLUSTER /////////////////
void Cluster::set_LATP(double new_LATP_parameter)
{
    LATP_alpha = new_LATP_parameter;
}
///////////////// PATH GENERATION FOR A PERSON /////////////////
void Cluster::generate_cluster_path(int to_visit, std::vector<Location*>& path)
{
    // In the case of more locations to visit than in the cluster, return pointers to all locations
    if (to_visit >= locations_num())
    {
        for (auto& gr : Groups)
        {
            for (auto& l : gr.locations())
            {
                path.push_back(&l);
            }
        }
    }
    else
    {
        int last_index = locations_num() - 1;
        // Vector with the locations indexes result
        std::vector<int> result_indexes;
        // Select the random indexes
        for (int i = 0; i < to_visit; ++i)
        {
            bool continue_loop = true;
            int curr_index;
            while (continue_loop)
            {
                continue_loop = false;
                curr_index = cl_engine.int_uniform(0, last_index);
                // Check if current location is already chosen
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
        assert(to_visit == static_cast<int>(result_indexes.size()));
        // Fill the path with the index in the vector
        for (int i = 0; i < to_visit; ++i)
        {
            path.push_back(select_location(result_indexes[i]));
        }
    }
}

} // namespace smooth_sim