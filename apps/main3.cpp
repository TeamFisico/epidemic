#include "simulation.hpp"
#include <fstream>
#include <iostream>


int main()
{
    using namespace smooth_simulation;

    using namespace std::literals::chrono_literals;

    Data starting_data{POPULATION_SIZE - 62, 50, 10, 2, 0, 0};

    Simulation sim{1000, 0.5, 0.1, 0.5, 0.02, 0.1, starting_data};


    std::ofstream o {"prova.txt"};
    Random eng;

    for (auto& p: sim.People)
    {
        assert(p.get_stay() == 0);
    }
    Person& p1 = sim.People[eng.int_uniform(0,POPULATION_SIZE-1)]; //take track

    o  << "Person position == "<<p1.get_position().get_X() << "\t" << p1.get_position().get_Y() << std::endl;
    o << "Path size == " << p1.path_size() << "\tAt home = " << p1.is_at_home()<<std::endl;
    o << "Target location " << sim.Waypoints[p1.get_target_i()].get_X()<<"\t";
    o << sim.Waypoints[p1.get_target_i()].get_Y()<<std::endl;
    int initial_path = p1.path_size();

    for (int i = 0; i < 100*UPDATE_ZONES_INTERVAL; ++i)
    {
        auto start = std::chrono::high_resolution_clock::now();
        sim.move();
        auto end = std::chrono::high_resolution_clock::now();
        if (p1.path_size() < initial_path) {
            o << "AAAAAAAAAAAAARRRRRRIVVVVVEEEEEEEDDDD" << std::endl;
            --initial_path;
        }
        o  << "Person position == "<<p1.get_position().get_X() << "\t" << p1.get_position().get_Y() << std::endl;
        o << "Path size == " << p1.path_size() << "\tAt home = " << p1.is_at_home()<<std::endl;
        o << "Target location " << sim.Waypoints[p1.get_target_i()].get_X()<<"\t";
        o << sim.Waypoints[p1.get_target_i()].get_Y()<<std::endl;
        std::chrono::duration<float> duration = end - start;
        std::cout << "Step " <<i << "-->"<< duration.count() << " s " << std::endl;


        }




}