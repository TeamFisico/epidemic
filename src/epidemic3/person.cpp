#include "person.hpp"
#include "simulation.hpp"
#include <math.h>
#include <algorithm>
#include <iostream>

namespace smooth_simulation
{
Person::Person(Status status, int cluster_label, Location home, Location current_location, Location target_location,
               bool is_at_place, double x_speed, double y_speed, int stay_time)
    : status{status}, label{cluster_label}, home{home}, location{current_location}, target{target_location},
      at_place{is_at_place}, stay{stay_time}
{
    velocity[0] = x_speed;
    velocity[1] = y_speed;
}
const Person& default_person()
{
    static Location def_loc{};
    static Person def_per{Status::Susceptible, 0, def_loc, def_loc, def_loc, false, 0.0, 0.0, 0};
    return def_per;
}
Person::Person()
    : status{default_person().status}, label{default_person().label}, home{default_person().home},
      location{default_person().location}, target{default_person().target}, at_place{default_person().at_place},
      stay{default_person().stay}
{
    velocity[0] = STARTING_VELOCITY[0];
    velocity[1] = STARTING_VELOCITY[1];
}
double Person::speed() const
{
    return sqrt(velocity[0] * velocity[0] + velocity[1] * velocity[1]);
}
bool Person::at_home() const
{
    if (location == home && target == home) return true;
    return false;
}
/////////////////////////////////////////////////////
////////             STATUS UPGRADE           ///////
/////////////////////////////////////////////////////
void Person::upgrade_status()
{
    switch (status)
    {
    case Status::Susceptible:
        status = Status::Latent;
        break;
    case Status::Latent:
        status = Status::Infected;
        break;
    case Status::Infected:
        status = Status::Recovered;
        break;
    default:
        break;
    }
}
/////////////////////////////////////////////////////
////////          VELOCITY UPDATE             ///////
/////////////////////////////////////////////////////
// uniformly generate a new velocity stddev in the range [0,MAXIMUM_VELOCITY_STDDEV) and then extract
// the x and y components from a normal distributions centerd on AVERAGE_VELOCITY
void Person::update_velocity()
{
    // velocity is measured in units/s
    Random rng{}; // seeded engine
    set_speed_x(rng.gauss(speed_x(), rng.uniform(0, MAXIMUM_VELOCITY_STDDEV)));
    set_speed_y(rng.gauss(speed_y(), rng.uniform(0, MAXIMUM_VELOCITY_STDDEV)));
}

/////////////////////////////////////////////////////
////////            TARGET UPDATE             ///////
/////////////////////////////////////////////////////
void Person::update_target(double LATP_parameter)
// use the Least action trip planning(LATP) algorithm to determine new person target
{
    std::vector<double> distances;
    std::vector<double> probabilities;
    distances.reserve(Paths.size());     // allocate the space
    probabilities.reserve(Paths.size()); // allocate the space
    double current_weight = 0.0;
    // compute distances and calculate weight
    for (int& index : Paths)
    {
        Location waypoint = Simulation::Waypoints[index];
        current_weight = weight_function(location.get_position().distance_to(waypoint.get_position()), LATP_parameter);
        distances.push_back(current_weight);
    }
    double sum = std::accumulate(std::begin(distances), std::end(distances), 0.0);

    for (double curr_weight : distances)
    {
        probabilities.push_back(curr_weight / sum);
    }
    // ok now knowing all the probabilities for a person to target an indexed waypoint
    // extract an index based on this probabilities

    Random rng{};
    // generate one index based on the previously determinated probabilities(weights)
    int index = rng.piecewise(Paths, probabilities);
    set_target(Simulation::Waypoints[index]);
    // TODO TESTING
}
/////////////////////////////////////////////////////
////////     CASE OF DIRECT HOME FLIGHT       ///////
/////////////////////////////////////////////////////
// move a person in a straight line to its home location
//when the person arrives refill path
void Person::move_home()
{
    // compute data for shortest possible path
    double delta_x = std::abs(location.get_X() - target.get_X());
    double delta_y = std::abs(location.get_Y() - target.get_Y());
    double direct_angle = atan(delta_y / delta_x); // angle connecting the target through a straight line

    update_velocity();   //assign this person a new velocity vector
    // now determine the displacement on each axis based on the current person speed
    double x_displacement = (speed() * cos(direct_angle)) * TIME_STEP; // moving of delta_x on the x axis
    double y_displacement = (speed() * sin(direct_angle)) * TIME_STEP; // moving of delta_y on the y axis

    Position new_position{location.get_X() + x_displacement, location.get_Y() + y_displacement};

    // check if the person will enter the target location radius
    if (new_position.in_radius(home.get_position(),HOME_RADIUS)) // the person has arrived to the target
    {
        location.set_position(home.get_position()); // set new position
        at_place = true;                     // the person is now at a place
        stay = determine_pause_time();       // how much time he/she will spend there
        //determine and fill path from home of green clusters
        if (are_white_available(*this)) { fill_path_white(*this); }
        else { fill_path_home(*this); }
        return;
    }

    location.set_position(new_position); // set new position

}
/////////////////////////////////////////////////////
/// CASE OF PERSON MOVING TO A NON-HOME LOCATION ////
/////////////////////////////////////////////////////
void Person::move_toward()
// move a person by an amount determined by the current speed slightly varying the angle
{
    // compute data for shortest possible path
    double delta_x = std::abs(location.get_X() - target.get_X());
    double delta_y = std::abs(location.get_Y() - target.get_Y());
    double direct_angle = atan(delta_y / delta_x); // angle connecting the target through a straight line

    update_velocity(); // assign this person a new velocity vector

    // generate an angle between velocity vector direction and the target's one so that will not point precisely to the
    // target
    Random rng{}; // seeded engine
    double final_angle = 0.0;
    if (direct_angle >= direction()) { final_angle = rng.uniform(direction(), direct_angle); }
    else
    {
        final_angle = rng.uniform(direct_angle, direction());
    }

    // calculate new displacement replacing the previous velocity vector with a new one with the same magnitude but
    // different direction so recalculating the new v_x and v_y to determine x_displacement and y_displacement
    double x_displacement = speed() * cos(final_angle) * TIME_STEP;
    double y_displacement = speed() * sin(final_angle) * TIME_STEP;

    Position new_position{location.get_X() + x_displacement, location.get_Y() + y_displacement};

    // check if the person will enter the target location radius
    if (new_position.in_radius(target.get_position(), target.get_radius())) // the person has arrived to the target
    {
        location.set_position(new_position); // set new position
        at_place = true;                     // the person is now at a place
        stay = determine_pause_time();       // how much time he/she will spend there
                                             //erase the current target from Paths
        remove_target(*this, target); //remove target from path
        return;
    }

    location.set_position(new_position); // set new position
}
/////////////////////////////////////////////////////
////////            MOVE A PERSON             ///////
/////////////////////////////////////////////////////
void Person::move_person()
{
    if (target == home) move_home();
    else
        move_toward();
}
/////////////////////////////////////////////////////
////////        DETERMINE PATHS SIZE          ///////
/////////////////////////////////////////////////////
int determine_fill_size(Person const& person)
{
    int current_paths_size = person.Paths.size();
    double selected_size = Simulation::Clusters[person.home_cluster()].size() * VISITING_PERCENTAGE; // Paths final size
    return static_cast<int>(selected_size - current_paths_size);
}
/////////////////////////////////////////////////////
////////      DETERMINE HOW TO FILL PATH      ///////
/////////////////////////////////////////////////////
//return true if there are any white clusters available
bool are_white_available(const Person& person)
{
    for (auto& cluster : Simulation::Clusters)
    {
        if (cluster.zone_type() == Zone::White && cluster.label() != person.home_cluster())
        {
            return true;
        }
    }
    return false;
}
/////////////////////////////////////////////////////
////////  PATH ASSIGNMENT FROM HOME CLUSTER   ///////
/////////////////////////////////////////////////////
// fill person Path vector with the waypoints to visit from the home cluster
void fill_path_home(Person& person)
{
    // determine this cluster's indeces range inside Waypoints array
    int starting_index = 0;
    int ending_index = 0;

    if (person.home_cluster() > 0) // this is not the first clust
    {
        for (int i = 0; i < person.home_cluster(); ++i)
        {
            starting_index += Simulation::Clusters[i].size(); // add up the indeces of the previous clusters
        }
        ending_index = starting_index + Simulation::Clusters[person.home_cluster()].size() - 1;
    }
    else
    {
        ending_index = Simulation::Clusters[person.home_cluster()].size() - 1;
    }

    int missing_waypoints = determine_fill_size(person); // number of waypoints to add to person.Paths

    std::vector<int> already_chosen_indeces;

    person.Paths.reserve(missing_waypoints); // avoid reallocation when pushing back
    already_chosen_indeces.reserve(person.Paths.size() + person.Paths.size());

    assert(already_chosen_indeces.empty()); // TODO consider deleting
    // keep track of the indeces of the waypoints already in person.Paths
    for (int& taken_index : person.Paths)
    {
        already_chosen_indeces.push_back(taken_index);
    }
    Random rng{}; // seeded engine

    int random_index = 0;
    for (int i = 0; i < missing_waypoints; ++i)
    {
        random_index = rng.int_uniform(starting_index, ending_index);
        for (unsigned long j = 0; j < already_chosen_indeces.size(); ++j)
        {
            if (already_chosen_indeces[j] == random_index)
            {
                random_index = rng.int_uniform(starting_index, ending_index); // try a new one
                j = 0;
                continue; // restart the inner loop
            }
        }

        already_chosen_indeces.push_back(random_index);
        person.Paths.push_back(random_index);
    }
}
/////////////////////////////////////////////////////
//////   PATH ASSIGNMENT(FROM WHITE CLUSTERS)   /////
/////////////////////////////////////////////////////
// select some waypoints among the current cluster and the others
void fill_path_white(Person& person)
{
    std::vector<int> white_clusters; // vector containing white clusters labels
    for (Cluster& cl : Simulation::Clusters)
    {
        if (cl.zone_type() == Zone::White && person.home_cluster() != cl.label()) white_clusters.push_back(cl.label());
    }
}

/////////////////////////////////////////////////////
//////        DETERMINE STAY AT A PLACE         /////
/////////////////////////////////////////////////////
// generating pause time at a place according to TPL(Truncated power Law) [see paper for details]
int determine_pause_time()
{
    double term1 = 0.0;
    double term2 = 0.0;
    double term3 = 0.0;
    double term4 = 0.0;
    double pause_time = 0.0;

    Random rng{}; // set the seed
    std::uniform_real_distribution<> rand(0.0, 1.0);

    double u = rng.uniform(0, 1); // number in range [0,1)

    term1 =
        (u * pow(MAX_PAUSE, PAUSE_EXPONENT)) - (u * pow(MIN_PAUSE, PAUSE_EXPONENT)) - pow(MAX_PAUSE, PAUSE_EXPONENT);
    term2 = pow(MAX_PAUSE, PAUSE_EXPONENT) * pow(MIN_PAUSE, PAUSE_EXPONENT);
    term3 = -(term1 / term2);
    term4 = pow(term3, (-1 / PAUSE_EXPONENT));
    pause_time = term4;

    return round(pause_time);
}
//remove a visited target from person.Path: since we don't care about the targets order, move the one to remove to
// the end and remove it preventing moving all items after it
void remove_target(Person& person,Location to_remove)
{
    for (int& targ_index : person.Paths)
    {
        if (Simulation::Waypoints[targ_index] == to_remove)
        {
            std::swap(targ_index,person.Paths.back()); //swap the last element with the one to be removed
            person.Paths.pop_back(); //erase the last element of the vector
            return;
        }
    }
    //if we get here there's a problem
    std::cerr<<"The target is not in person.Paths!.Aborting.";
}
//return a vector with white clusters labels
std::vector<int> white_clusters_labels()
{
    std::vector<int> labels;
    for (auto& cl : Simulation::Clusters)
    {
        if (cl.zone_type() == Zone::White) labels.push_back(cl.label());
    }
}

} // namespace smooth_simulation



















