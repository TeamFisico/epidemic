#include "simulation.hpp"
#include "random.hpp"

namespace smooth_sim
{

////////////////////////////////////////////////////////
/////          SIMULATION CONSTRUCTOR             //////
////////////////////////////////////////////////////////
Simulation::Simulation(int S, int E, int I, int R, int number_of_clusters, int number_of_Locations, double Side,
                       double alpha, double gamma, double beta, double spread_radius, int time_in_minutes,
                       int step_in_minutes)
    : sim_engine{},
      world{Side, number_of_clusters, number_of_Locations, S, E, I, R,sim_engine},
      alpha{alpha},
      beta{beta},
      gamma{gamma},
      spread_radius{spread_radius},
      time_in_minutes{time_in_minutes},
      step_in_minutes{step_in_minutes}
{
}
////////////////////////////////////////////////////////
////  PUBLIC METHOD: CLOSE PEOPLE ALL AROUND WORLD  ////
////////////////////////////////////////////////////////
void Simulation::Close_People(Person &current_person, std::vector<Person *> &close_people)
{
    close_people.clear();
    for (unsigned long i = 0; i < world.Clusters().size(); ++i)
    {
        if (world.Clusters()[i].get_color() == Color::Green)
        {
            for (unsigned long j = 0; j < world.Clusters()[i].population().size(); ++j)
            {
                const Position &pos = world.Clusters()[i].population()[j].Person_ref().get_pos();
                if (pos.InRadius(current_person.get_pos(), spread_radius) &&
                    world.Clusters()[i].population()[j].Person_ref().get_current_status() ==
                        Status::Susceptible &&
                    !world.Clusters()[i].population()[j].is_at_home())
                { // check if person is close_enough, Susceptible and not at_home
                    close_people.push_back(
                        &world.Clusters()[i].population()[j].Person_ref()); // push a pointer to
                                                                                                  // the current person
                                                                                                  // back to the end of
                                                                                                  // the vector
                }
            }
        }
    }
}
////////////////////////////////////////////////////////
/////  PUBLIC METHOD: CLOSE PEOPLE ALL AROUND WORLD  ///
////////////////////////////////////////////////////////
void Simulation::Close_Cluster_People(Person &current_person, std::vector<Person *> &close_people)
{
    close_people.clear();
    for (auto &a : world.Clusters()[current_person.get_cluster_index()].population())
    {
        const Position &pos = a.Person_ref().get_pos();
        if (pos.InRadius(current_person.get_pos(), spread_radius) && a.Person_ref().get_current_status() == Status::Susceptible &&
            !a.is_at_home())
        { // check if person is close_enough, Susceptible and not at_home
            close_people.push_back(
                &a.Person_ref()); // push a pointer to the current person back to the end of the vector
        }
    }
}

Data Simulation::get_Cluster_data(int i)
{
    unsigned int nS = 0;
    unsigned int nE = 0;
    unsigned int nI = 0;
    unsigned int nR = 0;
    for (auto &a : world.Clusters()[i].population())
    {
        switch (a.Person_ref().get_current_status())
        {
        case Status::Susceptible:
            nS++;
            break;
        case Status::Exposed:
            nE++;
            break;
        case Status::Infected:
            nI++;
            break;
        case Status::Recovered:
            nR++;
            break;
        }
    }
    return {nS, nE, nI, nR};
}

Data Simulation::get_data()
{
    unsigned int nS{};
    unsigned int nE{};
    unsigned int nI{};
    unsigned int nR{};
    for (auto &c : world.Clusters())
    {
        for (auto &a : c.population())
        {
            switch (a.Person_ref().get_current_status())
            {
            case Status::Susceptible:
                nS++;
                break;
            case Status::Exposed:
                nE++;
                break;
            case Status::Infected:
                nI++;
                break;
            case Status::Recovered:
                nR++;
                break;
            }
        }
    }
    return {nS, nE, nI, nR};
}

//std::vector<Location *> Simulation::green_loc_list()
//{
//    std::vector<Location *> result;
//    for (int i = 0; i < world.Clusters().size(); ++i)
//    {
//        if (world.Clusters()[i].get_color() == Color::Green)
//        { // if cluster is green
//            for (auto a :
//                 world.Clusters()[i].Location_list()) // TODO CHECK IF IS SAFE TO DO IT WITH POINTERS
//            {
//                result.push_back(a);
//            }
//        }
//    }
//    return result;
//}
////////////////////////////////////////////////////////
////       PUBLIC METHOD: UPDATE WORLD ZONES       /////
////////////////////////////////////////////////////////
void Simulation::update_Colors()
{
    for (unsigned long i = 0; i < world.Clusters().size(); ++i)
    {
        Data data = get_Cluster_data(i);
        if (data.I / data.S >= RED_ZONE_CONDITION)
        {
            world.Clusters()[i].set_color(Color::Red);
        }
        else if (data.I / data.S >= YELLOW_ZONE_CONDITION)
        {
            world.Clusters()[i].set_color(Color::Yellow);
        }
        else
        {
            world.Clusters()[i].set_color(Color::Green);
        }
    }
}

void Simulation::spread()
{
    std::vector<Person *> close_people{};
    for (auto &c : world.Clusters())
    {
        for (auto &a : c.population())
        {
            if (a.Person_ref().get_current_status() == Status::Exposed)
            { // if current person is in dormant Status
                if (sim_engine.try_event(alpha)) { a.Person_ref().set_new_status(Status::Infected); }
            }
            else if (a.Person_ref().get_current_status() == Status::Infected)
            {                        // if current person is infected
                if (!a.is_at_home()) // if current person is not at home, then spread the virus
                {
                    if (c.get_color() == Color::Green) // if cluster is Green
                    {
                        Close_People(
                            a.Person_ref(),
                            close_people); // susceptible people in Green Cluster near the infected that are not at home
                    }
                    else // if cluster is Yellow or Red
                    {
                        Close_Cluster_People(a.Person_ref(), close_people); // susceptible people in the same Cluster
                                                                            // near the infected that are not at home
                    }
                    for (auto &b : close_people)
                    {
                        if (sim_engine.try_event(beta)) { b->set_new_status(Status::Exposed); }
                    }
                }
                if (sim_engine.try_event(gamma)) { a.Person_ref().set_new_status(Status::Recovered); }
            }
        }
    }
}

void Simulation::move()
{
    for (auto &c : world.Clusters())
    { // check every person in mobility model in order
        if (c.get_color() == Color::Green)
        {
            std::vector<double> weights(world.Clusters().size(), 0); // weights for every inth cluster
            for (unsigned long i = 0; i < world.Clusters().size(); ++i)
            { // fill the weight fot the various cluster, except for the current
                if (world.Clusters()[i].get_color() == Color::Green)
                {
                    weights[i] = world.Clusters()[i].number_of_locations();
                }
            }
            double sum = std::accumulate(weights.begin(), weights.end(), 0.) -
                         world.Clusters()[c.index()]
                             .number_of_locations(); // sum of all locations in green cluster except current one
            weights[c.index()] =
                sum; // make sure there is a 50 percent probability hat chosen location is from current cluster

            for (auto &a : c.population())
            {
                if (a.is_at_home())
                {
                    if (a.Stay() <= 0)
                    {
                        if (!sim_engine.try_event(a.home_prob()))
                        {                    // check if the person leave home
                            a.not_at_home(); // set the person as not at home
                            world.generate_path(sim_engine.rounded_gauss(4, 1) + 1, weights, a.path(), sim_engine);
                        }
                    }
                    else
                    {
                        --a.Stay();
                    }
                }
                else if (a.at_target_location())
                {
                    if (a.Stay() <= 0)
                    {
                        clean_path(a);
                        a.next_location(sim_engine);
                    }
                    else
                    {
                        --a.Stay();
                    }
                }
                else
                {
                    a.move(sim_engine.uniform(2, 5), sim_engine);
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
                        if (!sim_engine.try_event(a.home_prob()))
                        {                    // check if the person leave home
                            a.not_at_home(); // set the person as not at home
                            // a.path() = sim::generate_path(list, 3, 0.5);
                            c.generate_path(sim_engine.rounded_gauss(2, 0.5) + 1, a.path(), sim_engine);
                        }
                    }
                    else
                    {
                        --a.Stay();
                    }
                }
                else if (a.at_target_location())
                {
                    if (a.Stay() <= 0)
                    {
                        clean_path(a);
                        a.next_location(sim_engine);
                    }
                    else
                    {
                        --a.Stay();
                    }
                }
                else
                {
                    a.move(sim_engine.uniform(2, 5), sim_engine);
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
                        if (!sim_engine.try_event(a.home_prob()))
                        {                    // check if the person leave home
                            a.not_at_home(); // set the person as not at home
                            // a.path() = sim::generate_path(list, 1, 0.2);
                            c.generate_path(sim_engine.rounded_gauss(0, 0.2) + 1, a.path(), sim_engine);
                        }
                    }
                    else
                    {
                        --a.Stay();
                    }
                }
                else if (a.at_target_location())
                {
                    if (a.Stay() <= 0)
                    {
                        clean_path(a);
                        a.next_location(sim_engine);
                    }
                    else
                    {
                        --a.Stay();
                    }
                }
                else
                {
                    a.move(sim_engine.uniform(2, 5), sim_engine);
                }
            }
        }
    }
}

void Simulation::update_Condition()
{
    for (auto &c : world.Clusters())
    {
        for (auto &a : c.population()) // set all the people's condition to new_condition
            a.Person_ref().pass_condition();
    }
}

void Simulation::clean_path(mobility_model &person)
{
    for (unsigned long i = 0; i < person.path().size(); ++i)
    {
        if (world.Clusters()[person.path()[i]->c_index()].get_color() != Color::Green)
        { // access the vector from opposite size, so you check all the elements correctly
            person.path()[i] =
                person.path()[person.path().size() - 1]; // copy the last element of the vector to the current
            person.path().pop_back();                    // delete the last element of a vector
            --i;
        }
    }
}

Position Simulation::person_pos(int cluster_index, int person_index)
{
    return world.Clusters()[cluster_index].population()[person_index].Person_ref().get_pos();
}

bool Simulation::at_home(int cluster_index, int person_index)
{
    return world.Clusters()[cluster_index].population()[person_index].is_at_home();
}

void Simulation::simulate()
{
    // TODO set the variables based on the total simulation time and the time interval of simulation, as the speed of
    // movement, the spreads variables, ecc..
    for (int i = 0; i < time_in_minutes / step_in_minutes; ++i)
    {
        move();   // move all people
        spread(); // spread the virus
        update_Condition();
    }
}

} // namespace smooth_sim