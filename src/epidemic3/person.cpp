#include "person.hpp"
#include "simulation.hpp"
#include "parameters.hpp"
#include "../random.hpp"
#include <math.h>

namespace smooth_simulation
{
Person::Person(Status status, int cluster_label, Location home, Location current_location, Location target_location,
               bool is_at_place, double x_speed, double y_speed)
    : status{status}, label{cluster_label}, home{home}, location{current_location}, target{target_location},
      at_place{is_at_place}
{
    velocity[0] = x_speed;
    velocity[1] = y_speed;
}
const Person& default_person()
{
    static Location def_loc{};
    static Person def_per{Status::Susceptible, 0, def_loc, def_loc, def_loc, false, 0.0, 0.0};
    return def_per;
}
Person::Person()
    : status{default_person().status}, label{default_person().label}, home{default_person().home},
      location{default_person().location}, target{default_person().target}, at_place{default_person().at_place}
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
//uniformly generate a new velocity stddev in the range [0,MAXIMUM_VELOCITY_STDDEV) and then extract
//the x and y components from a normal distributions centerd on AVERAGE_VELOCITY
void Person::update_velocity()
{
    // velocity is measured in units/s
    Random rng{};    //seeded engine
    set_speed_x(rng.gauss(speed_x(),rng.uniform(0, MAXIMUM_VELOCITY_STDDEV)));
    set_speed_y(rng.gauss(speed_y(),rng.uniform(0, MAXIMUM_VELOCITY_STDDEV)));
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
    int index = rng.piecewise(Paths,probabilities);
    set_target(Simulation::Waypoints[index]);
    // TODO TESTING
}
/////////////////////////////////////////////////////
////////     CASE OF DIRECT HOME FLIGHT       ///////
/////////////////////////////////////////////////////
// move a person in a straight line to its home location
void Person::move_home()
{
    update_speed();
    double delta_x = std::abs(location.get_X() - home.get_X());
    double delta_y = std::abs(location.get_Y() - home.get_Y());
    double theta = atan(delta_y / delta_x); // angle connecting the target through a straight line
    // now determine the displacement on each axis based on the current person speed
    delta_x = (speed() * cos(theta)) * TIME_STEP; // moving of delta_x on the x axis
    delta_y = (speed() * sin(theta)) * TIME_STEP; // moving of delta_y on the y axis
    double displacement = sqrt(delta_x * delta_x + delta_y * delta_y);

    if (location.get_position().distance_to(target.get_position()) < displacement)
    {
        set_location(home);
        at_place = true;
    }
    Position new_position {location.get_X() + delta_x, location.get_Y() + delta_y};

    location.set_position(new_position);
}
/////////////////////////////////////////////////////
/// CASE OF PERSON MOVING TO A NON-HOME LOCATION ////
/////////////////////////////////////////////////////
void Person::move_toward()
// move a person by an amount determined by the current speed slightly varying the angle
{
    //compute data for shortest possible path
    double delta_x = std::abs(location.get_X() - target.get_X());
    double delta_y = std::abs(location.get_Y() - target.get_Y());
    double theta = atan(delta_y / delta_x); // angle connecting the target through a straight line

    update_speed();   //assign this person a new velocity vector

    Random rng{};  //seeded engine
    // generate an angle between velocity vector direction and the target's one so that will not point precisely to the
    // target
    //TODO direction() can be lower than theta: test this case
    double final_angle = rng.uniform(direction(),theta);
    // calculate new displacement replacing the previous velocity vector with a new one with the same magnitude but
    // different direction so recalculating the new v_x and v_y to determine x_displacement and y_displacement
    x_displacement = speed() * cos(final_angle) * TIME_STEP;
    y_displacement = speed() * sin(final_angle) * TIME_STEP;

    Position new_position {location.get_X() + x_displacement, location.get_Y() + y_displacement};

    location.set_position(new_position); //set new position
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
    Random rng{}; //seeded engine

    int random_index = 0;
    for (int i = 0; i < missing_waypoints; ++i)
    {
        random_index = rng.int_uniform(starting_index,ending_index);
        for (unsigned long j = 0; j < already_chosen_indeces.size(); ++j)
        {
            if (already_chosen_indeces[j] == random_index)
            {
                random_index = rng.int_uniform(starting_index,ending_index); // try a new one
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
// generating pause time at a place according to TPL(Truncated power Law) [see
int determine_pause_time()
{
    double term1 = 0.0;
    double term2 = 0.0;
    double term3 = 0.0;
    double term4 = 0.0;
    double pause_time = 0.0;

    Random rng{}; // set the seed
    std::uniform_real_distribution<> rand(0.0, 1.0);

    double u = rng.uniform(0,1); // number in range [0,1)

    term1 = (u * pow(MAX_PAUSE, PAUSE_EXPONENT)) - (u * pow(MIN_PAUSE, PAUSE_EXPONENT))
            - pow(MAX_PAUSE, PAUSE_EXPONENT);
    term2 = pow(MAX_PAUSE, PAUSE_EXPONENT) * pow(MIN_PAUSE, PAUSE_EXPONENT);
    term3 = -(term1 / term2);
    term4 = pow(term3, (-1 / PAUSE_EXPONENT));
    pause_time = term4;

    return round(pause_time);
}

} // namespace SMOOTH
