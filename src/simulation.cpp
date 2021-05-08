#include <simulation.hpp>
#include <cassert>
#include <random>

using namespace sim;

//Contructor that generate a random std::vector<Person>
Simulation::Simulation(int S, int E, int I, int R, double alpha, double gamma, double beta, double spread_radius, int time_in_days) {

}
//default costructor
Simulation::Simulation()
{}

std::vector<Person*> Simulation::Close_People(Person& current_person) {
    std::vector<Person*> result;
    result.clear();
    std::vector<Person>* a = &Population;
    for(unsigned int i = 0; i < Population.size(); ++i){
        auto pos = a->operator[](i).get_pos(); //Pointer to the position of the person
        if(pos->InRadius(*current_person.get_pos(), spread_radius) && a->operator[](i).get_condition() == State::S){ // check if person is close_enough and Susceptible
            result.push_back(&(a->operator[](i)));  //push a pointer to the current person back to the end of the vector
        }
    }
    return result;
}

void Simulation::spread() {
    for(auto& a: Population)
    {
        if(a.get_condition() == State::E){ //if current person is in dormant state
            if(try_event((alpha))){
                a.next_condition();
            }
        }
        if(a.get_condition() == State::I){ //if current person is infected
            std::vector<Person*> close_people = Close_People(a); //susceptible people near the infected
            for(auto b: close_people){
                if(try_event((beta))){
                    b->next_condition();
                }
            }
            if(try_event((gamma))){
                a.next_condition();
            }
        }
    }
}

bool sim::try_event(double probability){
    std::random_device rd;
    std::mt19937 gen(rd());
    assert(probability >= 0.0 && probability <= 1.0);
    std::uniform_real_distribution<double> rnum(0.0,1.0);
    return rnum(gen) <= probability;
}
