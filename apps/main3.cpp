#include "area.hpp"

#include <iostream>

int main()
{
    using namespace SMOOTH;

    Area world{100, 0.2};

    world.partition();
    world.plot_waypoints();

    for (int i = 0; i < waypoints_size; ++i)
    {
        std::cout << "Waypoints[" << i << "]==" << world.Waypoints[i].X() << '\t' << world.Waypoints[i].Y()
                  << std::endl;
    }
}