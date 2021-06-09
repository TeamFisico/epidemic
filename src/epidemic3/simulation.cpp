#include "simulation.hpp"
#include <random>
namespace SMOOTH
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////    WORLD CREATION  ///////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////
////////          AREA PARTITIONING           ///////
/////////////////////////////////////////////////////
  std::array<Location, WAYPOINTS_SIZE> Simulation::Waypoints;
  std::array<Cluster, CLUSTERS_SIZE> Simulation::Clusters;
  std::array<Person, POPULATION_SIZE> Simulation::People;


void Simulation::partition_in_clusters()
{
    int total_sizes = 0;
    double wpts_left = WAYPOINTS_SIZE;
    double still_to_choose = CLUSTERS_SIZE;
    int index = 0;

    std::random_device rd;  // set the seed
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()

    for (int i = 0; i < 100; ++i)
    {

        double mean_size = wpts_left / still_to_choose;
        // 68% will be in a 25% change range
        std::normal_distribution rand(mean_size, mean_size / 4);
        int current = nearbyint(rand(gen));

        assert(current >= 0.0);
        if (still_to_choose == 1) // the last will take the remaining size --> avoid too many more loops
        {
            Clusters[CLUSTERS_SIZE - 1].size() = (int)wpts_left;
            Clusters[CLUSTERS_SIZE - 1].set_label(CLUSTERS_SIZE - 1);
            Clusters[index].set_weight((double)wpts_left / WAYPOINTS_SIZE);
            return;
        }

        if (total_sizes + current < WAYPOINTS_SIZE)
        {
            Clusters[index].size() = current;
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
////////    1st WAYPOINT OF THE GROUP PLOT    ///////
/////////////////////////////////////////////////////
Location Simulation::first_group_step(int label) const
// Plot the 1st waypoint of the group so that
// it's not in the transmission range of every waypoint
// already plotted
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> rand(0.0, side); // generate the waypoint coordinate in [0,size]

    double X = rand(gen);
    double Y = rand(gen);

    Location try_waypoint{X, Y};

    if (label == 0) // this is the first group of the first cluster
    {
        return try_waypoint;
    }
    else
    { // if 1 <= label <= C-1
        // TODO Optimize this code using function returning
        bool end_loop = false;
        bool change_wpt = false;
        // check the already plotted waypoints(from Clusters[0] to Clusters[label-1])
        while (!end_loop)
        {
            for (int curr_cluster_index = 0; curr_cluster_index < label; ++curr_cluster_index) // loop over Clusters
            {
                if (change_wpt) break;
                for (auto& current_group : Clusters[curr_cluster_index].Groups) // loop over Groups
                {
                    if (change_wpt) break;

                    int it_index = 0;
                    for (auto it = current_group.group_ptr; it_index < current_group.size();
                         ++it) // loop over group Waypoints
                    {

                        if (it->get_distance(try_waypoint) <= R)
                        {
                            change_wpt = true;
                            break;
                        }

                        if (it_index == current_group.size() - 1 && curr_cluster_index == label - 1) end_loop = true;

                        ++it_index; // next Location
                    }               // end loop waypoints
                }                   // end loop Groups
            }                       // end loop Clusters

            if (!end_loop)
            {
                // retry with new location
                try_waypoint.X() = rand(gen);
                try_waypoint.Y() = rand(gen);
            }
        } // end while
        assert(try_waypoint.X() > 0.0 && try_waypoint.Y() > 0.0 && try_waypoint.X() <= side &&
               try_waypoint.X() <= side);
        return try_waypoint;
    } // else
}

/////////////////////////////////////////////////////
////////     NEIGHBOURING WAYPOINTS PLOT      ///////
/////////////////////////////////////////////////////
// plot within 0.1 R from the other waypoint of this group
// by extracting the next waypoint from a gaussian distribution
// with mean on the previous waypoint with a stddev of 0.1R/3 so that the 99.7%
// will be in the 0.1R range
Location Simulation::plot_nearby_waypoints(int cluster_label, int group_label, Location const& starting_waypoint) const
{

    std::random_device gs; // set the seed for gaussian generation
    std::mt19937 gen_gaus(gs());

    for (int i = 1; i < Clusters[cluster_label].Groups[group_label].size(); ++i)
    {
        std::normal_distribution<double> gaus1(starting_waypoint.X(), 0.1 * R / 3); // generate x of next waypoint
        std::normal_distribution<double> gaus2(starting_waypoint.Y(), 0.1 * R / 3); // generate y of next waypoint

        double current_waypoint_x = gaus1(gen_gaus);
        double current_waypoint_y = gaus2(gen_gaus);

        // handle cases with a possible negative extraction
        if (current_waypoint_x < 0.0) current_waypoint_x = 0.0;
        if (current_waypoint_y < 0.0) current_waypoint_y = 0.0;

        Location current_waypoint{current_waypoint_x, current_waypoint_y};

        Clusters[cluster_label].Groups[group_label].group_ptr[i] = current_waypoint;

    } // end for

    // return the last plotted waypoint of this group
    int index = Clusters[cluster_label].Groups[group_label].size() - 1;
    return Clusters[cluster_label].Groups[group_label].group_ptr[index];
}

/////////////////////////////////////////////////////
////////  1st WAYPOINT OF OTHER GROUPS PLOT   ///////
/////////////////////////////////////////////////////
// plot the first waypoint of the current group within a distance d of
// Y/4 <= d <= Y/3
Location Simulation::other_groups_step(Location const& prev_group_waypoint) const
{
    double const Y = 2 * side / CLUSTERS_SIZE;

    std::random_device rd; // set the seed for the gaussian extraction
    std::mt19937 gen(rd());

    // generate waypoints in a square of side Y/3 (resize if out of bounds)
    double lower_x = prev_group_waypoint.X() - Y / 3;
    double upper_x = prev_group_waypoint.X() + Y / 3;
    double lower_y = prev_group_waypoint.Y() - Y / 3;
    double upper_y = prev_group_waypoint.Y() + Y / 3;

    if (lower_x < 0.0) lower_x = 0.0;
    if (lower_y < 0.0) lower_y = 0.0;
    if (upper_x > side) upper_x = side;
    if (upper_y > side) upper_y = side;

    std::uniform_real_distribution<> rand_x(lower_x, upper_x);
    std::uniform_real_distribution<> rand_y(lower_y, upper_y);

    while (true)
    {
        Location try_waypoint{rand_x(gen), rand_y(gen)};
        double distance = try_waypoint.get_distance(prev_group_waypoint);
        if (distance >= Y / 4 && distance <= Y / 3)
        {
            assert(try_waypoint.X() > 0.0 && try_waypoint.Y() > 0.0 && try_waypoint.X() <= side &&
                   try_waypoint.Y() <= side);
            return try_waypoint;
        }
    } // end while
}
/////////////////////////////////////////////////////
////////             FINAL PLOT              ////////
/////////////////////////////////////////////////////
void Simulation::plot_waypoints()
{
    for (int i = 0; i < CLUSTERS_SIZE; ++i)
    {
        // set the first waypoint associated with the first group
        Clusters[i].Groups[0].group_ptr[0] = first_group_step(i);
        // plot all the waypoints of the group around this one and return a reference to the last one
        Location previous_group_last_waypoint = plot_nearby_waypoints(i, 0, Clusters[i].Groups[0].group_ptr[0]);

        for (unsigned int j = 1; j < Clusters[i].Groups.size(); ++j) // now set the other groups' wpts
        {
            // setting the first waypoint of this group
            Clusters[i].Groups[j].group_ptr[0] = other_groups_step(previous_group_last_waypoint);
            // plot the neighbourhood and set the la
            previous_group_last_waypoint = plot_nearby_waypoints(i, j, Clusters[i].Groups[j].group_ptr[0]);
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////     SIMULATION     ///////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////
////////       SIMULATION CONSTRUCTOR         ///////
/////////////////////////////////////////////////////
Simulation::Simulation(double side, double transmission_range, double alpha, double percent_waypoint,
                       double minimum_pause, double maximum_pause)
    : side{side}, R{transmission_range}, alpha{alpha}, y{percent_waypoint}, min_pause{minimum_pause}, max_pause{
                                                                                                          maximum_pause}
{

    partition_in_clusters();
    for (Cluster& cluster : Clusters)
    {
        cluster.partition_in_groups();
    }
    plot_waypoints();
}
Simulation::Simulation()
{
}

/////////////////////////////////////////////////////
////////          CLUSTER ASSIGNMENT          ///////
/////////////////////////////////////////////////////
void Simulation::assign_to_cluster()
// assign a person to a cluster based on the cluster weight using piecewise-const-dist
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<double> weights(CLUSTERS_SIZE); // weights

    for (int i = 0; i < CLUSTERS_SIZE; ++i)
    {
        weights[i] = Clusters[i].weight();
    }

    std::discrete_distribution<int> dis(std::begin(weights), std::end(weights));

    for (auto& person : People)
    {
        int index = dis(gen);
        person.set_cluster(index);
    }
}

/////////////////////////////////////////////////////
////////          HOME ASSIGNMENT             ///////
/////////////////////////////////////////////////////
void Simulation::assign_home(int label)
// generate home address inside the relating cluster limits and generate groups of
// 1 to 5 people(families) which are gonna be assigned the same home location
{
    std::random_device rd1;
    std::mt19937 gen1(rd1());
    std::uniform_int_distribution<> family(1, 5);

    std::random_device rd2;
    std::mt19937 gen2(rd2());

    int people_left = POPULATION_SIZE;
    int current_family = 0;
    double lw_x = 0.0;
    double up_x = 0.0;
    double lw_y = 0.0;
    double up_y = 0.0;

    // iterate over People array
    for (auto it1 = std::begin(People); it1 != std::end(People); ++it1)
    {
        // set the bounds based on the belonging cluster for home generation
        lw_x = Clusters[it1->cluster_label()].lower_x();
        up_x = Clusters[it1->cluster_label()].upper_x();
        lw_y = Clusters[it1->cluster_label()].lower_y();
        up_y = Clusters[it1->cluster_label()].upper_y();

        std::uniform_real_distribution<double> x_home(lw_x, up_x);
        std::uniform_real_distribution<double> y_home(lw_y, up_y);

        current_family = family(gen1);
        Location home{x_home(gen2), y_home(gen2)};

        if (people_left <= 5)
        {
            for (auto it2 = it1; it2 != std::end(People); ++it2)
            {
                it2->set_home(home);
            }
            return;
        }

        int i = 0;
        for (auto it2 = it1; i < current_family; ++it2)
        {
            it2->set_home(home);
            people_left -= current_family;
            ++i;
        }
    }
}
/////////////////////////////////////////////////////
////////   PATH ASSIGNMENT(FROM 1 CLUSTER)    ///////
/////////////////////////////////////////////////////
// fill person Path vector with the waypoints to visit FROM THE CLUSTER HE/SHE'S IN CURRENTLY
void Simulation::fill_path_from_cluster(Person& person)
{

    int belonging_cluster_size = Clusters[person.cluster_label()].size();

    std::random_device rd;
    std::mt19937 gen(rd());

    int selected_size = static_cast<int>(belonging_cluster_size * y);
    // now determining the this cluster's indeces range inside Waypoints array

    int starting_index = 0;
    int ending_index = 0;

    if (person.cluster_label() > 0) // this is not the first clust
    {
        for (int i = 0; i < person.cluster_label(); ++i) // check the previous clusters
        {
            starting_index += Clusters[i].size();
            ++i;
        }
        ending_index = starting_index + belonging_cluster_size;
    }
    else
    {
        ending_index = belonging_cluster_size;
    }

    std::uniform_int_distribution<> rand(starting_index, ending_index);

    int random_index = 0;
    std::vector<int> already_chosen; // keep track of already chosen waypoints indeces

    assert(person.Paths.size() == 0);
    person.Paths.reserve(selected_size);
    already_chosen.reserve(selected_size);
    for (int i = 0; i < selected_size; ++i)
    {
        random_index = rand(gen);
        for (int i = 0; i < already_chosen.size(); ++i)
        {
            if (already_chosen[i] == random_index)
            {
                random_index = rand(gen); // try a new one
                i = -1;                   // restart then from 0
            }
        }
        already_chosen.push_back(random_index);
        person.Paths.push_back(random_index);
    }
}
/////////////////////////////////////////////////////
//////   PATH ASSIGNMENT(FROM GREEN CLUSTERS)   /////
/////////////////////////////////////////////////////
// select some waypoints among the current cluster and the others
void Simulation::fill_path(Person& person)
{
    // TODO define
}
/////////////////////////////////////////////////////
////////            TARGET UPDATE             ///////
/////////////////////////////////////////////////////
void Simulation::update_target(Person* person)
// use the Least action trip planning(LATP) algorithm to determine new person target
{
    // calculate distance between all the possibile paths
    // then calculate the probability to visit each waypoint
    // through the weight function and finally extract the next
    // target with the previous probabilities assigned to each wpt
    Location person_location = person->get_location();

    std::vector<double> distances;
    std::vector<double> probabilities;
    distances.reserve(person->Paths.size());     // allocate the space
    probabilities.reserve(person->Paths.size()); // allocate the space
    double current_weight = 0.0;
    // compute distances
    for (int& index : person->Paths)
    {
        Location waypoint = Waypoints[index];
        current_weight = weight_function(person_location.get_distance(waypoint), alpha);
        distances.push_back(current_weight);
    }
    double sum = std::accumulate(std::begin(distances), std::end(distances), 0.0);

    for (double curr_weight : distances)
    {
        probabilities.push_back(curr_weight / sum);
    }
    // ok now knowing all the probabilities for a person to target an indexed waypoint
    // extract an index based on this probabilities

    std::random_device rd;
    std::mt19937 gen(rd());
    // generate one index based on the previously determinated probabilities(weights)
    std::piecewise_constant_distribution<int> rand(std::begin(person->Paths), std::begin(person->Paths),
                                                   probabilities.begin());
    person->set_target(Waypoints[rand(gen)]);
    // TODO TESTING
}

double weight_function(double distance, double LATP_parameter)
{
    return 1 / std::pow(distance, LATP_parameter);
}

} // namespace SMOOTH
