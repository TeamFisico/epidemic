#include "world.hpp"
#include <random>

using namespace sim;

std::vector<Location *> World::Location_list()
{
    std::vector<Location *> result;
    result.clear();
    for (unsigned int i = 0; i <= clusters.size(); ++i)
    {
        std::vector<Location *> temporary = clusters.operator[](i).Location_list();
        for (unsigned int j = 0; j <= temporary.size(); ++j)
        {
            result.push_back(temporary.operator[](j));
        }
    }
    return result;
}

std::vector<Person *> World::Total_Population()
{
    std::vector<Person *> result;
    result.clear();
    for (unsigned int i = 0; i <= clusters.size(); ++i)
    {
        std::vector<Person *> temporary = clusters.operator[](i).Person_list();
        for (unsigned int j = 0; j <= temporary.size(); ++j)
        {
            result.push_back(temporary.operator[](j));
        }
    }
    return result;
}

Cluster *World::select_cluster()
{
    // generate vector with number of locations for every cluster
    int total_pop = number_of_people();
    int last_index = clusters.size() - 1;
    std::vector<int> location_number{};
    for (int i = 0; i <= last_index; ++i)
    {
        location_number.push_back(clusters.operator[](i).number_of_locations());
    }
    // select index of the cluster randomly based on the people_number vector
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> rand(1, total_pop);
    int rnum = rand(gen);
    int index_result{};
    for (int i = 0; i <= last_index; ++i)
    {
        if (rnum <= location_number.operator[](i))
        {
            index_result = i;
            break;
        }
        rnum -= location_number.operator[](i);
    }
    // return pointer to cluster at selected index
    return &clusters.operator[](index_result);
}

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

std::vector<Location *> sim::generate_path(std::vector<Location *> list, double mean, double dev)
{
    int to_visit = rounded_norm(mean - 1, dev) + 1; // make sure is always >= 1
    int last_index = list.size() - 1;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> rand(0, last_index);
    std::vector<Location *> result;
    result.clear();
    for (int i = 1; i <= to_visit; ++i)
    {
        bool continue_loop = true;
        Location *current = nullptr;
        while (continue_loop)
        {
            continue_loop = false;
            current = list.operator[](rand(gen));
            for (auto a : result)
            {
                if (current == a)
                {
                    continue_loop = true;
                }
            }
        }
        result.push_back(current);
    }
    return result;
}