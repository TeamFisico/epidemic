#include <simulation.hpp>

//Contructor that generate a random std::vector<Person>
Simulation::Simulation(int S, int E, int I, int R, double alpha, double gamma, double beta, double spread_radius, int time_in_days) {

}
//default costructor
Simulation::Simulation()
{}

std::vector<Person*> Simulation::Close_Persons(Person& current_person) {
    std::vector<Person*> result;
    result.clear();
    for(auto& a: Population){
        if(current_person.pos.InRadius(spread_radius)){ // check if person is close_enough
            result.push_back(&a);  //push pointer to the person at the end of the vector (probably wrong code)
        }
    }
    return result;
}

void Simulation::spread() {
    
}
