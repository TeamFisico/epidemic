#include "simulation.hpp"
#include <iostream>
#include <random>
#include "random.hpp"


int main()
{
    using namespace SMOOTH;


    using namespace std::literals::chrono_literals;

    auto start = std::chrono::high_resolution_clock::now();

    Random rd;

    auto end = std::chrono::high_resolution_clock::now();





    std::chrono::duration<float> duration = end - start;
    std::cout << "Time taken : " << duration.count() << " s " << std::endl;
}