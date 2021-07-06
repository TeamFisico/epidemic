#include "simulation.hpp"
#include "random.hpp"

namespace smooth_sim
{

////////////////////////////////////////////////////////
/////          SIMULATION CONSTRUCTOR             //////
////////////////////////////////////////////////////////
Simulation::Simulation(int S, int E, int I, int R, int number_of_clusters, int number_of_Locations, double Side,
                       double alpha, double gamma, double beta, double spread_radius, int time_in_minutes)
    : sim_engine{},
      wrld{Side, number_of_clusters, number_of_Locations, S, E, I, R},
      alpha{alpha},
      beta{beta},
      gamma{gamma},
      spread_radius{spread_radius},
      time_in_minutes{time_in_minutes}
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////           PRIVATE METHODS           /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////// MOVE STEP FOR PEOPLE IN WHITE CLUSTERS /////////////////
void Simulation::move_white(Cluster& cluster, std::vector<double>& weights_v)
{
    for (auto& p : cluster.people())
    {
        if (p.is_at_home())
        {
            if (p.stay_time() <= 0)
            {
                if (!sim_engine.try_event(p.home_prob())) // determine if person leaves home
                {
                    p.set_is_at_home(false); // set the person as not at home
                    wrld.generate_path(sim_engine.rounded_gauss(WHITE_PATH_MEAN, WHITE_PATH_STDDEV) + 1, weights_v,
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
                p.next_location(cluster.get_LATP(),sim_engine);
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
///////////////// MOVE STEP FOR PEOPLE IN YELLOW CLUSTERS /////////////////
void Simulation::move_yellow(Cluster& cluster)
{
    for (auto& p : cluster.people())
    {
        if (p.is_at_home())
        {
            if (p.stay_time() <= 0)
            {
                if (!sim_engine.try_event(p.home_prob()))
                {                            // check if the person leave home
                    p.set_is_at_home(false); // set the person as not at home
                    // a.path() = sim::generate_path(list, 3, 0.5);
                    cluster.generate_path(sim_engine.rounded_gauss(YELLOW_PATH_MEAN, YELLOW_PATH_STDDEV) + 1, p.path());
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
                p.next_location(cluster.get_LATP(),sim_engine);
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
///////////////// MOVE STEP FOR PEOPLE IN RED CLUSTERS /////////////////
void Simulation::move_red(Cluster& cluster)
{
    for (auto& p : cluster.people())
    {
        if (p.is_at_home())
        {
            if (p.stay_time() <= 0)
            {
                if (!sim_engine.try_event(p.home_prob()))
                {                            // check if the person leave home
                    p.set_is_at_home(false); // set the person as not at home
                    // a.path() = sim::generate_path(list, 1, 0.2);
                    cluster.generate_path(sim_engine.rounded_gauss(RED_PATH_MEAN, RED_PATH_STDDEV) + 1, p.path());
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
                p.next_location(cluster.get_LATP(),sim_engine);
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
///////////////// CLOSE PEOPLE /////////////////
void Simulation::close_people(Person& current_person, std::vector<Person*>& close_people)
{
    close_people.clear();
    for (auto& cl : wrld.Clusters)
    {
        if (cl.get_zone() == Zone::Green)
        {
            for (auto& p : cl.people())
            {
                Position const& pos = p.person().get_position();
                if (pos.in_radius(current_person.get_position(), spread_radius) &&
                    p.person().get_current_status() == Status::Susceptible &&
                    !p.is_at_home()) // check if person is close_enough, Susceptible and not at_home
                {
                    close_people.push_back(&p.person()); // push a pointer to the current person back
                }
            }
        }
    }
}
///////////////// CLOSE PEOPLE INSIDE CLUSTER /////////////////
void Simulation::close_cluster_people(Person& current_person, std::vector<Person*>& close_people_v)
{
    close_people_v.clear();
    for (auto& a : wrld.Clusters[current_person.get_label()].people())
    {
        const Position& pos = a.person().get_position();
        if (pos.in_radius(current_person.get_position(), spread_radius) &&
            a.person().get_current_status() == Status::Susceptible && !a.is_at_home())
        { // check if person is close_enough, Susceptible and not at_home
            close_people_v.push_back(
                &a.person()); // push a pointer to the current person back to the end of the vector
        }
    }
}

///////////////// UPDATE PEOPLE STATUS /////////////////
void Simulation::update_people_status()
{
    for (auto& cl : wrld.Clusters)
    {
        for (auto& p : cl.people()) // set all the people's condition to new_condition
        {
            p.person().update_status();
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////           PUBLIC METHODS            /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////// MOVE WHITE /////////////////
World& Simulation::world()
{
    return wrld;
}
World Simulation::get_world() const
{
    return wrld;
}

Data Simulation::get_Cluster_data(int i)
{
    unsigned int nS = 0;
    unsigned int nE = 0;
    unsigned int nI = 0;
    unsigned int nR = 0;
    for (auto& a : wrld.Clusters[i].get_people())
    {
        switch (a.get_person().get_current_status())
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
    for (auto& c : wrld.Clusters)
    {
        for (auto& a : c.get_people())
        {
            switch (a.get_person().get_current_status())
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

///////////////// UPDATE ZONES /////////////////
void Simulation::update_zones()
{
    for (unsigned long i = 0; i < wrld.Clusters.size(); ++i)
    {
        Data data = get_Cluster_data(i);
        if (static_cast<double>(data.I) / static_cast<double>(data.S) >= RED_ZONE_CONDITION)
        {
            wrld.Clusters[i].set_zone(Zone::Red);
            wrld.Clusters[i].set_LATP(RED_ZONE_LATP_ALPHA);
        }
        else if (static_cast<double>(data.I) / static_cast<double>(data.S) >= YELLOW_ZONE_CONDITION)
        {
            wrld.Clusters[i].set_zone(Zone::Yellow);
            wrld.Clusters[i].set_LATP(YELLOW_ZONE_LATP_ALPHA);
        }
        else
        {
            wrld.Clusters[i].set_zone(Zone::Green);
            wrld.Clusters[i].set_LATP(WHITE_ZONE_LATP_ALPHA);
        }
    }
}
///////////////// SPREAD /////////////////
void Simulation::spread()
{
    std::vector<Person*> close_people_v{};
    for (auto& cl : wrld.Clusters)
    {
        for (auto& p : cl.people()) // p is the current person
        {
            if (p.person().get_current_status() == Status::Exposed)
            { // if current person is in dormant Status
                if (sim_engine.try_event(alpha)) { p.person().set_new_status(Status::Infected); }
            }
            else if (p.person().get_current_status() == Status::Infected)
            {                        // if current person is infected
                if (!p.is_at_home()) // if current person is not at home, then spread the virus
                {
                    if (cl.get_zone() == Zone::Green) // if cluster is Green
                    {
                        // susceptible people in Green Cluster near the infected that are not at home
                        close_people(p.person(), close_people_v);
                    }
                    else // if cluster is Yellow or Red
                    {
                        // susceptible people in the same Cluster near the infected that are not at home
                        close_cluster_people(p.person(), close_people_v);
                    }
                    for (auto& b : close_people_v)
                    {
                        if (sim_engine.try_event(beta)) // determine infection based on beta parameter
                        {
                            b->set_new_status(Status::Exposed);
                        }
                    }
                }
                if (sim_engine.try_event(gamma)) // determine recovery based on gamma parameter
                {
                    p.person().set_new_status(Status::Recovered);
                }
            }
        }
    }
    update_people_status(); // pass new status for everyone
}
///////////////// MOVE /////////////////
void Simulation::move()
{
    for (auto& cluster : wrld.Clusters)
    {
        if (cluster.get_zone() == Zone::Green)
        {
            std::vector<double> weights(wrld.Clusters.size(), 0); // weights for every inth cluster
            // fill the weight fot the various clusters, except for the current
            for (unsigned long i = 0; i < wrld.Clusters.size(); ++i)
            {
                if (wrld.Clusters[i].get_zone() == Zone::Green)
                {
                    weights[i] = wrld.Clusters[i].locations_num();
                }
            }
            // get the sum of all locations in green cluster except current one
            double current =  wrld.Clusters[cluster.get_label()].locations_num();
            double sum = std::accumulate(std::begin(weights), std::end(weights), 0.) - current;
            // make sure there is a 50 percent probability hat chosen location is from current cluster
            weights[cluster.get_label()] = sum;
            move_white(cluster, weights);
        }
        else if (cluster.get_zone() == Zone::Yellow)
        {
            move_yellow(cluster);
        }
        else if (cluster.get_zone() == Zone::Red)
        {
            move_red(cluster);
        }
    }
}
///////////////// CLEAN PATH OF A MODELED PERSON /////////////////
void Simulation::clean_path(Mobility_model& person)
{
    for (unsigned long i = 0; i < person.path().size(); ++i)
    {
        if (wrld.Clusters[person.path()[i]->get_label()].get_zone() != Zone::Green)
        {
            // access the vector from opposite size, so to check all the elements correctly since erasing element
            // copy the last element of the vector to the current
            person.path()[i] = person.path()[person.path().size() - 1];
            person.path().pop_back();
            --i;
        }
    }
}
///////////////// PERFORM THE SIMULATION /////////////////
void Simulation::simulate()
{
    // TODO set the variables based on the total simulation time and the time interval of simulation, as the speed of
    // movement, the spreads variables, ecc..
    for (int i = 0; i < time_in_minutes / STEP_TIME; ++i)
    {
        move();   // move all people
        spread(); // spread the virus
        update_people_status();
    }
}

/////////////////// FUNCTIONS USED FOR TESTING  /////////////////////////////////////////
Position Simulation::get_person_pos(int cluster_label, int person_index)
{
    return wrld.Clusters[cluster_label].people()[person_index].person().get_position();
}

bool Simulation::is_person_at_home(int cluster_index, int person_index)
{
    return wrld.Clusters[cluster_index].people()[person_index].is_at_home();

}
//////////////////////////////////////////////////////////////////////////////////////////


} // namespace smooth_sim