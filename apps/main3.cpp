#include "../include/random.hpp"
#include "simulation.hpp"
#include <iostream>
#include <random>

int main()
{
    using namespace SMOOTH;

    using namespace std::literals::chrono_literals;

    auto start = std::chrono::high_resolution_clock::now();
    Data starting {POPULATION_SIZE,0,0,0,0};
    Simulation sim{1000,0.5,0.7,starting};
    auto end = std::chrono::high_resolution_clock::now();

//    for (auto& wpt : sim.Waypoints)
//    {
//        std::cout << wpt.X() << '\t' << wpt.Y() << std::endl;
//    }
    int i = 0;
    for (Cluster& cl : sim.Clusters)
    {
        std::cout << "First waypoint of first group in Cluster " << i+1<<std::endl;
        std::cout << cl.Groups[i].pointed_waypoint()[0].X();
        std::cout << '\t';
        std::cout << cl.Groups[i].pointed_waypoint()[0].Y() << std::endl;
        ++i;
    }

    std::chrono::duration<float> duration = end - start;
    std::cout << "Time taken : " << duration.count() << " s " << std::endl;
}