#include "cluster.hpp"

using namespace sim;

std::vector<Location *> Cluster::Location_list()
{
    std::vector<Location*> result;
    result.clear();
    for(unsigned int i = 0; i <= groups.size(); ++i){
        std::vector<Location*> temporary = groups.operator[](i).Location_list();
        for(unsigned int j = 0; j <= temporary.size(); ++j)
        {
            result.push_back(temporary.operator[](j));
        }
    }
    return result;
}

std::vector<Person *> Cluster::Person_list()
{
    std::vector<Person*> result;
    result.clear();
    for(unsigned int i = 0; i <= Population.size(); ++i){
        result.push_back(&Population.operator[](i));
    }
    return result;
}

int Cluster::number_of_locations()
{
    int sum{};
    for(auto& a: groups){
        sum += a.Location_list().size();
    }
    return sum;
}

int Cluster::number_of_people()
{
    return Population.size();
}