#include "simulation.hpp"
#include <chrono>
#include <iostream>

int main()
{
    using namespace sim;

    auto start = std::chrono::high_resolution_clock::now();

    Simulation prova{200000 ,3, 4, 4, 10, 500, 1000,0.5,0.3,0.2,1,20,10};

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> duration = end - start;
    std::cout << "Time taken : " << duration.count() << " s " << std::endl;

    auto start2 = std::chrono::high_resolution_clock::now();

    prova.spread();

    auto end2 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> duration2 = end2 - start2;
    std::cout << "Time taken : " << duration2.count() << " s " << std::endl;





}