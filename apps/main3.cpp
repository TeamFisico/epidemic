#include "area.hpp"
#include <iostream>
#include <chrono>
#include <random>

int main()
{
    using namespace SMOOTH;

    Area world{100, 0.5};

    using namespace std::literals::chrono_literals;

    auto start = std::chrono::high_resolution_clock::now();

    world.partition();
    world.plot_waypoints();

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> duration = end - start;
    std::cout << "Time taken : " << duration.count() << " s " << std::endl;

    int i = 1;
    for (auto& wpts : world.Waypoints)
    {
        std::cout << "[" << i << "]==" << wpts.X() << '\t' << wpts.Y() << std::endl;
        ++i;
    }

    for (auto& cluster : world.Clusters)
    {
        std::cout << "Cluster.weight == " << cluster.weight() << std::endl;
    }
}