#include "simulation.hpp"
#include <fstream>
#include <iostream>

int main()
{
    using namespace smooth_simulation;

    using namespace std::literals::chrono_literals;

    Data starting_data{POPULATION_SIZE - 62, 50, 10, 2, 0, 0};

    Simulation sim{1000, 0.5, 0.1, 0.5, 0.02, 0.1, starting_data};

    auto start = std::chrono::high_resolution_clock::now();

    //    sim.simulate();
    Random rng;
    std::ofstream out{"out.txt"};
    int p_index = rng.int_uniform(0, POPULATION_SIZE - 1); // track 1 person movements
    Person& person = sim.People[p_index];
    std::cout << person.get_position().get_X() << "\t" << person.get_position().get_Y() << std::endl;
    std::cout << "Is home?-->";
    if(person.at_home()) {std::cout <<"yes"<< std::endl;}
    else std::cout<<"no"<<std::endl;
          for (int i = 0; i < 5*UPDATE_ZONES_INTERVAL;++i)
          {
              Location& target = sim.Waypoints[person.get_target_i()];
              sim.move();
              out << "Step " << i<<std::endl;
              out << person.get_position().get_X() << "\t" << person.get_position().get_Y()<<std::endl;
              out << "Is home?-->";
              if(person.at_home()) {out <<"yes"<< std::endl;}
              else out<<"no"<<std::endl;
              out <<"target positio == " <<target.get_position().get_X() << "\t"<< target.get_Y()<<std::endl;
              out << "Path size == " << person.path_size()<<std::endl;
          }

//    for (auto& p  : sim.People)
//    {
//        out << p.get_position().get_X() << "\t" << p.get_position().get_Y() << std::endl;
//        out << p.get_home().get_position().get_X() << p.get_home().get_position().get_Y()<<std::endl;
//        out << "Is home?-->";
//        if(p.at_home()) {out <<"yes"<< std::endl;}
//        else out<<"no"<<std::endl;
//    }
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> duration = end - start;
    std::cout << "Time taken: " << duration.count() << " s " << std::endl;
}