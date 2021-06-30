#include "simulation.hpp"
#include <fstream>
#include <iostream>
#include <string>
using namespace smooth_simulation;


std::string st(Person p)
{
    switch (p.current_status())
    {
    case Status::Exposed:
        return {"Exposed"};
    case Status::Infected:
        return{"Infected"};
    case Status::Susceptible:
        return {"Susceptible"};
    case Status::Dead:
         return {"Dead"};
    }
}

int main()
{
    using namespace smooth_simulation;

    using namespace std::literals::chrono_literals;

    Data starting_data{POPULATION_SIZE - 140, 100, 40, 0, 0, 0};

    Simulation sim{1000, 1.0, 0.1, 0.5, 0.02, 0.1, starting_data};


    std::ofstream o {"prova.txt"};
    Random eng;

    sim.simulate();


//
//
//    for (auto& p: sim.People)
//    {
//        assert(p.get_stay() == 0);
//        if(p.current_status() == Status::Infected)
//        {
//            std::cout << "\t\t\t\tI-->Clusters"<<p.home_cluster()<<std::endl;
//        }
//        else if (p.current_status() == Status::Recovered)
//        {
//            std::cout << "\t\t\t\t\t\t\t\t\t\tR-->Clusters"<<p.home_cluster()<<std::endl;
//
//        }
//        else if(p.current_status() == Status::Exposed)
//        {
//            std::cout << "E-->Clusters"<<p.home_cluster()<<std::endl;
//        }
//    }
//    Person& p1 = sim.People[eng.int_uniform(0,POPULATION_SIZE-1)]; //take track
//    o << "STATUS " << st(p1)<<std::endl;
//    o << "POSITION == " << p1.get_position().get_X() << "\t" << p1.get_position().get_Y() << std::endl;
//    o << "PATH SIZE == " << p1.path_size() << "\tAt home = " << p1.is_at_home() << std::endl;
//    o << "CURRENT TARGET POSITION" << sim.Waypoints[p1.get_target_i()].get_X() << "\t";
//    o << sim.Waypoints[p1.get_target_i()].get_Y() << std::endl;
//
//    for (int j = 1; j < 100; ++j)
//    {
//
//    for (int i = 0; i < UPDATE_ZONES_INTERVAL; ++i)
//    {
//        auto start = std::chrono::high_resolution_clock::now();
//        sim.move();
//        sim.spread();
//        auto end = std::chrono::high_resolution_clock::now();
//
//        o << "STATUS " << st(p1)<<std::endl;
//        o << "POSITION == " << p1.get_position().get_X() << "\t" << p1.get_position().get_Y() << std::endl;
//        o << "PATH SIZE == " << p1.path_size() << "\tAt home = " << p1.is_at_home() << std::endl;
//        o << "TARGET " << sim.Waypoints[p1.get_target_i()].get_X() << "\t";
//        o << sim.Waypoints[p1.get_target_i()].get_Y() << std::endl;
//        std::chrono::duration<float> duration = end - start;
//        std::cout << "Block " << j << "   Step " <<i<<"--->" << duration.count() << " s " << std::endl;
//    }
//
//      sim.update_data();
//      sim.update_zones();
//      Data d = sim.get_simulation_data();
////      std::cout <<"S == " << d.S <<"E == " << d.E <<"I == " << d.I <<"R == " << d.R <<"D == " << d.D <<std::endl;
//    }




}