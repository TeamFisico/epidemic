#include "simulation.hpp"
#include <iostream>

int main()
{
    using namespace smooth_simulation;

    using namespace std::literals::chrono_literals;

    Data starting{POPULATION_SIZE, 0, 0, 0, 0};

    Simulation sim{1000, 0.5, starting};

//    printing the locations in initial home locations
    auto start = std::chrono::high_resolution_clock::now();

    sim.move();
    auto end = std::chrono::high_resolution_clock::now();

//    for (auto& p : sim.People)
//    {
//        if (p.get_location().get_position() != p.get_home().get_position())
//        std:: cout << p.get_location().get_position().get_X()  << "\t" << p.get_location().get_Y()<<"\n";
//    }

    std::chrono::duration<float> duration = end - start;
    std::cout << "Time taken: " << duration.count() << " s " << std::endl;
}