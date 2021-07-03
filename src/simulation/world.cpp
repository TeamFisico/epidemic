#include "world.hpp"
#include "random.hpp"
#include <random>

namespace smooth_sim
{
World::World(double Side_length, int number_of_clusters, int number_of_location, int S, int E, int I, int R,Random& simulation_engine)
      :wrld_eng{simulation_engine}
{
    Position blh_corner{0, 0};
    Position trh_corner{Side_length, Side_length};
    Area = {blh_corner, trh_corner};

    std::vector<Rectangle> cluster_areas = Area.Divide(number_of_clusters);
    // create and fill a vector with the number of location for each cluster
    std::vector<int> loc_num(number_of_clusters, 0);
    int loc_left = number_of_location;
    int remaining_cluster = number_of_clusters;
    for (int i = 0; i < number_of_clusters; ++i)
    {
        if (i == number_of_clusters - 1) { loc_num[i] += loc_left; }
        else
        {
            double mean = static_cast<double>(loc_left) / static_cast<double>(remaining_cluster);
            int current = wrld_eng.rounded_gauss(mean, mean / 4);
            while (current >= loc_left || current <= 0)
            { // make sure current value is valid
                current = wrld_eng.rounded_gauss(mean, mean / 4);
            }
            loc_num[i] += current;
            loc_left -= current;
            --remaining_cluster;
        }
    }
    // create and fill a vector with the number of susceptible individuals for each cluster
    std::vector<int> S_pop_num(number_of_clusters, 0);
    int S_pop_left = S;
    remaining_cluster = number_of_clusters;
    for (int i = 0; i < number_of_clusters; ++i)
    {
        if (i == number_of_clusters - 1) { S_pop_num[i] += S_pop_left; }
        else
        {
            double mean = static_cast<double>(S_pop_left) / static_cast<double>(remaining_cluster);
            int current = wrld_eng.rounded_gauss(mean, mean / 4);
            while (current >= 4 * S_pop_left / 5 || current <= S_pop_left / 5)
            { // make sure current value is valid
                current = wrld_eng.rounded_gauss(mean, mean / 4);
            }
            S_pop_num[i] += current;
            S_pop_left -= current;
            --remaining_cluster;
        }
    }
    // create and fill 3 vectors with the number of (E,I,R respectively) individuals for each cluster, in this case use
    // an uniform distribution that assign 10 individuals at a time(or the remaiming ones) E Individuals
    std::vector<int> E_pop_num(number_of_clusters, 0);
    int E_pop_left = E;
    while (E_pop_left > 10)
    {
        E_pop_num[wrld_eng.int_uniform(0, number_of_clusters - 1)] += 10;
        E_pop_left -= 10;
    }
    E_pop_num[wrld_eng.int_uniform(0, number_of_clusters - 1)] += E_pop_left;
    // I individuals
    std::vector<int> I_pop_num(number_of_clusters, 0);
    int I_pop_left = I;
    while (I_pop_left > 10)
    {
        I_pop_num[wrld_eng.int_uniform(0, number_of_clusters - 1)] += 10;
        I_pop_left -= 10;
    }
    I_pop_num[wrld_eng.int_uniform(0, number_of_clusters - 1)] += I_pop_left;
    // R Individuals
    std::vector<int> R_pop_num(number_of_clusters, 0);
    int R_pop_left = R;
    while (R_pop_left > 10)
    {
        R_pop_num[wrld_eng.int_uniform(0, number_of_clusters - 1)] += 10;
        R_pop_left -= 10;
    }
    R_pop_num[wrld_eng.int_uniform(0, number_of_clusters - 1)] += R_pop_left;
    // fill the cluster vector
    clusters.clear();
    clusters.reserve(number_of_clusters);
    for (int i = 0; i < number_of_clusters; ++i)
    {
        clusters.emplace_back(S_pop_num[i], E_pop_num[i], I_pop_num[i],R_pop_num[i], loc_num[i], cluster_areas[i], Color::Green,i,wrld_eng);
    }
}

//World::World() : Area{}, clusters{}
//{
//}

/////ununsed functions
//std::vector<Location *> World::Location_list()
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
//std::vector<Person *> World::Total_Population()
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
//Cluster *World::select_cluster()
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

int World::number_of_locations()
{
    int sum{};
    for (auto &a : clusters)
    {
        sum += a.number_of_locations();
    }
    return sum;
}

int World::number_of_people()
{
    int sum{};
    for (auto &a : clusters)
    {
        sum += a.number_of_people();
    }
    return sum;
}

// the vector weight has to be created in Simulation::move() for every cluster so
// that the weight of the current cluster is equal the sum of the other weights
void World::generate_path(int to_visit, const std::vector<double> &weights, std::vector<Location *> &path,Random &simulation_engine)
{
    path.reserve(to_visit);
    // Second Method, using the cluster generate path
    std::vector<int> choose(weights.size(), 0); // number of location to chose from any of the cluster
    for (int i = 0; i < to_visit; ++i)
    {
        choose[simulation_engine.discrete(weights)] += 1;
    }
    for (int i = 0; i < weights.size(); ++i)
    {
        if (choose[i] > 0) { clusters[i].generate_path(choose[i], path, simulation_engine); }
    }
}
/// UNUSED
//Cluster *World::get_cluster(int index)
//{
//    auto it = clusters.begin();
//    it += index;
//    return &(*it);
//}
//unused function
//
//std::vector<Location *> generate_path(std::vector<Location *> list, double mean, double dev)
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
} // namespace smooth_sim