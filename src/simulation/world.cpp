#include "world.hpp"
#include "random.hpp"
#include <cassert>
#include <random>

namespace smooth_sim
{

////////////////////////////////////////////////////////
/////               WORLD CONSTRUCTOR             //////
////////////////////////////////////////////////////////
World::World(double Side_length, int number_of_clusters, int number_of_locations, int S, int E, int I, int R)
    : wrld_eng{}
{
    ///////// Area partitioning  /////////

    Position blh_corner{0, 0};
    Position trh_corner{Side_length, Side_length};
    Area = {blh_corner, trh_corner};
    std::vector<Rectangle> cluster_areas = Area.divide(number_of_clusters);

    ///////// Locations in each cluster determination /////////

    std::vector<int> locations_number(number_of_clusters, 0);
    fill_with_locations_num(number_of_clusters, number_of_locations, locations_number);

    ///////// Distribution of S,E,I,R people over the clusters determination /////////

    std::vector<int> susceptibles(number_of_clusters, 0);
    std::vector<int> exposed(number_of_clusters, 0);
    std::vector<int> infected(number_of_clusters, 0);
    std::vector<int> recovered(number_of_clusters, 0);

    fill_with_S_individuals(number_of_clusters, S, susceptibles);
    fill_with_E_I_R_individuals(number_of_clusters, E, I, R, exposed, infected, recovered);

    ///////// Clusters construction /////////

    Clusters.reserve(number_of_clusters);

    for (int i = 0; i < number_of_clusters; ++i) // construct clusters vector element by element
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
        if (i == clusters_num - 1) // the current cluster is the last one
        {
            loc_num[i] += loc_left;
        }
        else
        {
            double mean = static_cast<double>(loc_left) / static_cast<double>(remaining_cluster);
            int current = wrld_eng.rounded_gauss(mean, mean / 4);
            while (current >= loc_left || current <= 0) // make sure current value is valid
            {
                current = wrld_eng.rounded_gauss(mean, mean / 4);
            }
            loc_num[i] += current;
            loc_left -= current;
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
        if (i == clusters_num - 1) // current cluster is the last one
        {
            S_v[i] += S_pop_left;
        }
        else
        {
            double mean = (static_cast<double>(S_pop_left) / static_cast<double>(left_clusters));
            int current = wrld_eng.rounded_gauss(mean, mean / 4);
            while (current >= 4 * S_pop_left / 5 || current <= S_pop_left / 5) // make sure current value is valid
            {
                current = wrld_eng.rounded_gauss(mean, mean / 4);
            }
            S_v[i] += current;
            S_pop_left -= current;
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

    int constexpr ppl_block = 10;

    int E_pop_left = E;
    while (E_pop_left > ppl_block)
    {
        E_v[wrld_eng.int_uniform(0, -1)] += ppl_block;
        E_pop_left -= ppl_block;
    }
    E_v[wrld_eng.int_uniform(0, clusters_nums - 1)] += E_pop_left;
    // I individuals
    int I_pop_left = I;
    while (I_pop_left > ppl_block)
    {
        I_v[wrld_eng.int_uniform(0, clusters_nums - 1)] += ppl_block;
        I_pop_left -= ppl_block;
    }
    I_v[wrld_eng.int_uniform(0, clusters_nums - 1)] += I_pop_left;
    // R Individuals
    int R_pop_left = R;
    while (R_pop_left > ppl_block)
    {
        R_v[wrld_eng.int_uniform(0, clusters_nums - 1)] += ppl_block;
        R_pop_left -= ppl_block;
    }
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
    auto add_op = [&](unsigned a, Cluster b) { return a + b.locations_num(); };
    return std::accumulate(std::begin(Clusters), std::end(Clusters), 0, add_op);
}
///////////////// NUMBER OF PEOPLE IN WORLD /////////////////
unsigned World::people_num() const
{
    auto add_op = [&](unsigned int a, Cluster b) { return a + b.people_num(); };
    return std::accumulate(std::begin(Clusters), std::end(Clusters), 0, add_op);
}
///////////////// GET CLUSTERS /////////////////
std::vector<Cluster> World::get_clusters() const
{
    return Clusters;
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
    // the vector weight has to be created in Simulation::move() for every cluster so
    // that the weight of the current cluster is equal the sum of the other weights
    path.reserve(to_visit);
    // Using cluster to generate path
    std::vector<int> choose(weights.size(), 0); // number of location to chose from any of the cluster
    for (int i = 0; i < to_visit; ++i)
    {
        choose[engine.discrete(weights)] += 1;
    }
    for (unsigned long i = 0; i < weights.size(); ++i)
    {
        if (choose[i] > 0) { Clusters[i].generate_path(choose[i], path); }
    }
}
} // namespace smooth_sim

////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// UNUSED FUNCTIONS
// std::vector<Location *> World::Location_list()
//{
//    std::vector<Location *> result;
//    result.clear();
//    for (unsigned int i = 0; i < clusters.size(); ++i)
//    {
//        std::vector<Location *> temporary = clusters[](i).Location_list();
//        for (unsigned int j = 0; j < temporary.size(); ++j)
//        {
//            result.push_back(temporary[](j));
//        }
//    }
//    return result;
//}
//
// std::vector<Person *> World::Total_Population()
//{
//    std::vector<Person *> result;
//    result.clear();
//    for (unsigned int i = 0; i < clusters.size(); ++i)
//    {
//        std::vector<Person *> temporary = clusters[](i).Person_list();
//        for (unsigned int j = 0; j < temporary.size(); ++j)
//        {
//            result.push_back(temporary[](j));
//        }
//    }
//    return result;
//}
//
// Cluster *World::select_cluster()
//{
//    // generate vector with number of locations for every cluster
//    int total_pop = number_of_people();
//    int last_index = clusters.size() - 1;
//    std::vector<int> location_number{};
//    for (int i = 0; i <= last_index; ++i)
//    {
//        location_number.push_back(clusters[](i).number_of_locations());
//    }
//    // select index of the cluster randomly based on the people_number vector
//    std::random_device rd;
//    std::mt19937 gen(rd());
//    std::uniform_int_distribution<> rand(1, total_pop);
//    int rnum = rand(gen);
//    int index_result{};
//    for (int i = 0; i <= last_index; ++i)
//    {
//        if (rnum <= location_number[](i))
//        {
//            index_result = i;
//            break;
//        }
//        rnum -= location_number[](i);
//    }
//    // return pointer to cluster at selected index
//    return &clusters[](index_result);
//}

// Cluster *World::get_cluster(int index)
//{
//    auto it = clusters.begin();
//    it += index;
//    return &(*it);
//}
// unused function
//
// std::vector<Location *> generate_path(std::vector<Location *> list, double mean, double dev)
//{
//    Random rng;
//    int to_visit = rng.rounded_gauss(mean - 1, dev) + 1; // make sure is always >= 1
//    int last_index = list.size() - 1;
//    std::vector<Location *> result;
//    result.clear();
//    for (int i = 1; i <= to_visit; ++i)
//    {
//        bool continue_loop = true;
//        Location *current = nullptr;
//        while (continue_loop)
//        {
//            continue_loop = false;
//            current = list[](rng.int_uniform(0, last_index));
//            for (auto a : result)
//            {
//                if (current == a) { continue_loop = true; }
//            }
//        }
//        result.push_back(current);
//    }
//    return result;
//}