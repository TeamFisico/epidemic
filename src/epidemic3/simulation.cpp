#include "simulation.hpp"
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
        try_position = generate_close_position(current_center, TRANSMISSION_RANGE / 10.0);

        for (unsigned int j = 0; j < already_setted_wpts.size(); ++j) // check already setted waypoints
        {
            if (try_position.distance_to(already_setted_wpts[j]) >
                TRANSMISSION_RANGE / 10.0) // if it doesn't respect the condition
            {
                try_position = generate_close_position(current_center, TRANSMISSION_RANGE / 10.0);
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
    std::array<double, CLUSTERS_SIZE> weights{}; // fill with clusters weights
    std::array<int, POPULATION_SIZE> labels{};   // fill with generated labels according to weights

    for (int i = 0; i < CLUSTERS_SIZE; ++i) // fill with clusters weight
    {
        weights[i] = Clusters[i].weight();
    }

    // fill labels array with generated labels
    engine.engine().generate<std::discrete_distribution>(std::begin(labels), std::end(labels), std::begin(weights),
                                                         std::end(weights));
    // iterate over the labels array assigning the corresponding label to each person
    int j = 0;
    for (auto& label : labels)
    {
        People[j].set_cluster(label);          // label the person with its home cluster
        Clusters[label].People_i.push_back(j); // add person's index into People array to the cluster
        ++j;
    }
}
/////////////////////////////////////////////////////
////////          HOME ASSIGNMENT             ///////
/////////////////////////////////////////////////////
void Simulation::assign_home_to_people()
// generate home address inside the relating cluster limits and assign them to
// families
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
        // set the bounds based on the belonging cluster for home generation
        lw_x = Clusters[People[i].home_cluster()].lower_x();
        up_x = Clusters[People[i].home_cluster()].upper_x();
        lw_y = Clusters[People[i].home_cluster()].lower_y();
        up_y = Clusters[People[i].home_cluster()].upper_y();

        family_size = engine.rounded_gauss(AVERAGE_FAMILY_SIZE, 1.5); // a family as a mean
        if (family_size < 1) family_size = 1;
        Position try_pos{engine.uniform(lw_x, up_x), engine.uniform(lw_y, up_y)};

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
            }
        }
        else
        {
            int k = 0; // take trace of the already setted homes
            for (int j = i; k < family_size; ++j)
            {
                People[j].set_home(gen_home);
                ++k;
            }
            people_left -= family_size;         // decrease left people
            generated_homes.push_back(try_pos); // take trace of the setted position
        }
    } // end loop over People
}
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

    for (auto& p : People)
    {
        p.location = p.home;
    }
    std::cout << "People initial location setted being home\n";
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
      spread_radius{spread_radius}, alpha{alpha}, beta{beta}, gamma{gamma}, kappa{kappa}, data{data}, engine{}
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
                auto const& pos = person.location.get_position();
                auto const& stat = person.current_status();
                bool const& is_home = person.at_home();
                // check if person is close enough(most restrictive condition),susceptible and not at home
                if (pos.in_radius(current_person.location.get_position(), spread_radius) && stat == Status::Exposed &&
                    !is_home)
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
        auto const& pos = person.location.get_position();
        auto const& stat = person.current_status();
        bool const& is_home = person.at_home();
        // check if person is close enough(most restrictive condition),susceptible and not at home
        if (pos.in_radius(current_person.location.get_position(), spread_radius) && stat == Status::Exposed && !is_home)
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
    for (int i = 0; i < 50; ++i)
    {
        for (auto& c : Clusters)
        {
            c.move();
        }
    }
}
/////////////////////////////////////////////////////
/////    UPDATE CLUSTER ZONES BASED ON DATA     /////
/////////////////////////////////////////////////////
void Simulation::update_zones()
{
    for (auto& cl : Clusters)
    {
        double ratio{(double)data.I / data.S}; // infected - suceptible ratio

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
///////             UPDATE DATA               ///////
/////////////////////////////////////////////////////
void Simulation::update_data()
{
    for (auto& cl : Clusters)
    {
        cl.update_data();
    }
}
/////////////////////////////////////////////////////
///////               GET DATA                ///////
/////////////////////////////////////////////////////
Data Simulation::get_simulation_data() const
{
    unsigned int nS = 0;
    unsigned int nE = 0;
    unsigned int nI = 0;
    unsigned int nR = 0;
    unsigned int nD = 0;
    unsigned int ICU_cap = 0;

    for (auto& cl : Clusters)
    {
        nS += cl.get_data().S;
        nE += cl.get_data().E;
        nI += cl.get_data().I;
        nR += cl.get_data().R;
        nD += cl.get_data().D;
        ICU_cap += cl.get_data().ICU_capacity; // TODO fallo bene
    }
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
                if (engine.try_event(alpha)) { person.set_new_status(Status::Infected); }
            }
            else if (person.current_status() == Status::Infected)
            {
                if (!person.at_home()) // the person is not at home
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
                        } // the close one's are exposed
                    }
                }
                // else ignore the persone
                if (engine.try_event(gamma))
                {
                    person.set_new_status(Status::Recovered);
                } // determine if the person will recover
                if (engine.try_event(kappa))
                {
                    person.set_new_status(Status::Dead);
                } // determine if the person will
                  // die
            }
        } // end loop over cluster's people
    }     // end loop over clusters
}
/////////////////////////////////////////////////////
///////               SIMULATE                ///////
/////////////////////////////////////////////////////
void Simulation::simulate()
{
    move();
    spread();
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
