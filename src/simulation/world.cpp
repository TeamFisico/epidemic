#include "world.hpp"
#include "random.hpp"
#include <cassert>
#include <iostream>
#include <random>

namespace smooth_sim
{

////////////////////////////////////////////////////////
/////               WORLD CONSTRUCTOR             //////
////////////////////////////////////////////////////////
World::World(int side_length, int number_of_clusters, int number_of_locations, int S, int E, int I, int R)
    : wrld_eng{},
      side{side_length}
{
    ///////// Area partitioning  /////////

    Position blh_corner{0, 0};
    Position trh_corner{static_cast<double>(side_length), static_cast<double>(side_length)};
    Area = {blh_corner, trh_corner};
    // Generate the Clusters Areas by dividing the world Area
    std::vector<Rectangle> cluster_areas = Area.divide(number_of_clusters, wrld_eng);

    ///////// Locations in each cluster determination /////////

    // Vector with the number of locations for every cluster, make sure every cluster has at least a location
    std::vector<int> locations_number(number_of_clusters, 1);
    // Fill the vector
    fill_with_locations_num(number_of_clusters, number_of_locations - number_of_clusters, locations_number);

    ///////// Distribution of S,E,I,R people over the clusters determination /////////

    // Vectors with the number of people for every cluster
    std::vector<int> susceptibles(number_of_clusters, 0);
    std::vector<int> exposed(number_of_clusters, 0);
    std::vector<int> infected(number_of_clusters, 0);
    std::vector<int> recovered(number_of_clusters, 0);

    // Fill the vectors
    fill_with_S_individuals(number_of_clusters, S, susceptibles);
    fill_with_E_I_R_individuals(number_of_clusters, E, I, R, exposed, infected, recovered);

    ///////// Clusters construction /////////

    Clusters.reserve(number_of_clusters);

    // Construct clusters vector element by element
    for (int i = 0; i < number_of_clusters; ++i)
    {
        Clusters.emplace_back(susceptibles[i], exposed[i], infected[i], recovered[i], locations_number[i],
                              cluster_areas[i], Zone::Green, i, WHITE_ZONE_LATP_ALPHA);
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////           PRIVATE METHODS           /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////// FILL A VECTOR WITH NUMBER OF LOCATIONS FOR THE CORRESPONDING CLUSTER /////////////////
void World::fill_with_locations_num(unsigned clusters_num, int locations_num, std::vector<int>& loc_num)
{
    assert(loc_num.size() == clusters_num);
    int loc_left = locations_num;
    int remaining_cluster = clusters_num;
    for (unsigned i = 0; i < clusters_num; ++i)
    {
        // The current cluster is the last one
        if (i == clusters_num - 1)
        {
            // Add the remaining location to the last cluster
            loc_num[i] += loc_left;
        }
        else
        {
            double mean = static_cast<double>(loc_left) / static_cast<double>(remaining_cluster);
            // Generate random number of location
            int rand_loc_num = wrld_eng.rounded_gauss(mean, mean / 4);
            // Make sure the value is valid, if not generate again
            while (rand_loc_num >= loc_left || rand_loc_num <= 0)
            {
                rand_loc_num = wrld_eng.rounded_gauss(mean, mean / 4);
            }
            loc_num[i] += rand_loc_num;
            loc_left -= rand_loc_num;
            --remaining_cluster;
        }
    }
}
///////////////// FILL WITH S INDIVIDUALS IN CORRESPONDING CLUSTERS /////////////////
void World::fill_with_S_individuals(unsigned clusters_num, int S, std::vector<int>& S_v)
{
    assert(S_v.size() == clusters_num);

    int S_pop_left = S;
    int left_clusters = clusters_num;
    for (unsigned i = 0; i < clusters_num; ++i)
    {
        // Current cluster is the last one
        if (i == clusters_num - 1)
        {
            // Add the remaining individuals to the last cluster
            S_v[i] += S_pop_left;
        }
        else
        {
            double mean = (static_cast<double>(S_pop_left) / static_cast<double>(left_clusters));
            int rand_people_num = wrld_eng.rounded_gauss(mean, mean / 4);
            // Limit the possible values, if value is not valid, generate again
            while (rand_people_num >= 8 * mean / 5 || rand_people_num <= mean / 5 || rand_people_num >= S_pop_left ||
                   rand_people_num <= 0)
            {
                rand_people_num = wrld_eng.rounded_gauss(mean, mean / 4);
            }
            S_v[i] += rand_people_num;
            S_pop_left -= rand_people_num;
            --left_clusters;
        }
    }
}
///////////////// FILL WITH E,I,R INDIVIDUALS IN CORRESPONDING CLUSTERS /////////////////
void World::fill_with_E_I_R_individuals(unsigned clusters_nums, int E, int I, int R, std::vector<int>& E_v,
                                        std::vector<int>& I_v, std::vector<int>& R_v)
{
    assert(E_v.size() == clusters_nums);
    assert(I_v.size() == clusters_nums);
    assert(R_v.size() == clusters_nums);

    // Distribute, when possible, in group of 10
    int constexpr ppl_block = 10;
    // Distribute E individuals
    int E_pop_left = E;
    while (E_pop_left > ppl_block)
    {
        // Uniformly select the cluster
        E_v[wrld_eng.int_uniform(0, clusters_nums - 1)] += ppl_block;
        E_pop_left -= ppl_block;
    }
    // Distribute the last E individuals
    E_v[wrld_eng.int_uniform(0, clusters_nums - 1)] += E_pop_left;
    // Distribute I individuals
    int I_pop_left = I;
    while (I_pop_left > ppl_block)
    {
        // Uniformly select the cluster
        I_v[wrld_eng.int_uniform(0, clusters_nums - 1)] += ppl_block;
        I_pop_left -= ppl_block;
    }
    // Distribute the last I individuals
    I_v[wrld_eng.int_uniform(0, clusters_nums - 1)] += I_pop_left;
    // Distribute R individuals
    int R_pop_left = R;
    while (R_pop_left > ppl_block)
    {
        // Uniformly select the cluster
        R_v[wrld_eng.int_uniform(0, clusters_nums - 1)] += ppl_block;
        R_pop_left -= ppl_block;
    }
    // Distribute the last R individuals
    R_v[wrld_eng.int_uniform(0, clusters_nums - 1)] += R_pop_left;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////           PUBLIC METHODS            /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////// WORLD SIZE /////////////////
unsigned World::size() const
{
    return Clusters.size();
}
///////////////// NUMBER OF LOCATIONS IN WORLD /////////////////
unsigned World::locations_num() const
{
    // Define the operator
    auto add_op = [&](unsigned a, const Cluster& b) { return a + b.locations_num(); };
    // Sum using the previously defined operator
    return std::accumulate(std::begin(Clusters), std::end(Clusters), 0, add_op);
}
///////////////// NUMBER OF PEOPLE IN WORLD /////////////////
unsigned World::people_num() const
{
    // Define the operator
    auto add_op = [&](unsigned int a, const Cluster& b) { return a + b.people_num(); };
    // Sum using the previously defined operator
    return std::accumulate(std::begin(Clusters), std::end(Clusters), 0, add_op);
}
///////////////// GET AREA SIDE /////////////////
unsigned World::get_side() const
{
    return side;
}
///////////////// REFERENCE TO CLUSTERS /////////////////
std::vector<Cluster>& World::clusters()
{
    return Clusters;
}
///////////////// GENERATE PATH  /////////////////
void World::generate_path(int to_visit, const std::vector<double>& weights, std::vector<Location*>& path,
                          Random& engine)
{
    // The vector weight has to be created in Simulation::move() for every cluster so
    // that the weight of the current cluster is equal the sum of the other weights
    path.reserve(to_visit);
    // Using cluster to generate path
    // Vector that will hold the number of location to chose from any of the cluster
    std::vector<int> choose(weights.size(), 0);
    // Fill the vector
    for (int i = 0; i < to_visit; ++i)
    {
        // Choose the cluster where to choose using the weights vector
        choose[engine.discrete(weights)] += 1;
    }
    // Choose the locations from every cluster using generate_cluster_path
    for (unsigned long i = 0; i < weights.size(); ++i)
    {
        if (choose[i] > 0) { Clusters[i].generate_cluster_path(choose[i], path); }
    }
}
} // namespace smooth_sim