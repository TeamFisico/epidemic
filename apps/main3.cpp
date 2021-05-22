#include "area.hpp"

#include <iostream>

int main()
{
    using namespace SMOOTH;

    Area world{1000, 0.5};

    using namespace std::literals::chrono_literals;

    auto start = std::chrono::high_resolution_clock::now();

    world.partition();
    world.plot_waypoints();

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> duration = end - start;
    std::cout << duration.count() << " s " << std::endl;

    int i = 0;
    for (auto &wpts : world.Waypoints)
    {
        std::cout << "Waypoints[" << i << "]==" << wpts.X() << '\t' << wpts.Y() << std::endl;
        ++i;
    }

    int j = 0;
    for (auto &cluster : world.Clusters)
    {
        std::cout << "Clusters[" << i << "].weight == " << cluster.cluster_weight() << "\n";
        ++j;
    }
}