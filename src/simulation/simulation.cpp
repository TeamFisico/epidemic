#include "simulation.hpp"
#include "parameters.hpp"
#include "random.hpp"
#include <cassert>

namespace smooth_sim
{
// Contructor that generate a random std::vector<Person>
Simulation::Simulation(int S, int E, int I, int R, int number_of_clusters, int number_of_Locations, double Side,
                       double alpha, double gamma, double beta, double spread_radius, int time_in_minutes,
                       int step_in_minutes)
    : world{Side, number_of_clusters, number_of_Locations, S, E, I, R},
      alpha{alpha},
      beta{beta},
      gamma{gamma},
      spread_radius{spread_radius},
      time_in_minutes{time_in_minutes},
      step_in_minutes{step_in_minutes}
{
}

void Simulation::Close_People(Person &current_person, std::vector<Person *> &close_people)
{
    close_people.clear();
    for (int i = 0; i < world.Clusters().size(); ++i)
    {
        if (world.Clusters().operator[](i).get_color() == Color::Green)
        {
            for (int j = 0; j < world.Clusters().operator[](i).population().size(); ++j)
            {
                const Position &pos = world.Clusters().operator[](i).population().operator[](j).Person_ref().get_pos();
                if (pos.InRadius(current_person.get_pos(), spread_radius) &&
                    world.Clusters().operator[](i).population().operator[](j).Person_ref().get_condition() ==
                        State::S &&
                    !world.Clusters().operator[](i).population().operator[](j).is_at_home())
                { // check if person is close_enough, Susceptible and not at_home
                    close_people.push_back(
                        &world.Clusters().operator[](i).population().operator[](j).Person_ref()); // push a pointer to
                                                                                                  // the current person
                                                                                                  // back to the end of
                                                                                                  // the vector
                }
            }
        }
    }
    /*for (unsigned int i = 0; i < people.size(); ++i)
    {
        auto pos = people.operator[](i).get_person()->get_pos(); // Pointer to the position of the person
        if (pos->InRadius(*current_person.get_pos(), spread_radius) &&
    people.operator[](i).get_person()->get_condition() == State::S && !people.operator[](i).is_at_home()) { // check if
    person is close_enough, Susceptible and not at_home result.push_back(people.operator[](i).get_person()); // push a
    pointer to the current person back to the end of the vector
        }
    }*/
}

void Simulation::Close_Cluster_People(Person &current_person, std::vector<Person *> &close_people)
{
    close_people.clear();
    for (auto &a : world.Clusters().operator[](current_person.get_cluster_index()).population())
    {
        const Position &pos = a.Person_ref().get_pos();
        if (pos.InRadius(current_person.get_pos(), spread_radius) && a.Person_ref().get_condition() == State::S &&
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
    for (auto &a : world.Clusters().operator[](i).population())
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
    for (int i = 0; i < world.Clusters().size(); ++i)
    {
        if (world.Clusters().operator[](i).get_color() == Color::Green)
        { // if cluster is green
            for (auto a :
                 world.Clusters().operator[](i).Location_list()) // TODO CHECK IF IS SAFE TO DO IT WITH POINTERS
            {
                result.push_back(a);
            }
        }
    }
    return result;
}

void Simulation::update_Colors()
{
    for (int i = 0; i < world.Clusters().size(); ++i)
    {
        Data data = get_Cluster_data(i);
        if (data.I >= data.S / 4)
        { // condition when a cluster is considered Red, to change
            world.Clusters().operator[](i).get_color() = Color::Red;
        }
        else if (data.I >= data.S / 10)
        { // condition when a cluster is considered Red, to change
            world.Clusters().operator[](i).get_color() = Color::Yellow;
        }
        else
        {
            world.Clusters().operator[](i).get_color() = Color::Green;
        }
    }
}

void Simulation::spread()
{
    Random rng;
    std::vector<Person *> close_people{};
    for (auto &c : world.Clusters())
    {
        for (auto &a : c.population())
        {
            if (a.Person_ref().get_condition() == State::E)
            { // if current person is in dormant state
                if (rng.try_event(alpha)) { a.Person_ref().get_new_condition() = State::I; }
            }
            else if (a.Person_ref().get_condition() == State::I)
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
                        if (rng.try_event(beta)) { b->get_new_condition() = State::E; }
                    }
                }
                if (rng.try_event(gamma)) { a.Person_ref().get_new_condition() = State::R; }
            }
        }
    }
}

void Simulation::move()
{
    Random rng;
    for (auto &c : world.Clusters())
    { // check every person in mobility model in order
        if (c.get_color() == Color::Green)
        {
            std::vector<double> weights(world.Clusters().size(), 0); // weights for every inth cluster
            for (int i = 0; i < world.Clusters().size(); ++i)
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
                        if (!rng.try_event(a.home_prob()))
                        {                    // check if the person leave home
                            a.not_at_home(); // set the person as not at home
                            /*if (rng.try_event(0.50))
                            { // select the location from the cluster in which the person is located
                                //a.path() = sim::generate_path(list, 5, 1);
                                a.path() = c.generate_path(5,1,rng);
                            }
                            else
                            { // select from all green locations
                                //a.path() = sim::generate_path(green_list, 5, 1);
                                a.path() = c.generate_path(5,1,rng);
                            }*/
                            world.generate_path(rng.rounded_gauss(4, 1) + 1, weights, a.path(), rng);
                            // c.generate_path(rng.rounded_gauss(4,1)+1,a.path(),rng);
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
                        a.next_location(rng);
                    }
                    else
                    {
                        --a.Stay();
                    }
                }
                else
                {
                    a.move(rng.uniform(2, 5), rng);
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
                            // a.path() = sim::generate_path(list, 3, 0.5);
                            c.generate_path(rng.rounded_gauss(2, 0.5) + 1, a.path(), rng);
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
                        a.next_location(rng);
                    }
                    else
                    {
                        --a.Stay();
                    }
                }
                else
                {
                    a.move(rng.uniform(2, 5), rng);
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
                            // a.path() = sim::generate_path(list, 1, 0.2);
                            c.generate_path(rng.rounded_gauss(0, 0.2) + 1, a.path(), rng);
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
                        a.next_location(rng);
                    }
                    else
                    {
                        --a.Stay();
                    }
                }
                else
                {
                    a.move(rng.uniform(2, 5), rng);
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
    for (auto &c : world.Clusters())
    {
        for (auto &a : c.population()) // set all the people's condition to new_condition
            a.Person_ref().pass_condition();
    }
}

void Simulation::clean_path(mobility_model &person)
{
    /*for(auto& a: person.path()){
        if(world.Clusters()[a->c_index()].get_color() != Color::Green){
            //delete the last element of the vector to make sure it does not move the vector
            a = *(person.path().end() - 1); //copy the last element of the vector to the current
            person.path().pop_back(); //delete the last element of a vector
        }
    }*/
    for (int i = 0; i < person.path().size(); ++i)
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