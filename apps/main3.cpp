#include "simulation.hpp"
#include <iostream>
#include <random>

int main()
{
    using namespace SMOOTH;

    using namespace std::literals::chrono_literals;

    auto start = std::chrono::high_resolution_clock::now();

    Simulation sim {100,0.5,2.0,0.5,1,3};

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> duration = end - start;
    std::cout << "Time taken : " << duration.count() << " s " << std::endl;

}