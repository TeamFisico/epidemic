#include "iomanip"
#include "simulation.hpp"
#include <chrono>
#include <fstream>
#include <iostream>

int main()
{
    using namespace sim;

/*    auto start = std::chrono::high_resolution_clock::now();

    Simulation prova{25000 ,3, 25000, 4, 5, 500, 1000,0.5,0.3,0.2,0.5,20,10};

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> duration = end - start;
    std::cout << "Time taken : " << duration.count() << " s " << std::endl;

    auto start2 = std::chrono::high_resolution_clock::now();

    prova.move();

    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration2 = end2 - start2;
    std::cout << "Time taken : " << duration2.count() << " s " << std::endl;

    auto start3 = std::chrono::high_resolution_clock::now();

    prova.move();

    auto end3 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> duration3 = end3 - start3;
    std::cout << "Time taken : " << duration3.count() << " s " << std::endl;

    auto start4 = std::chrono::high_resolution_clock::now();

    prova.spread();

    auto end4 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> duration4 = end4 - start4;
    std::cout << "Time taken : " << duration4.count() << " s " << std::endl;
*/

    auto start = std::chrono::high_resolution_clock::now();

    Simulation prova{25000 ,3, 7, 4, 5, 400, 300,0.1,0.02,0.3,0.5,20,10};
    for(int k = 0; k < 10; ++k){
        prova.move();
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> duration = end - start;
    std::cout << "Generation and first movement : " << duration.count() << " s " << std::endl;

    std::vector<Data> result{};
    for(int i = 0; i < 4; ++i){
        start = std::chrono::high_resolution_clock::now();
        for(int j = 0; j < 10; ++j){
            prova.move();
            prova.spread();
            prova.update_Condition();
            result.push_back(prova.get_data());
        }
        prova.update_Colors();
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        std::cout << i+1 <<"-nth Cycle : " << duration.count() << " s " << std::endl;
    }

    std::ofstream out{"output.txt"};

    for (auto &a : result)
    {
        out << "S = " << a.S << " E = " << a.E << " I = " << a.I << " R = " << a.R << std::endl;
    }
}