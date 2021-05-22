#include "area.hpp"
#include <random>
#include <iostream>

int main()
{
    using namespace SMOOTH;

    Area world{1000, 1};

    using namespace std::literals::chrono_literals;

    auto start = std::chrono::high_resolution_clock::now();

    world.partition();
    world.plot_waypoints();

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> duration = end - start;
    std::cout << duration.count() << " s " << std::endl;

    int i = 1;
    for (auto &wpts : world.Waypoints)
    {
        std::cout << "[" << i << "]==" << wpts.X() << '\t' << wpts.Y() << std::endl;
        ++i;
    }

    for (auto& cluster: world.Clusters)
    {
        std::cout<< "Cluster.weight == " << cluster.weight() << std::endl;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<double> weights (clusters_size);  //weights

    for (int i = 0; i < clusters_size; ++i)
    {
        weights[i] = world.Clusters[i].weight();
    }

    std::discrete_distribution<int> dis (std::begin(weights),std::end(weights));

    for (int j = 0; j < 100; ++j)
    {
        std:: cout << dis(gen) << std::endl;
    }
}