#include "world.hpp"
#include <iostream>
using namespace sim;

int main(){
    World prova{1000,5,1000,10000,323,433,542};
    mobility_model& per = prova.Clusters()[0].population()[0];
    std::vector<double> weights(5,1);
    Random rng;
    prova.generate_path(5,1,weights,per.path(),rng);
    for(auto& a: per.path()){
        std::cout << "X = " << a->get_pos().get_x() << " Y = " << a->get_pos().get_x() << std::endl;
    }
    return 0;
}