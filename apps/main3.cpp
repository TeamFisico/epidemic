#include "simulation.hpp"
#include <iostream>

int main()
{
    using namespace smooth_simulation;

    using namespace std::literals::chrono_literals;

    Data starting{POPULATION_SIZE, 0, 0, 0, 0};
    auto start = std::chrono::high_resolution_clock::now();

    Simulation sim{1200, 0.5, starting};
    auto end = std::chrono::high_resolution_clock::now();

    int i = 1;
    for (auto& wpt : sim.Waypoints)
    {
        std:: cout << "[" << i << "]" << wpt.get_X() << "\t" << wpt.get_Y() << std::endl;
        ++i;
    }

    std::chrono::duration<float> duration = end - start;
    std::cout << "Time taken: " << duration.count() << " s " << std::endl;
}