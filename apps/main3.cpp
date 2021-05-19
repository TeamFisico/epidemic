#include "area.hpp"

#include <iostream>

int main()
{
    using namespace SMOOTH;

    Area world{1000, 0.5};

    world.partition();
    world.plot_waypoints();

    int i = 0;
    for (auto& wpts : world.Waypoints)
    {
        std::cout << "Waypoints[" << i << "]==" << wpts.X() << '\t' << wpts.Y()
                  << std::endl;
        ++i;
    }

    int j = 0;
    for (auto& cluster : world.Clusters)
    {
        std::cout<<"Clusters[" << i << "].weight == " << cluster.cluster_weight()<<"\n";
      ++j;
    }
}