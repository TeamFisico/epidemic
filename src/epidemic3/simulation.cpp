#include "simulation.hpp"
#include <fstream>
#include <iostream>

namespace smooth_simulation
{
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
// clusters
void Simulation::initialise_people_status(int E, int I, int R)
{
    std::vector<int> chosen_people_i; // vector to take trace of already setted people indeces
    chosen_people_i.reserve(E + I + R);
    // setting exposed individuals
    int cl_index = 0;
    while (E > 0)
    {
        if (cl_index == CLUSTERS_SIZE) { cl_index = 0; } // occurs when E > CLUSTERS_SIZE
        auto& cl = Clusters[cl_index];
        int try_person_i = engine.int_uniform(cl.lower_index(), cl.upper_index()); // pick a random index
        for (unsigned long j = 0; j < chosen_people_i.size(); ++j)                 // check if already taken
        {
            if (chosen_people_i[j] == try_person_i) // the index has been already taken
            {
                try_person_i = engine.int_uniform(cl.lower_index(), cl.upper_index()); // pick a random index
                j = 0;
                continue; // restart the loop
            }
        }
        People[try_person_i].set_current_status(Status::Exposed);
        chosen_people_i.push_back(try_person_i);
        --E;
        ++cl_index;
    }
    // setting infected individuals
    cl_index = 0;
    while (I > 0)
    {
        if (cl_index == CLUSTERS_SIZE) { cl_index = 0; } // occurs when E > CLUSTERS_SIZE
        auto& cl = Clusters[cl_index];
        int try_person_i = engine.int_uniform(cl.lower_index(), cl.upper_index()); // pick a random index
        for (unsigned long j = 0; j < chosen_people_i.size(); ++j)                 // check if already taken
        {
            if (chosen_people_i[j] == try_person_i) // the index has been already taken
            {
                try_person_i = engine.int_uniform(cl.lower_index(), cl.upper_index()); // pick a random index
                j = 0;
                continue; // restart the loop
            }
        }
        People[try_person_i].set_current_status(Status::Infected);
        chosen_people_i.push_back(try_person_i);
        --I;
        ++cl_index;
    }
    // setting recovered individuals
    cl_index = 0;
    while (R > 0)
    {
        if (cl_index == CLUSTERS_SIZE) { cl_index = 0; } // occurs when E > CLUSTERS_SIZE
        auto& cl = Clusters[cl_index];
        int try_person_i = engine.int_uniform(cl.lower_index(), cl.upper_index()); // pick a random index
        for (unsigned long j = 0; j < chosen_people_i.size(); ++j)                 // check if already taken
        {
            if (chosen_people_i[j] == try_person_i) // the index has been already taken
            {
                try_person_i = engine.int_uniform(cl.lower_index(), cl.upper_index()); // pick a random index
                j = 0;
                continue; // restart the loop
            }
        }
        People[try_person_i].set_current_status(Status::Recovered);
        chosen_people_i.push_back(try_person_i);
        --R;
        ++cl_index;
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

    for (auto& cluster : Clusters) // set (x,y) limits for each cluster
    {
        cluster.set_limits();
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
}
/////////////////////////////////////////////////////
////       CLOSE PEOPLE ALL OVER THE AREA        ////
/////////////////////////////////////////////////////
void Simulation::close_people_fill(const Person& current_person, std::vector<int> close_people_i)
{
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
                if (pos.in_radius(current_person.position, spread_radius) && stat == Status::Exposed && !is_home)
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
void Simulation::close_cluster_people_fill(const Person& current_person, std::vector<int> close_people_i)
{
    close_people_i.clear();
    for (int& person_i : Clusters[current_person.label].People_i) // loop over people in this person's cluster indeces
    {
        Person& person = People[person_i]; // ref to current cluster person
        auto const& pos = person.position;
        auto const& stat = person.current_status();
        bool const& is_home = person.at_home;
        // check if person is close enough(most restrictive condition),susceptible and not at home
        if (pos.in_radius(current_person.position, spread_radius) && stat == Status::Exposed && !is_home)
        {
            close_people_i.push_back(person_i); // ok, add this person index to close people vector
        }
    } // end loop
}
/////////////////////////////////////////////////////
////////            MOVE PEOPLE               ///////
/////////////////////////////////////////////////////
void Simulation::move()
{
    for (auto& c : Clusters)
    {
        c.move(engine);
    }
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
        if (p.changed_status)
        {
            p.update_status();
            p.set_changed_status(false);
        }
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
    std::vector<int> close_people_i; // contains close people indeces

    for (auto& cl : Clusters) // loop over clusters
    {
        for (int& person_i : cl.People_i) // loop over indeces of people of this cluster
        {
            auto& person = People[person_i]; // ref to current person
            if (person.current_status() == Status::Exposed)
            {
                // determine if this person will be able to be infected
                if (engine.try_event(alpha))
                {
                    person.set_new_status(Status::Infected);
                    person.set_changed_status(true);
                }
            }
            else if (person.current_status() == Status::Infected)
            {
                if (!person.at_home) // the person is not at home
                {
                    if (cl.zone_type() == Zone::White) // the cluster is white
                    {
                        close_people_fill(person, close_people_i); // fill with close ppl indeces
                    }
                    else // the cluster is either yellow,orange or red
                    {
                        close_cluster_people_fill(person, close_people_i);
                    }
                    for (int& close_i : close_people_i) // loop over close people
                    {
                        Person& close_person = People[close_i]; // ref to current close person
                        if (engine.try_event(beta))
                        {
                            close_person.set_new_status(Status::Exposed);
                            close_person.set_changed_status(true);
                        } // the close one's are exposed
                    }
                }
                // else ignore the persone
                if (engine.try_event(gamma))
                {
                    person.set_new_status(Status::Recovered);
                    person.set_changed_status(true);
                    continue;
                } // determine if the person will recover
                else if (engine.try_event(kappa))
                {
                    person.set_new_status(Status::Dead);
                    person.set_changed_status(true);
                } // determine if the person will
            }     // end Infected case
        }         // end loop over cluster's people
    }             // end loop over clusters
}
/////////////////////////////////////////////////////
///////               SIMULATE                ///////
/////////////////////////////////////////////////////
void Simulation::simulate()
{
    using namespace std::literals::chrono_literals;
    std::ofstream out{"out.txt"};
    initialise_people_status(data.E, data.I, data.R);
    std::cout << "initialised status\n";
    for (int i = 0; i < 3; ++i)
    {
        auto start = std::chrono::high_resolution_clock::now();
        for (int steps = 1; steps < UPDATE_ZONES_INTERVAL; ++steps) // do 1 block
        {
            move();
            spread();
            update_people_status();
        }
        update_data();
        update_zones();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = end - start;
        std::cout << "block " << i << "-->" << duration.count() << " s " << std::endl;
        out << "Block " << i << std::endl;
        out << "S == " << data.S << "\tE == " << data.E << std::endl;
        out << "I == " << data.I << "\tR == " << data.R << std::endl;
        out << "D == " << data.D << std::endl;
    }
    //        get_simulation_data();
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

//////////////// HELPER FUNCTIONS  //////////////////
double weight_function(double distance, double LATP_parameter)
{
    return 1 / std::pow(distance, LATP_parameter);
}

} // namespace smooth_simulation
