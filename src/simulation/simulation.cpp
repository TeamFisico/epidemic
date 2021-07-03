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
      world{Side, number_of_clusters, number_of_Locations, S, E, I, R, sim_engine},
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
    for (auto &cl : world.Clusters())
    {
        if (cl.get_color() == Color::Green)
        {
            for (auto &p : cl.population())
            {
                Position const &pos = p.person_ref().get_pos();
                if (pos.InRadius(current_person.get_pos(), spread_radius) &&
                    p.person_ref().get_current_status() == Status::Susceptible &&
                    !p.is_at_home()) // check if person is close_enough, Susceptible and not at_home
                {
                    close_people.push_back(&p.person_ref()); // push a pointer to the current person back
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
    for (auto &a : world.Clusters()[current_person.get_label()].population())
    {
        const Position &pos = a.person_ref().get_pos();
        if (pos.InRadius(current_person.get_pos(), spread_radius) &&
            a.person_ref().get_current_status() == Status::Susceptible && !a.is_at_home())
        { // check if person is close_enough, Susceptible and not at_home
            close_people.push_back(
                &a.person_ref()); // push a pointer to the current person back to the end of the vector
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
        switch (a.person_ref().get_current_status())
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
            switch (a.person_ref().get_current_status())
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

// std::vector<Location *> Simulation::green_loc_list()
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
        if (data.I / data.S >= RED_ZONE_CONDITION) { world.Clusters()[i].set_color(Color::Red); }
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
            if (a.person_ref().get_current_status() == Status::Exposed)
            { // if current person is in dormant Status
                if (sim_engine.try_event(alpha)) { a.person_ref().set_new_status(Status::Infected); }
            }
            else if (a.person_ref().get_current_status() == Status::Infected)
            {                        // if current person is infected
                if (!a.is_at_home()) // if current person is not at home, then spread the virus
                {
                    if (c.get_color() == Color::Green) // if cluster is Green
                    {
                        Close_People(
                            a.person_ref(),
                            close_people); // susceptible people in Green Cluster near the infected that are not at home
                    }
                    else // if cluster is Yellow or Red
                    {
                        Close_Cluster_People(a.person_ref(), close_people); // susceptible people in the same Cluster
                                                                            // near the infected that are not at home
                    }
                    for (auto &b : close_people)
                    {
                        if (sim_engine.try_event(beta)) { b->set_new_status(Status::Exposed); }
                    }
                }
                if (sim_engine.try_event(gamma)) { a.person_ref().set_new_status(Status::Recovered); }
            }
        }
    }
}

void Simulation::move_white(Cluster &cluster, std::vector<double> &weights)
{
    for (auto &p : cluster.population())
    {
        if (p.is_at_home())
        {
            if (p.stay_time() <= 0)
            {
                if (!sim_engine.try_event(p.home_prob()))
                {                    // check if the person leave home
                    p.not_at_home(); // set the person as not at home
                    world.generate_path(sim_engine.rounded_gauss(WHITE_PATH_MEAN, WHITE_PATH_STDDEV) + 1, weights,
                                        p.path(), sim_engine);
                }
            }
            else
            {
                p.decrease_stay();
            }
        }
        else if (p.at_target_location())
        {
            if (p.stay_time() <= 0)
            {
                clean_path(p);
                p.next_location(sim_engine);
            }
            else
            {
                p.decrease_stay();
            }
        }
        else
        {
            p.move(sim_engine.rand_speed(), sim_engine);
        }
    }
}
void Simulation::move_yellow(Cluster &cluster)
{
    for (auto &p : cluster.population())
    {
        if (p.is_at_home())
        {
            if (p.stay_time() <= 0)
            {
                if (!sim_engine.try_event(p.home_prob()))
                {                    // check if the person leave home
                    p.not_at_home(); // set the person as not at home
                    // a.path() = sim::generate_path(list, 3, 0.5);
                    cluster.generate_path(sim_engine.rounded_gauss(YELLOW_PATH_MEAN, YELLOW_PATH_STDDEV) + 1, p.path(),
                                          sim_engine);
                }
            }
            else
            {
                p.decrease_stay();
            }
        }
        else if (p.at_target_location())
        {
            if (p.stay_time() <= 0)
            {
                clean_path(p);
                p.next_location(sim_engine);
            }
            else
            {
                p.decrease_stay();
            }
        }
        else
        {
            p.move(sim_engine.rand_speed(), sim_engine);
        }
    }
}
void Simulation::move_red(Cluster &cluster)
{
    for (auto &p : cluster.population())
    {
        if (p.is_at_home())
        {
            if (p.stay_time() <= 0)
            {
                if (!sim_engine.try_event(p.home_prob()))
                {                    // check if the person leave home
                    p.not_at_home(); // set the person as not at home
                    // a.path() = sim::generate_path(list, 1, 0.2);
                    cluster.generate_path(sim_engine.rounded_gauss(RED_PATH_MEAN, RED_PATH_STDDEV) + 1, p.path(),
                                          sim_engine);
                }
            }
            else
            {
                p.decrease_stay();
            }
        }
        else if (p.at_target_location())
        {
            if (p.stay_time() <= 0)
            {
                clean_path(p);
                p.next_location(sim_engine);
            }
            else
            {
                p.decrease_stay();
            }
        }
        else
        {
            p.move(sim_engine.rand_speed(), sim_engine);
        }
    }
}

void Simulation::move()
{
    for (auto &cluster : world.Clusters())
    {
        if (cluster.get_color() == Color::Green)
        {
            std::vector<double> weights(world.Clusters().size(), 0); // weights for every inth cluster
            for (unsigned long i = 0; i < world.Clusters().size();
                 ++i) // fill the weight fot the various cluster, except for the current
            {
                if (world.Clusters()[i].get_color() == Color::Green)
                {
                    weights[i] = world.Clusters()[i].number_of_locations();
                }
            }
            double sum = std::accumulate(weights.begin(), weights.end(), 0.) -
                         world.Clusters()[cluster.index()]
                             .number_of_locations(); // sum of all locations in green cluster except current one
            weights[cluster.index()] =
                sum; // make sure there is a 50 percent probability hat chosen location is from current cluster
            move_white(cluster, weights);
        }
        else if (cluster.get_color() == Color::Yellow)
        {
            move_yellow(cluster);
        }
        else if (cluster.get_color() == Color::Red)
        {
            move_red(cluster);
        }
    }
}

void Simulation::update_people_status()
{
    for (auto &c : world.Clusters())
    {
        for (auto &a : c.population()) // set all the people's condition to new_condition
        {
            a.person_ref().update_status();
        }
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
    return world.Clusters()[cluster_index].population()[person_index].person_ref().get_pos();
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
        update_people_status();
    }
}

} // namespace smooth_sim