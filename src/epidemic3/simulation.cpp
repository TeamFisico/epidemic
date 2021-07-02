#include "simulation.hpp"
#include <fstream>
#include <iostream>



std::vector<double> total_spread{};
std::vector<double> spread_infectedd{};
std::vector<double> spread_exposedd{};
std::vector<double> close_people{};
std::vector<double> close_people_cluster{};
int times_close_people_called;
int times_close_cluster_people_called;
int times_total_spread_called;
int times_spread_infected_called;
int times_spread_exposed_called;

namespace smooth_simulation
{
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////      BENCHMARKING CLASSES                ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class ClosePeople
{
  private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
  public:
    ClosePeople()
    {
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }
    void Stop()
    {
        auto endTimepoint = std::chrono::high_resolution_clock::now();
        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

        auto duration = end - start;
        double ms = duration * 0.001;
        ++times_close_people_called;
        close_people.push_back(ms);
    }
    ~ClosePeople()
    {
        Stop();
    }
};

class SpreadTimer
{
  private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
  public:
    SpreadTimer()
    {
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }
    void Stop()
    {
        auto endTimepoint = std::chrono::high_resolution_clock::now();
        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

        auto duration = end - start;
        double ms = duration * 0.001;
        ++times_total_spread_called;
        total_spread.push_back(ms);
    }
    ~SpreadTimer()
    {
        Stop();
    }
};
class SpreadExposed
{
  private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
  public:
    SpreadExposed()
    {
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }
    void Stop()
    {
        auto endTimepoint = std::chrono::high_resolution_clock::now();
        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

        auto duration = end - start;
        double ms = duration * 0.001;
        ++times_spread_exposed_called;
        spread_exposedd.push_back(ms);
    }
    ~SpreadExposed()
    {
        Stop();
    }
};
class SpreadInfected
{
  private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
  public:
    SpreadInfected()
    {
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }
    void Stop()
    {
        auto endTimepoint = std::chrono::high_resolution_clock::now();
        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

        auto duration = end - start;
        double ms = duration * 0.001;
        ++times_spread_infected_called;
        spread_infectedd.push_back(ms);
    }
    ~SpreadInfected()
    {
        Stop();
    }
};
class ClosePeopleCluster
{
  private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
  public:
    ClosePeopleCluster()
    {
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }
    void Stop()
    {
        auto endTimepoint = std::chrono::high_resolution_clock::now();
        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

        auto duration = end - start;
        double ms = duration * 0.001;
        ++times_close_cluster_people_called;
        close_people_cluster.push_back(ms);
    }
    ~ClosePeopleCluster()
    {
        Stop();
    }
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////    WORLD CREATION  ///////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// initialising static data members of the class
std::array<Location, WAYPOINTS_SIZE> Simulation::Waypoints;
std::array<Cluster, CLUSTERS_SIZE> Simulation::Clusters;
std::array<Person, POPULATION_SIZE> Simulation::People;
/////////////////////////////////////////////////////
////////          AREA PARTITIONING           ///////
/////////////////////////////////////////////////////
void Simulation::partition_in_clusters()
{
    int total_sizes = 0;
    double wpts_left = WAYPOINTS_SIZE;
    double still_to_choose = CLUSTERS_SIZE;
    int index = 0;

    for (int i = 0; i < 100; ++i)
    {
        double mean_size = wpts_left / still_to_choose;
        // 68% will be in a 25% change range
        int current = engine.rounded_gauss(mean_size, mean_size / 4);

        assert(current >= 0.0);
        if (still_to_choose == 1) // the last will take the remaining size --> avoid too many more loops
        {
            Clusters[CLUSTERS_SIZE - 1].set_size((int)wpts_left);
            Clusters[CLUSTERS_SIZE - 1].set_label(CLUSTERS_SIZE - 1);
            Clusters[index].set_weight((double)wpts_left / WAYPOINTS_SIZE);
            return;
        }

        if (total_sizes + current < WAYPOINTS_SIZE)
        {
            Clusters[index].set_size(current);
            Clusters[index].set_label(index); // label the group with its index(from 0 to C-1)
            Clusters[index].set_weight((double)current /
                                       WAYPOINTS_SIZE); // set the weight to be be chosen by a random person
            total_sizes += current;                     // add this cluster's size to the counter
            --still_to_choose;
            wpts_left -= current;
            ++index;
        }
    }
}
/////////////////////////////////////////////////////
////////   FIRST GROUP PLOT HELPER FUNCTIONS  ///////
/////////////////////////////////////////////////////
// return true if no waypoints of this group and a trial location are under transmission range
// return false otherwise
bool check_group(Group const& group, Position try_position)
{
    for (int i = 0; i < group.size(); ++i)
    {
        if (group.pointed_waypoint()[i].get_position().in_radius(try_position, TRANSMISSION_RANGE)) { return false; }
    }
    return true;
}

bool check_cluster(Cluster const& cluster, Position try_position)
{
    for (auto const& group : cluster.Groups)
    {
        if (!check_group(group, try_position)) return false;
    }
    return true;
}
// check up to Clusters[label-1] returning true if all the clusters are okay, false other wise
bool check_labeled_clusters(int cl_label, Position try_position)
{
    for (int i = 0; i < cl_label; ++i)
    {
        if (!check_cluster(Simulation::Clusters[i], try_position)) return false;
    }
    return true;
}
/////////////////////////////////////////////////////
////////    1st WAYPOINT OF THE GROUP PLOT    ///////
/////////////////////////////////////////////////////
Location Simulation::first_group_step(int clust_lab)
// Plot the 1st waypoint of the group so that it's not in the transmission range
// of any already plotted waypoint
{
    Position try_position{engine.uniform(0.0, side), engine.uniform(0.0, side)};

    if (clust_lab == 0) // this is the first group of the first cluster
    {
        return Location{try_position, clust_lab};
    }
    else
    { // if 1 <= label <= C-1
        while (true)
        {
            if (check_labeled_clusters(clust_lab, try_position)) // no other wpts are under trans range
            {
                return Location{try_position, clust_lab};
            }
            else // try with another one
            {
                try_position = {engine.uniform(0.0, side), engine.uniform(0.0, side)};
            }
        }
    }
}
/////////////////////////////////////////////////////
////////     NEIGHBOURING WAYPOINTS PLOT      ///////
/////////////////////////////////////////////////////
// Plot the other waypoints of this group in such a way that their distance in never < TRANSMISSION_RANGE / 10
// the return value is the last plotted waypoint of this group
Location Simulation::plot_nearby_waypoints(int clust_lab, int grp_lab, Location const& starting_waypoint)
{
    int num_to_plot = Clusters[clust_lab].Groups[grp_lab].size();
    Position try_position{};
    Position current_center{};
    std::vector<Position> already_setted_wpts{starting_waypoint.get_position()};

    for (int i = 0; i < num_to_plot; ++i)
    {
        current_center = engine.engine().pick(already_setted_wpts); // randomly pick a center position
        try_position = generate_close_position(current_center, TRANSMISSION_RANGE / 10.0,engine);

        for (unsigned int j = 0; j < already_setted_wpts.size(); ++j) // check already setted waypoints
        {
            if (try_position.distance_to(already_setted_wpts[j]) >
                TRANSMISSION_RANGE / 10.0) // if it doesn't respect the condition
            {
                try_position = generate_close_position(current_center, TRANSMISSION_RANGE / 10.0,engine);
                j = 0; // generate a new position and restart the loop
                continue;
            }
        } // end inner for

        already_setted_wpts.push_back(try_position); // keep track of the setted position
        // now set the Location in the corresponding place in Waypoints array
        Clusters[clust_lab].Groups[grp_lab].pointed_waypoint()[i] = Location{try_position, clust_lab};

    }                                                                               // end for
    return Clusters[clust_lab].Groups[grp_lab].pointed_waypoint()[num_to_plot - 1]; // last plotted wpt
}
/////////////////////////////////////////////////////
////////  1st WAYPOINT OF OTHER GROUPS PLOT   ///////
/////////////////////////////////////////////////////
// plot the first waypoint of the current group within a distance Y/4 <= d <= Y/3
Location Simulation::other_groups_step(Location const& prev_group_waypoint, int clust_lab)
{
    double const Y = 2 * side / CLUSTERS_SIZE;

    // generate waypoints in a square of side Y/3 (resize if out of bounds)
    double lower_x = prev_group_waypoint.get_X() - Y / 3;
    double upper_x = prev_group_waypoint.get_X() + Y / 3;
    double lower_y = prev_group_waypoint.get_Y() - Y / 3;
    double upper_y = prev_group_waypoint.get_Y() + Y / 3;

    if (lower_x < 0.0) lower_x = 0.0;
    if (lower_y < 0.0) lower_y = 0.0;
    if (upper_x > side) upper_x = side;
    if (upper_y > side) upper_y = side;

    while (true)
    {
        Position try_position{engine.uniform(lower_x, upper_x), engine.uniform(lower_y, upper_y)};
        double distance = try_position.distance_to(prev_group_waypoint.get_position());

        if (distance >= Y / 4 && distance <= Y / 3) { return Location{try_position, clust_lab}; }
    } // end while
}
/////////////////////////////////////////////////////
////////             FINAL PLOT              ////////
/////////////////////////////////////////////////////
void Simulation::plot_waypoints()
{
    for (int cl_index = 0; cl_index < CLUSTERS_SIZE; ++cl_index)
    {
        // set the first waypoint associated with the first group
        Clusters[cl_index].Groups[0].pointed_waypoint()[0] = first_group_step(cl_index);
        // plot all the waypoints of the group around this one and return a reference to the last one
        Location previous_group_last_waypoint =
            plot_nearby_waypoints(cl_index, 0, Clusters[cl_index].Groups[0].pointed_waypoint()[0]);

        for (unsigned int gr_index = 1; gr_index < Clusters[cl_index].Groups.size();
             ++gr_index) // now set the other groups' wpts
        {
            // setting the first waypoint of this group
            Clusters[cl_index].Groups[gr_index].pointed_waypoint()[0] =
                other_groups_step(previous_group_last_waypoint, cl_index);
            // plot the neighbourhood and set the la
            previous_group_last_waypoint =
                plot_nearby_waypoints(cl_index, gr_index, Clusters[cl_index].Groups[gr_index].pointed_waypoint()[0]);
        }
    }
}
/////////////////////////////////////////////////////
////////          CLUSTER ASSIGNMENT          ///////
/////////////////////////////////////////////////////
void Simulation::assign_cluster_to_people()
// assign a person to a cluster based on the cluster weight using piecewise-const-dist
{
    std::vector<double> weights; // fill with clusters weights

    weights.reserve(CLUSTERS_SIZE);
    for (int i = 0; i < CLUSTERS_SIZE; ++i)
    {
        weights.push_back(Clusters[i].weight()); // fill with i-th weight
    }

    int p_index = 0;
    for (auto& person : People)
    {
        int label = engine.discrete(weights); // extract a number n 0<=n<= CLUSTERS_SIZE -1 -->label
        person.set_cluster_label(label);
        Clusters[label].add_person_i(p_index);
        ++p_index;
    }
}
/////////////////////////////////////////////////////
////////          HOME ASSIGNMENT             ///////
/////////////////////////////////////////////////////
// generate home address inside the relating cluster limits and assign them to families; set that as initial person's position
void Simulation::assign_home_to_people()

{
    double lw_x = 0.0;
    double up_x = 0.0;
    double lw_y = 0.0;
    double up_y = 0.0;
    int family_size = 0;
    int people_left = POPULATION_SIZE;
    std::vector<Position> generated_homes;
    generated_homes.reserve(POPULATION_SIZE / 5); // will need at least this space

    for (int i = 0; i < POPULATION_SIZE; i += family_size) // loop over
    {
        Person& person = People[i];
        // set the bounds based on the belonging cluster for home generation
        lw_x = Clusters[person.label].lower_x();
        up_x = Clusters[person.label].upper_x();
        lw_y = Clusters[person.label].lower_y();
        up_y = Clusters[person.label].upper_y();

        family_size = engine.rounded_gauss(AVERAGE_FAMILY_SIZE, 1.5); // a family as a mean
        if (family_size < 1) family_size = 1;

        double x = engine.uniform(lw_x, up_x);
        double y = engine.uniform(lw_y, up_y);
        Position try_pos{x, y};

        // if necessary generate a new position until its a one not already taken by another family
        for (unsigned int i = 0; i < generated_homes.size(); ++i)
        {
            if (try_pos == generated_homes[i])
            {
                try_pos = {engine.uniform(lw_x, up_x), engine.uniform(lw_y, up_y)}; // try a new one
                i = 0;
                continue; // restart the loop
            }
        }
        Location gen_home{try_pos, HOME_RADIUS}; // home to assign

        if (people_left <= 5) // set the last ones as a family
        {
            for (int j = i; j < POPULATION_SIZE; ++j)
            {
                People[j].set_home(gen_home);
                People[j].set_at_home();
            }
        }
        else
        {
            int k = 0; // take trace of the already setted homes
            for (int j = i; k < family_size; ++j)
            {
                People[j].set_home(gen_home);
                People[j].set_at_home();
                ++k;
            }
            people_left -= family_size;         // decrease left people
            generated_homes.push_back(try_pos); // take trace of the setted position
        }
    } // end loop over People
}
/////////////////////////////////////////////////////
////   DISTRIBUTE I,E,R people over the map     /////
/////////////////////////////////////////////////////
// each person is default constructed to be Susceptible, so this function distributes uniformly E,I,R people over
// clusters. It does that by setting blocks of people_block people randomly over the clusters
void Simulation::initialize_people_status(int E, int I, int R)
{
    int  people_block = 10;   //number of people assigned together to a cluster(if possible)
    int chosen_label = 0;
    //TODO if E,I,R are high(altough very unlikely to be allowed) choose a small people_block
    if (E+I+R >= 1000){ people_block = 4; }

    std::array<int,CLUSTERS_SIZE> E_people{};
    int E_people_left = E;
    while(E_people_left > people_block)
    {
        chosen_label = engine.int_uniform(0, CLUSTERS_SIZE - 1);
        //make sure the corresponding cluster has enough People available to set
        while(Clusters[chosen_label].population_size() < (E_people[chosen_label] + people_block) )
        {
            chosen_label = engine.int_uniform(0, CLUSTERS_SIZE - 1);
        }
        E_people[chosen_label] += people_block;
        E_people_left -= people_block;
    }
    chosen_label = engine.int_uniform(0, CLUSTERS_SIZE - 1);
    //make sure the corresponding cluster has enough People available to set
    while(Clusters[chosen_label].population_size() < (E_people[chosen_label] + E_people_left))
    {
        chosen_label = engine.int_uniform(0, CLUSTERS_SIZE - 1);
    }
    E_people[chosen_label]+= E_people_left;

    //I individuals
    std::array<int,CLUSTERS_SIZE> I_people{};
    int I_people_left = I;
    while(I_people_left > people_block)
    {
        chosen_label = engine.int_uniform(0, CLUSTERS_SIZE - 1);
        //make sure the corresponding cluster has enough People available to set
        while(Clusters[chosen_label].population_size() < (E_people[chosen_label] + I_people[chosen_label] + people_block))
        {
            std::cout << "retrying\n";
            chosen_label = engine.int_uniform(0, CLUSTERS_SIZE - 1);
        }

        I_people[chosen_label] += people_block;
        I_people_left -= people_block;
    }
    //set the remainder
    chosen_label = engine.int_uniform(0, CLUSTERS_SIZE - 1);
    //make sure the corresponding cluster has enough People available to set
    while(Clusters[chosen_label].population_size() < (E_people[chosen_label] + I_people[chosen_label] + I_people_left ))
    {
        chosen_label = engine.int_uniform(0, CLUSTERS_SIZE - 1);
    }
    I_people[chosen_label] += I_people_left;

    //R Individuals
    std::array<int,CLUSTERS_SIZE> R_people{};
    int R_people_left = R;
    while(R_people_left > people_block)
    {
        chosen_label = engine.int_uniform(0, CLUSTERS_SIZE - 1);
        //make sure the corresponding cluster has enough People available to set
        while(Clusters[chosen_label].population_size() < (E_people[chosen_label] + I_people[chosen_label] + R_people[chosen_label] + people_block))
        {
            chosen_label = engine.int_uniform(0, CLUSTERS_SIZE - 1);
        }
        R_people[chosen_label] += people_block;
        R_people_left -= people_block;
    }
    //set the remainder
    chosen_label = engine.int_uniform(0, CLUSTERS_SIZE - 1);
    //make sure the corresponding cluster has enough People available to set
    while(Clusters[chosen_label].population_size() < (E_people[chosen_label] + I_people[chosen_label] + R_people[chosen_label] + R_people_left))
    {
        chosen_label = engine.int_uniform(0, CLUSTERS_SIZE - 1);
    }
    R_people[chosen_label] += R_people_left;

    //now setting E,I,R status to the selected people through cluster.People_i
    for (int cl_index = 0; cl_index < CLUSTERS_SIZE;++cl_index)
    {
        Cluster& curr_cluster = Clusters[cl_index];
        int E_people_to_set = E_people[cl_index];
        int I_people_to_set = I_people[cl_index];
        int R_people_to_set = R_people[cl_index];
        int starting_index = 0;

        for (int i = 0; i < E_people_to_set; ++i)
        {
            People[curr_cluster.People_i[i]].set_current_status(Status::Exposed);
            People[curr_cluster.People_i[i]].set_new_status(Status::Exposed);
        }
        //starting_index people have already been setted in this cluster People_i, so start from starting_index + 1
        starting_index = E_people_to_set;
        for (int i = starting_index; i < I_people_to_set + starting_index; ++i)
        {
            People[curr_cluster.People_i[i]].set_current_status(Status::Infected);
            People[curr_cluster.People_i[i]].set_new_status(Status::Infected);
        }
        //starting_index people have already been setted in this cluster People_i, so start from starting_index + 1
        starting_index = E_people_to_set + I_people_to_set;
        for (int i = starting_index; i < R_people_to_set + starting_index; ++i)
        {
            People[curr_cluster.People_i[i]].set_current_status(Status::Recovered);
            People[curr_cluster.People_i[i]].set_new_status(Status::Recovered);
        }
    }
}
/////////////////////////////////////////////////////
////////         WORLD GENERATION             ///////
/////////////////////////////////////////////////////
void Simulation::world_generation()
{
    partition_in_clusters();
    for (auto& cluster : Clusters)
    {
        cluster.partition_in_groups(engine);
    }

    plot_waypoints(); // plot the waypoints on the map

    int i = 1;
    for (auto& cluster : Clusters) // set (x,y) limits for each cluster
    {
        cluster.set_limits();
        std::cout << "Cluster "<<i<<" size == "<<cluster.size()<<std::endl;
        ++i;
    }

    assign_cluster_to_people();

    assign_home_to_people();

    set_clusters_bounds_indeces();

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////     SIMULATION     ///////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////
////////       SIMULATION CONSTRUCTOR         ///////
/////////////////////////////////////////////////////
Simulation::Simulation(double side, double spread_radius, double alpha, double beta, double gamma, double kappa,
                       Data data)
       : side{side},
         spread_radius{spread_radius},
         alpha{alpha},
         beta{beta},
         gamma{gamma},
         kappa{kappa},
         data{data},
         engine{}
{
    world_generation();
    std::cout << "World successfully constructed!" << std::endl;

    initialize_people_status(data.E,data.I,data.R);
    std::cout <<"initialised EIR"<<std::endl;

}
/////////////////////////////////////////////////////
////       CLOSE PEOPLE ALL OVER THE AREA        ////
/////////////////////////////////////////////////////
void Simulation::close_people_fill(const Person& current_person, std::vector<int>& close_people_i)
{
    ///////// BENCHMARKING ///////////
    ClosePeople c{};
    //////////////////////////////////
    close_people_i.clear();
    for (auto& cl : Clusters) // loop over clusters
    {
        if (cl.zone_type() == Zone::White) // current cluster is white
        {
            for (int& person_i : cl.People_i) // loop over people in this cluster indeces
            {
                Person& person = People[person_i]; // ref to current cluster person
                auto const& pos = person.position;
                auto const& stat = person.current_status();
                bool const& is_home = person.at_home;
                // check if person is close enough(most restrictive condition),susceptible and not at home
                if (pos.in_radius(current_person.position, spread_radius) && stat == Status::Susceptible && !is_home)
                {
                    close_people_i.push_back(person_i); // ok, add this person index to close people vector
                }
            } // end loop
        }
        // if not white-->ignore
    }
}
/////////////////////////////////////////////////////
////       CLOSE PEOPLE IN PERSON CLUSTER        ////
/////////////////////////////////////////////////////
void Simulation::close_cluster_people_fill(const Person& current_person, std::vector<int>& close_people_i)
{
    ///////// BENCHMARKING ///////////
    ClosePeopleCluster c{};
    //////////////////////////////////
    close_people_i.clear();
    for (int& person_i : Clusters[current_person.label].People_i) // loop over people in this person's cluster indeces
    {
        Person& person = People[person_i]; // ref to current cluster person
        auto const& pos = person.position;
        auto const& stat = person.current_status();
        bool const& is_home = person.at_home;
        // check if person is close enough(most restrictive condition),susceptible and not at home
        if (pos.in_radius(current_person.position, spread_radius) && stat == Status::Susceptible && !is_home)
        {
            close_people_i.push_back(person_i); // ok, add this person index to close people vector
        }
    } // end loop
}
/////////////////////////////////////////////////////
////////            MOVE PEOPLE               ///////
/////////////////////////////////////////////////////
void Simulation::move(bool is_first_movement)
{
    for (auto& cl : Clusters)
    {
        cl.move(is_first_movement,engine);
    }
}
/////////////////////////////////////////////////////
/////    SPREAD STEP FOR EXPOSED PEOPLE         /////
/////////////////////////////////////////////////////
void Simulation::spread_exposed(Person& person)
{
    SpreadExposed s{};
    // determine if this person will be able to be infected
    if (engine.try_event(alpha))
    {
        person.set_new_status(Status::Infected);
    }
}
/////////////////////////////////////////////////////
/////    SPREAD STEP FOR INFECTED PEOPLE         /////
/////////////////////////////////////////////////////
void Simulation::spread_infected(Person& person,std::vector<int>& close_people_indeces_v)
{
    /////////// BENCHMARKING ///
    SpreadInfected s{};
    /////////////////////////////
    if (!person.at_home) // the person is not at home
    {
        if (Clusters[person.label].zone_type() == Zone::White) // the cluster is white
        {
            close_people_fill(person, close_people_indeces_v); // fill with close ppl indeces
        }
        else // the cluster is either yellow,orange or red
        {
            close_cluster_people_fill(person, close_people_indeces_v);
        }
        for (int& close_i : close_people_indeces_v) // loop over close people
        {
            Person& close_person = People[close_i]; // ref to current close person
            if (engine.try_event(beta))
            {
                close_person.set_new_status(Status::Exposed);
            } // the close one's are exposed
        }
    }
    if (engine.try_event(gamma))
    {
        person.set_new_status(Status::Recovered);
    } // determine if the person will recover
    else if (engine.try_event(kappa))
    {
        person.set_new_status(Status::Dead);
    } // determine if the person will
}
/////////////////////////////////////////////////////
/////    UPDATE CLUSTER ZONES BASED ON DATA     /////
/////////////////////////////////////////////////////
void Simulation::update_zones()
{
    for (auto& cl : Clusters)
    {
        double ratio{(double)cl.data.I / cl.data.S}; // infected - suceptible ratio

        if (ratio < WHITE_CLUSTER_RATIO)
        {
            cl.set_zone(Zone::White);
            cl.set_LATP_parameter(WHITE_LATP_PARAMETER);
        }
        else if (ratio > WHITE_CLUSTER_RATIO && ratio <= YELLOW_CLUSTER_RATIO)
        {
            cl.set_zone(Zone::Yellow);
            cl.set_LATP_parameter(YELLOW_LATP_PARAMETER);
        }
        else if (ratio > YELLOW_CLUSTER_RATIO && ratio <= ORANGE_CLUSTER_RATIO)
        {
            cl.set_zone(Zone::Orange);
            cl.set_LATP_parameter(ORANGE_LATP_PARAMETER);
        }
        else if (ratio > ORANGE_CLUSTER_RATIO) // red condition
        {
            cl.set_zone(Zone::Red);
            cl.set_LATP_parameter(RED_LATP_PARAMETER);
        }
    }
}
/////////////////////////////////////////////////////
////////       UPDATE PEOPLE STATUS           ///////
/////////////////////////////////////////////////////
void Simulation::update_people_status()
{
    for (auto& p : People)
    {
       p.update_status();
    }
}

/////////////////////////////////////////////////////
///////             UPDATE DATA               ///////
/////////////////////////////////////////////////////
// updates cluster data and consequently simulation summary data
void Simulation::update_data()
{
    data.S = 0;
    data.E = 0;
    data.I = 0;
    data.R = 0;
    data.D = 0;
    for (auto& cl : Clusters)
    {
        cl.update_data();
        data.S += cl.data.S;
        data.E += cl.data.E;
        data.I += cl.data.I;
        data.R += cl.data.R;
        data.D += cl.data.D;
    }
}
void Simulation::clear_dead_people()
{
    for (auto& cl : Clusters)
    {
        cl.clear_dead_people();
    }
}
/////////////////////////////////////////////////////
///////               GET DATA                ///////
/////////////////////////////////////////////////////
Data Simulation::get_simulation_data() const
{
    return data;
}
/////////////////////////////////////////////////////
///////          SPREAD THE DISEASE           ///////
/////////////////////////////////////////////////////
void Simulation::spread()
{
    SpreadTimer s{};
    std::vector<int> close_people_i; // contains close people indeces

    for (auto& cl : Clusters) // loop over clusters
    {
        for (int& person_i : cl.People_i) // loop over indeces of people of this cluster
        {
            auto& person = People[person_i]; // ref to current person

            switch (person.current_status())
            {
            case Status::Exposed:
                spread_exposed(person);
                break;
            case Status::Infected:
                spread_infected(person,close_people_i);
                break;
            default:
                break;
            }
        }  // end loop over cluster's people
    }  // end loop over clusters
    //now updating people status for the ones who changed
    update_people_status();
}
/////////////////////////////////////////////////////
///////               SIMULATE                ///////
/////////////////////////////////////////////////////
void Simulation::simulate()
{
    using namespace std::literals::chrono_literals;
    std::vector<Data> simulation_d;
    std::ofstream out{"out.txt"};
    for (int i = 0; i < 10; ++i)
    {
        auto start = std::chrono::high_resolution_clock::now();
        for (int steps = 1; steps < UPDATE_ZONES_INTERVAL; ++steps) // do 1 block
        {
            move(false);
            spread();
        }
        auto end = std::chrono::high_resolution_clock::now();
        update_data();
        update_zones();
//        clear_dead_people();
        simulation_d.push_back(data);
        std::chrono::duration<float> duration = end - start;
        std::cout << "Block"<<i<<"---->"<<duration.count()<<std::endl;
    }
    for(auto& d: simulation_d)
    {
        out << "S == "<<d.S<<"\tE == "<<d.E<<std::endl;
        out << "I == "<<d.I<<"\tR == "<<d.R<<std::endl;
        out << "D == "<<d.D<<std::endl;
    }
}
// calculates the index range [lower,upper] (referred to Waypoints array)of the waypoints belonging to each cluster.
void Simulation::set_clusters_bounds_indeces()
{
    int lower_index = 0;
    int upper_index = 0;

    // first cluster
    upper_index = Simulation::Clusters[0].size() - 1;
    Simulation::Clusters[0].set_lower_index(0);
    Simulation::Clusters[0].set_upper_index(upper_index);
    lower_index = upper_index + 1; // the upper becomes now lower for the second
    for (int i = 1; i < CLUSTERS_SIZE; ++i)
    {
        auto& current = Simulation::Clusters[i]; // current cluster

        upper_index = lower_index + current.size() - 1;
        current.set_lower_index(lower_index);
        current.set_upper_index(upper_index);
        lower_index = upper_index + 1; // the upper becomes now lower for the next
    }                                  // end for
}
//////////////////////////////////////////
///temporarily used to benchmark
void Simulation::benchmarking()
{



    int n_blocks = 33;
    update_data();
    for (int j = 1; j < n_blocks; ++j)
    {
        for (int i = 0; i < UPDATE_ZONES_INTERVAL; ++i)
        {
            move(false);
            spread();
        }
        update_data();
        update_zones();
    }

    std::ofstream output {"timing.txt"};


    double sum_close_people_cluster = std::accumulate(std::begin(close_people_cluster),std::end(close_people_cluster),0.000);
    double sum_close_people = std::accumulate(std::begin(close_people),std::end(close_people),0.000);
    double sum_exposed = std::accumulate(std::begin(spread_exposedd),std::end(spread_exposedd),0.000);
    double sum_infected = std::accumulate(std::begin(spread_infectedd),std::end(spread_infectedd),0.000);
    double sum_spread = std::accumulate(std::begin(total_spread),std::end(total_spread),0.000);

    double total_steps = n_blocks * UPDATE_ZONES_INTERVAL;

    output << "CLOSE PEOPLE IN CLUSTER\nCalled " <<times_close_cluster_people_called<<" times\n";
    output << "Total time = " << sum_close_people_cluster<<"ms"<<std::endl;
    output << "Mean time per call = " << (sum_close_people_cluster / times_close_cluster_people_called) <<"ms"<< std::endl;
    output << "CLOSE PEOPLE EVERYWHERE\nCalled " <<times_close_people_called<<" times\n";
    output << "Total time = " << sum_close_people<<"ms"<<std::endl;
    output << "Mean time per call = " << (sum_close_people / times_close_people_called) <<"ms"<< std::endl;
    output << "SPREAD EXPOSED\nCalled " <<times_spread_exposed_called<<" times\n";
    output << "Total time = " << sum_exposed<<"ms"<<std::endl;
    output << "Mean time per call = " << (sum_exposed / times_spread_exposed_called) << std::endl;
    output << "SPREAD INFECTED\nCalled " <<times_spread_infected_called<<" times\n";
    output << "Total time = " << sum_infected<<"ms"<<std::endl;
    output << "Mean time per call = " << (sum_infected / times_spread_infected_called)<<"ms" << std::endl;
    output << "SPREAD TOTALE\nCalled " <<times_total_spread_called<<" times\n";
    output << "Total time = " << sum_spread<<"ms"<<std::endl;
    output << "Mean time = " << (sum_spread / times_total_spread_called) <<"ms"<< std::endl;
}

} // namespace smooth_simulation
