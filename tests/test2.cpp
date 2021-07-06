#include "simulation.hpp"
#include <iostream>
#include "doctest.h"

using namespace smooth_sim;

TEST_CASE("Path cleaning test")
{
    Simulation Test(10000, 0, 0, 0,10,1000,1000,0.1,0.1,0.1,1,100);
    for(unsigned int i = 0; i < 10; ++i) //move ten times
    {
        Test.move();
    }
    //Change the color of 5 clusters to Yellow
    for(unsigned int i = 0; i < 10; i += 2){
        Test.world().clusters()[i].set_zone(Zone::Yellow);
    }
    //Clean the path based on cluster color
    for(auto& cl: Test.world().clusters()){
        for(auto& person: cl.people()){
            if(Test.world().clusters()[person.cluster_index()].get_zone() == Zone::Green){
                Test.clean_path(person); //To make public for testing
            }
            else{
                Test.clean_cluster_path(person); //TODO add this function from the other branch
            }
        }
    }

    int counter = 0;
    for(auto& cl: Test.world().clusters()){
        for(auto& person: cl.people()){
            if(Test.world().clusters()[person.cluster_index()].get_zone() == Zone::Green){ //if the person is in a green cluster
                for(auto& loc: person.path()){
                    if(Test.world().clusters()[loc->get_cluster_index()].get_zone() != Zone::Green){ //check if all location are valid
                        ++counter;
                    }
                }
            }
            else{ //if the person is not in a green cluster
                for(auto& loc: person.path()){
                    if(loc->get_cluster_index() != person.cluster_index()){ //check if all location are valid
                        ++counter;
                    }
                }
            }
        }
    }

    CHECK(counter == 0);
}