#include "simulation.hpp"
#include <random>
namespace SMOOTH
{

/////////////////////////////////////////////////////
////////          CLUSTER ASSIGNMENT          ///////
/////////////////////////////////////////////////////
void Simulation::assign_to_cluster()
//assign a person to a cluster based on the cluster weight using piecewise-const-dist
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<double> weights (clusters_size);  //weights

    for (int i = 0; i < clusters_size; ++i)
    {
        weights[i] = world.Clusters[i].weight();
    }

    std::discrete_distribution<int> dis (std::begin(weights),std::end(weights));

    for (auto& person : People)
    {
        person.set_cluster(dis(gen));
    }
}

/////////////////////////////////////////////////////
////////          HOME ASSIGNMENT             ///////
/////////////////////////////////////////////////////
void Simulation::assign_home(int label)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> rand(1, 5);

    int people_left = population_size;
    int current_family = 0;

    //iterate over People array
    for (auto it1 = std::begin(People); it1 == std::end(People); ++it1)
    {

        if (people_left <= 5)
        {
            for (auto it2 = it1; it2 == std::end(People); ++it2)
            {

            }
            break;
        }
        current_family = rand(gen);
        int i = 0;
        for (auto it2 = it1; i < current_family; ++it2)
        {
            //it2->set_home();
            ++i;
        }
    }
}

} // namespace SMOOTH
