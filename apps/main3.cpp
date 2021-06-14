#include "simulation.hpp"
#include <iostream>

int main()
{
    using namespace smooth_simulation;

    using namespace std::literals::chrono_literals;

    auto start = std::chrono::high_resolution_clock::now();
    Data starting{POPULATION_SIZE, 0, 0, 0, 0};
    Simulation sim{10000, 0.5, starting};
    auto end = std::chrono::high_resolution_clock::now();

//    int j = 0;
//    for (auto& person : sim.People)
//    {
//        std::cout << "People[" << j<< "].home_cluster == " << person.home_cluster() << std::endl;
//        std::cout << "People[" << j<< "].home == " << person.get_home().get_X() << "\t" << person.get_home().get_Y() << std::endl;
//        ++j;
//    }

    std::chrono::duration<float> duration = end - start;
    std::cout << "Time taken for world construction: " << duration.count() << " s " << std::endl;
}