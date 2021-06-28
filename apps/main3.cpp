#include "simulation.hpp"
#include <iostream>
#include <fstream>

int main()
{
    using namespace smooth_simulation;

    using namespace std::literals::chrono_literals;

    Data starting_data{POPULATION_SIZE - 62, 50, 10, 2, 0, 0};

    Simulation sim{500, 0.5, 0.1, 0.5, 0.02, 0.1, starting_data};

    auto start = std::chrono::high_resolution_clock::now();

    sim.simulate();

    auto end = std::chrono::high_resolution_clock::now();


    std::chrono::duration<float> duration = end - start;
    std::cout << "Time taken: " << duration.count() << " s " << std::endl;
}