#include "simulation.hpp"
#include <cassert>
#include "random.hpp"
#include "parameters.hpp"


using namespace sim;

// Contructor that generate a random std::vector<Person>
Simulation::Simulation(int S, int E, int I, int R,int number_of_clusters, int number_of_Locations, double Side, double alpha, double gamma,
                       double beta, double spread_radius, int time_in_minutes, int step_in_minutes)
    :world{Side, number_of_clusters, number_of_Locations, S, E, I, R}, alpha{alpha}, beta{beta}, gamma{gamma}, spread_radius{spread_radius}, time_in_minutes{time_in_minutes}, step_in_minutes{step_in_minutes}
{
}

std::vector<Person *> Simulation::Close_People(Person &current_person)
{
    std::vector<Person *> result;
    result.clear();
    for(int i = 0; i < world.Clusters().size(); ++i){
        if(world.Clusters().operator[](i).get_color()==Color::Green)
        {
            for (int j = 0; j < world.Clusters().operator[](i).population().size(); ++j)
            {
                auto pos = world.Clusters().operator[](i).population().operator[](j).Person_ref().get_pos();
                if (pos.InRadius(current_person.get_pos(), spread_radius) &&
                    world.Clusters().operator[](i).population().operator[](j).Person_ref().get_condition() ==
                        State::S &&
                    !world.Clusters().operator[](i).population().operator[](j).is_at_home())
                { // check if person is close_enough, Susceptible and not at_home
                    result.push_back(
                        &world.Clusters().operator[](i).population().operator[](j).Person_ref()); // push a pointer to the current person back to the end of the vector
                }
            }
        }
    }
    /*for (unsigned int i = 0; i < people.size(); ++i)
    {
        auto pos = people.operator[](i).get_person()->get_pos(); // Pointer to the position of the person
        if (pos->InRadius(*current_person.get_pos(), spread_radius) && people.operator[](i).get_person()->get_condition() == State::S && !people.operator[](i).is_at_home())
        {                                          // check if person is close_enough, Susceptible and not at_home
            result.push_back(people.operator[](i).get_person()); // push a pointer to the current person back to the end of the vector
        }
    }*/
    return result;
}

std::vector<Person *> Simulation::Close_Cluster_People(Person &current_person)
{
    std::vector<Person *> result;
    for (auto & a: world.Clusters().operator[](current_person.get_cluster_index()).population()){
        auto pos = a.Person_ref().get_pos();
        if (pos.InRadius(current_person.get_pos(), spread_radius) && a.Person_ref().get_condition() == State::S && !a.is_at_home())
        {                                          // check if person is close_enough, Susceptible and not at_home
            result.push_back(&a.Person_ref()); // push a pointer to the current person back to the end of the vector
        }
    }
    return result;
}

Data Simulation::get_Cluster_data(int i)
{
    unsigned int nS = 0;
    unsigned int nE = 0;
    unsigned int nI = 0;
    unsigned int nR = 0;
    for(auto & a: world.Clusters().operator[](i).population()){
        switch(a.Person_ref().get_condition()) {
        case State::S:
            nS++;
            break;
        case State::E:
            nE++;
            break;
        case State::I:
            nI++;
            break;
        case State::R:
            nR++;
            break;
        }
    }
    return {nS,nE,nI,nR};
}

Data Simulation::get_data()
{
    unsigned int nS{};
    unsigned int nE{};
    unsigned int nI{};
    unsigned int nR{};
    for (auto& c: world.Clusters())
    {
        for(auto & a: c.population())
        {
            switch (a.Person_ref().get_condition())
            {
            case State::S:
                nS++;
                break;
            case State::E:
                nE++;
                break;
            case State::I:
                nI++;
                break;
            case State::R:
                nR++;
                break;
            }
        }
    }
    return {nS, nE, nI, nR};
}

std::vector<Location *> Simulation::green_loc_list()
{
    std::vector<Location *> result;
    for(int i = 0; i < world.Clusters().size(); ++i){
        if(world.Clusters().operator[](i).get_color() == Color::Green){ //if cluster is green
            for (auto a : world.Clusters().operator[](i).Location_list()) //TODO CHECK IF IS SAFE TO DO IT WITH POINTERS
            {
                result.push_back(a);
            }
        }
    }
    return result;
}

void Simulation::update_Colors()
{
    for(int i = 0; i < world.Clusters().size(); ++i){
        Data data = get_Cluster_data(i);
        if(data.I >= data.S/4){ //condition when a cluster is considered Red, to change
            world.Clusters().operator[](i).get_color() = Color::Red;
        }
        else if(data.I >= data.S/10){ //condition when a cluster is considered Red, to change
            world.Clusters().operator[](i).get_color() = Color::Yellow;
        }
        else{
            world.Clusters().operator[](i).get_color() = Color::Green;
        }
    }
}

void Simulation::spread()
{
    Random rng;
    for(auto& c: world.Clusters())
    {
        for (auto& a : c.population())
        {
            if (a.Person_ref().get_condition() == State::E)
            { // if current person is in dormant state
                if (rng.try_event(alpha))
                {
                    a.Person_ref().get_new_condition() = State::I;
                }
            }
            else if (a.Person_ref().get_condition() == State::I)
            {                        // if current person is infected
                if (!a.is_at_home()) // if current person is not at home, then spread the virus
                {
                    std::vector<Person *> close_people{};
                    if (c.get_color() == Color::Green) //if cluster is Green
                    {
                        close_people = Close_People(a.Person_ref()); // susceptible people in Green Cluster near the infected that are not at home
                    }
                    else //if cluster is Yellow or Red
                    {
                        close_people = Close_Cluster_People(a.Person_ref()); // susceptible people in the same Cluster near the infected that are not at home
                    }
                    for (auto b : close_people)
                    {
                        if (rng.try_event(beta)) { b->get_new_condition() = State::E; }
                    }
                }
                if (rng.try_event(gamma))
                {
                    a.Person_ref().get_new_condition() = State::R;
                }
            }
        }
    }
}

void Simulation::move()
{
    Random rng;
    for(auto& c: world.Clusters())
    { // check every person in mobility model in order
        if (c.get_color() == Color::Green)
        {
            for (auto &a : c.population())
            {
                if (a.is_at_home())
                {
                    if (a.Stay() <= 0)
                    {
                        if (!rng.try_event(a.home_prob()))
                        {                    // check if the person leave home
                            a.not_at_home(); // set the person as not at home
                            if (rng.try_event(0.50))
                            { // select the location from the cluster in which the person is located
                                //a.path() = sim::generate_path(list, 5, 1);
                                a.path() = c.generate_path(5,1);
                            }
                            else
                            { // select from all green locations
                                //a.path() = sim::generate_path(green_list, 5, 1);
                                a.path() = c.generate_path(5,1);
                            }
                        }
                    }
                    else
                    {
                        --a.Stay();
                    }
                }
                else if (a.at_target_location())
                {
                    if (a.Stay() <= 0) {
                        a.next_location();
                    }
                    else{
                        --a.Stay();
                    }
                }
                else{
                    a.move();
                }
            }
        }
        else if (c.get_color() == Color::Yellow)
        {
            for (auto &a : c.population())
            {
                if (a.is_at_home())
                {
                    if (a.Stay() <= 0)
                    {
                        if (!rng.try_event(a.home_prob()))
                        {                    // check if the person leave home
                            a.not_at_home(); // set the person as not at home
                            //a.path() = sim::generate_path(list, 3, 0.5);
                            a.path() = c.generate_path(3,0.5);
                        }
                    }
                    else
                    {
                        --a.Stay();
                    }
                }
                else if (a.at_target_location())
                {
                    if (a.Stay() <= 0) {
                        a.next_location();
                    }
                    else{
                        --a.Stay();
                    }
                }
                else{
                    a.move();
                }
            }
        }
        else if (c.get_color() == Color::Red)
        {
            for (auto &a : c.population())
            {
                if (a.is_at_home())
                {
                    if (a.Stay() <= 0)
                    {
                        if (!rng.try_event(a.home_prob()))
                        {                    // check if the person leave home
                            a.not_at_home(); // set the person as not at home
                            //a.path() = sim::generate_path(list, 1, 0.2);
                            a.path() = c.generate_path(1, 0.2);
                        }
                    }
                    else
                    {
                        --a.Stay();
                    }
                }
                else if (a.at_target_location())
                {
                    if (a.Stay() <= 0) {
                        a.next_location();
                    }
                    else{
                        --a.Stay();
                    }
                }
                else{
                    a.move();
                }
            }
        }
    }

  /*          for (auto &a : c.population())
        {
            if (a.is_at_home())
            { // check if pointed person is at home
                if (a.Stay() <= 0)
                {
                    if (!try_event(a.home_prob()))
                    {                    // check if the person leave home
                        a.not_at_home(); // set the person as not at home
                        if (c.get_color() == Color::Green)
                        { // TODO change location_list to a reference
                            if (try_event(0.50))
                            { // select the location from the cluster in which the person is located
                                a.path() =
                                    sim::generate_path(c.Location_list(), 5, 1);
                            }
                            else
                            { // select from all locations
                                a.path() = sim::generate_path(green_loc_list(), 5, 1);
                            }
                        }
                        else if (c.get_color() == Color::Yellow)
                        {
                            a.path() =
                                sim::generate_path(c.Location_list(), 3, 0.5);
                        }
                        else if (c.get_color() == Color::Red)
                        {
                            a.path() =
                                sim::generate_path(c.Location_list(), 1, 0.2);
                        }
                    }
                }
                else
                {
                    --a.Stay();
                }
            }
            else if (a.at_target_location())
            {
                a.next_location();
            }
            else
            {
                a.move();
            }
        }
    }*/
}

void Simulation::update_Condition()
{
    for(auto &c : world.Clusters()){
        for(auto &a : c.population())//set all the people's condition to new_condition
            a.Person_ref().pass_condition();
    }
}

void Simulation::simulate()
{
    //TODO set the variables based on the total simulation time and the time interval of simulation, as the speed of movement, the spreads variables, ecc..
    for(int i = 0; i < time_in_minutes/step_in_minutes; ++i)
    {
        move();   // move all people
        spread(); // spread the virus
        update_Condition();
    }
}

