#include "simulation.hpp"
#include <random>
namespace SMOOTH
{

/////////////////////////////////////////////////////
////////          CLUSTER ASSIGNMENT          ///////
/////////////////////////////////////////////////////
void Simulation::assign_to_cluster()
// assign a person to a cluster based on the cluster weight using piecewise-const-dist
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<double> weights(clusters_size); // weights

    for (int i = 0; i < clusters_size; ++i)
    {
        weights[i] = world.Clusters[i].weight();
    }

    std::discrete_distribution<int> dis(std::begin(weights), std::end(weights));

    for (auto& person : People)
    {
        int index = dis(gen);
        person.set_cluster(index, &(world.Clusters[index]));
    }
}

/////////////////////////////////////////////////////
////////          HOME ASSIGNMENT             ///////
/////////////////////////////////////////////////////
void Simulation::assign_home(int label)
// generate home address inside the relating cluster limits and generate groups of
// 1 to 5 people(families) which are gonna be assigned the same home location
{
    std::random_device rd1;
    std::mt19937 gen1(rd1());
    std::uniform_int_distribution<> family(1, 5);

    std::random_device rd2;
    std::mt19937 gen2(rd2());

    int people_left = population_size;
    int current_family = 0;
    double lw_x = 0.0;
    double up_x = 0.0;
    double lw_y = 0.0;
    double up_y = 0.0;

    // iterate over People array
    for (auto it1 = std::begin(People); it1 != std::end(People); ++it1)
    {
        // set the bounds based on the belonging cluster for home generation
        lw_x = world.Clusters[it1->cluster_label()].lower_x();
        up_x = world.Clusters[it1->cluster_label()].upper_x();
        lw_y = world.Clusters[it1->cluster_label()].lower_y();
        up_y = world.Clusters[it1->cluster_label()].upper_y();

        std::uniform_real_distribution<double> x_home(lw_x, up_x);
        std::uniform_real_distribution<double> y_home(lw_y, up_y);

        current_family = family(gen1);
        Location home{x_home(gen2), y_home(gen2)};

        if (people_left <= 5)
        {
            for (auto it2 = it1; it2 != std::end(People); ++it2)
            {
                it2->set_home(home);
            }
            return;
        }

        int i = 0;
        for (auto it2 = it1; i < current_family; ++it2)
        {
            it2->set_home(home);
            people_left -= current_family;
            ++i;
        }
    }
}

} // namespace SMOOTH
