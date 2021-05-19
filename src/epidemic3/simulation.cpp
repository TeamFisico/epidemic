#include "simulation.hpp"
#include <random>
namespace SMOOTH
{

// Extract groups of 1 to 5 people and assign them the same home
void Simulation::assign_home(int label)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> rand(1, 5);

    int people_left = population_size;
    int current_fam = 0;

    for (auto it = std::begin(People); it == std::end(People); ++it)
    {

        if (people_left <= 5)
        {
            for (auto it2 = it; it2 == std::end(People); ++it2)
            {
                //                it2->set_home();
            }
            break;
        }
        current_fam = rand(gen);
        int i = 0;
        for (auto it2 = it; i < current_fam; it2 += i)
        {
            //            it2->set_home();
        }
    }
}

void Simulation::assign_to_cluster()
{

}



} // namespace SMOOTH








































