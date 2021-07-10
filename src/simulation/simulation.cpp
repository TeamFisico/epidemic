#include "simulation.hpp"
#include "random.hpp"

namespace smooth_sim
{

////////////////////////////////////////////////////////
/////          SIMULATION CONSTRUCTOR             //////
////////////////////////////////////////////////////////
Simulation::Simulation(int S, int E, int I, int R, int number_of_clusters, int number_of_Locations, int side,
                       double alpha, double beta, double gamma, double spread_radius)
    : sim_engine{},
      wrld{side, number_of_clusters, number_of_Locations, S, E, I, R},
      alpha{alpha},
      beta{beta},
      gamma{gamma},
      spread_radius{spread_radius}
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
        // Check if person is at home
        if (p.is_at_home())
        {
            // Check the stay_time
            if (p.stay_time() <= 0)
            {
                // Determine if person leaves home
                if (!sim_engine.try_event(p.home_prob()))
                {
                    // Set the person as not at home
                    p.set_is_at_home(false);
                    // Generate path
                    wrld.generate_path(sim_engine.rounded_gauss(GREEN_PATH_MEAN, GREEN_PATH_STDDEV) + 1, weights_v,
                                       p.path(), sim_engine);
                }
            }
            else
            {
                p.decrease_stay();
            }
        }
        // If not check if person is at target location
        else if (p.at_target_location())
        {
            // Checks stay, and if necessary select next location
            if (p.stay_time() <= 0) { p.next_location(cluster.get_LATP(), sim_engine); }
            else
            {
                p.decrease_stay();
            }
        }
        // If not then move
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
        // Check if person is at home
        if (p.is_at_home())
        {
            // Check the stay_time
            if (p.stay_time() <= 0)
            {
                // Determine if person leaves home
                if (!sim_engine.try_event(p.home_prob()))
                {
                    // Set the person as not at home
                    p.set_is_at_home(false);
                    // Generate path
                    cluster.generate_cluster_path(sim_engine.rounded_gauss(YELLOW_PATH_MEAN, YELLOW_PATH_STDDEV) + 1,
                                                  p.path());
                }
            }
            else
            {
                p.decrease_stay();
            }
        }
        // If not check if person is at target location
        else if (p.at_target_location())
        {
            // Checks stay, and if necessary select next location
            if (p.stay_time() <= 0) { p.next_location(cluster.get_LATP(), sim_engine); }
            else
            {
                p.decrease_stay();
            }
        }
        // If not then move
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
        // Check if person is at home
        if (p.is_at_home())
        {
            // Check the stay_time
            if (p.stay_time() <= 0)
            {
                // Determine if person leaves home
                if (!sim_engine.try_event(p.home_prob()))
                {
                    // Set the person as not at home
                    p.set_is_at_home(false);
                    // Generate path
                    cluster.generate_cluster_path(sim_engine.rounded_gauss(RED_PATH_MEAN, RED_PATH_STDDEV) + 1,
                                                  p.path());
                }
            }
            else
            {
                p.decrease_stay();
            }
        }
        // If not check if person is at target location
        else if (p.at_target_location())
        {
            // Checks stay, and if necessary select next location
            if (p.stay_time() <= 0) { p.next_location(cluster.get_LATP(), sim_engine); }
            else
            {
                p.decrease_stay();
            }
        }
        // If not then move
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
    // Check every person from every cluster
    for (auto& cl : wrld.Clusters)
    {
        if (cl.get_zone() == Zone::Green)
        {
            for (auto& p : cl.people())
            {
                Position const& pos = p.person().get_position();
                // Check if person is close_enough, Susceptible and not at_home
                if (pos.in_radius(current_person.get_position(), spread_radius) &&
                    p.person().get_current_status() == Status::Susceptible && !p.is_at_home())
                {
                    // Add a pointer to the current to the vector
                    close_people.push_back(&p.person());
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
        // Check if person is close_enough, Susceptible and not at_home
        if (pos.in_radius(current_person.get_position(), spread_radius) &&
            a.person().get_current_status() == Status::Susceptible && !a.is_at_home())
        {
            // Add a pointer to the current to the vector
            close_people_v.push_back(&a.person());
        }
    }
}

///////////////// UPDATE PEOPLE STATUS /////////////////
void Simulation::update_people_status()
{
    for (auto& cl : wrld.Clusters)
    {
        for (auto& p : cl.people())
        {
            // Set all the people's condition to new_condition
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

Data Simulation::get_Cluster_data(int i)
{
    int nS = 0;
    int nE = 0;
    int nI = 0;
    int nR = 0;
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
    int nS{};
    int nE{};
    int nI{};
    int nR{};
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
        // Get data from the cluster
        Data data = get_Cluster_data(i);
        double LATP = wrld.Clusters[i].get_LATP();
        // Check if cluster need to become Red
        if (static_cast<double>(data.I) / static_cast<double>(data.S) >= RED_ZONE_CONDITION)
        {
            // Change cluster color and cluster LATP parameter
            wrld.Clusters[i].set_zone(Zone::Red);
            wrld.Clusters[i].set_LATP(RED_ZONE_LATP_ALPHA);
            // Clean the path and check target location validity
            for (auto& person : wrld.Clusters[i].people())
            {
                clean_cluster_path(person);
                // If target location is not valid, select next location
                if (person.get_target_location()->get_label() != static_cast<int>(i))
                {
                    person.next_location(LATP, sim_engine);
                }
            }
        }
        // Check if cluster need to become Yellow
        else if (static_cast<double>(data.I) / static_cast<double>(data.S) >= YELLOW_ZONE_CONDITION)
        {
            // Change cluster color and cluster LATP parameter
            wrld.Clusters[i].set_zone(Zone::Yellow);
            wrld.Clusters[i].set_LATP(YELLOW_ZONE_LATP_ALPHA);
            // Clean the path and check target location validity
            for (auto& person : wrld.Clusters[i].people())
            {
                clean_cluster_path(person);
                // If target location is not valid, select next location
                if (person.get_target_location()->get_label() != static_cast<int>(i))
                {
                    person.next_location(LATP, sim_engine);
                }
            }
        }
        // Or if cluster need to become Green
        else
        {
            // Change cluster color and cluster LATP parameter
            wrld.Clusters[i].set_zone(Zone::Green);
            wrld.Clusters[i].set_LATP(WHITE_ZONE_LATP_ALPHA);
            // Clean the path and check target location validity
            for (auto& person : wrld.Clusters[i].people())
            {
                clean_path(person);
                // If target location is not valid, select next location
                if (wrld.Clusters[person.get_target_location()->get_label()].get_zone() != Zone::Green)
                {
                    person.next_location(LATP, sim_engine);
                }
            }
        }
    }
}
///////////////// SPREAD /////////////////
void Simulation::spread()
{
    std::vector<Person*> close_people_v{};
    for (auto& cl : wrld.Clusters)
    {
        // p is the current person
        for (auto& p : cl.people())
        {
            // If current person is in Exposed Status
            if (p.person().get_current_status() == Status::Exposed)
            {
                // Check if it becomes Infected
                if (sim_engine.try_event(alpha)) { p.person().set_new_status(Status::Infected); }
            }
            // If current person is infected
            else if (p.person().get_current_status() == Status::Infected)
            {
                // If current person is not at home, then spread the virus
                if (!p.is_at_home())
                {
                    // If cluster is Green
                    if (cl.get_zone() == Zone::Green)
                    {
                        // Susceptible people in Green Cluster near the infected that are not at home
                        close_people(p.person(), close_people_v);
                    }
                    // If cluster is Yellow or Red
                    else
                    {
                        // Susceptible people in the same Cluster near the infected that are not at home
                        close_cluster_people(p.person(), close_people_v);
                    }
                    // Check every person in close people vector
                    for (auto& b : close_people_v)
                    {
                        // Check if it becomes Exposed
                        if (sim_engine.try_event(beta)) { b->set_new_status(Status::Exposed); }
                    }
                }
                // Check if it becomes Recovered
                if (sim_engine.try_event(gamma)) { p.person().set_new_status(Status::Recovered); }
            }
        }
    }
    // Pass new status for everyone
    update_people_status();
}
///////////////// MOVE /////////////////
void Simulation::move()
{
    for (auto& cluster : wrld.Clusters)
    {
        // Check if cluster is Green
        if (cluster.get_zone() == Zone::Green)
        {
            // Vector which will hold the weights for every i-nth cluster
            std::vector<double> weights(wrld.Clusters.size(), 0);
            // Fill the weight vector for the various clusters, except for the current cluster
            for (unsigned long i = 0; i < wrld.Clusters.size(); ++i)
            {
                // Check if cluster is Green
                if (wrld.Clusters[i].get_zone() == Zone::Green) { weights[i] = wrld.Clusters[i].locations_num(); }
            }
            // Get the sum of all locations in green cluster except current one
            double current = wrld.Clusters[cluster.get_label()].locations_num();
            double sum = std::accumulate(std::begin(weights), std::end(weights), 0.) - current;
            // Make sure there is a 50 percent probability that the chosen location is from current cluster, by making
            // the weight of the current cluster the sum of the other clusters weight
            weights[cluster.get_label()] = sum;
            // Move
            move_white(cluster, weights);
        }
        // Check if cluster is Yellow
        else if (cluster.get_zone() == Zone::Yellow)
        {
            // Move
            move_yellow(cluster);
        }
        // Check if cluster is Yellow
        else if (cluster.get_zone() == Zone::Red)
        {
            // Move
            move_red(cluster);
        }
    }
}
///////////////// CLEAN PATH OF A PERSON IN GREEN CLUSTER /////////////////
void Simulation::clean_path(Mobility_model& person)
{
    for (unsigned long i = 0; i < person.path().size(); ++i)
    {
        // Check if the location is in a Green Cluster
        if (wrld.Clusters[person.path()[i]->get_label()].get_zone() != Zone::Green)
        {
            // Copy the last element of the vector to the current
            person.path()[i] = person.path()[person.path().size() - 1];
            // Eliminate the last vector
            person.path().pop_back();
            // Check the moved element
            --i;
        }
    }
}
///////////////// CLEAN PATH OF A PERSON IN A YELLOW OR RED CLUSTER /////////////////
void Simulation::clean_cluster_path(Mobility_model& person)
{
    for (unsigned long i = 0; i < person.path().size(); ++i)
    {
        // Check if the location is in the same cluster iun which the person reside
        if (person.path()[i]->get_label() !=
            person.get_label()) // check if the location is in the same cluster iun which the person reside
        {
            // Copy the last element of the vector to the current
            person.path()[i] = person.path()[person.path().size() - 1];
            // Eliminate the last vector
            person.path().pop_back();
            // Check the moved element
            --i;
        }
    }
}

} // namespace smooth_sim