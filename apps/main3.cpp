#include "simulation.hpp"
#include <iostream>
#include <random>

int main()
{
    using namespace SMOOTH;

    using namespace std::literals::chrono_literals;

    auto start = std::chrono::high_resolution_clock::now();


    Simulation sim {1000,0.5,0.5,0.8,10,20};

    auto end = std::chrono::high_resolution_clock::now();

    for (Location& wpt : sim.Waypoints)
    {
        std::cout << wpt.X() << '\t' << wpt.Y() << std::endl;
    }

    std::chrono::duration<float> duration = end - start;
    std::cout << "Time taken : " << duration.count() << " s " << std::endl;

}