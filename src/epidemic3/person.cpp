#include "person.hpp"
#include "simulation.hpp"
#include <math.h>
#include <random>

namespace SMOOTH
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
    static Location def_loc{0.0, 0.0};
    static Person def_per{Status::Susceptible, 0, def_loc, def_loc, def_loc, false, 0.0, 0.0};
    return def_per;
}
Person::Person()
    : status{default_person().status}, label{default_person().label}, home{default_person().home},
      location{default_person().location}, target{default_person().target}, at_place{default_person().at_place}
{
    velocity[0] = default_person().velocity[0];
    velocity[1] = default_person().velocity[1];
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
////////            SPEED UPDATE              ///////
/////////////////////////////////////////////////////
void Person::update_speed()
{
    const double max_std_dev = 1; // TODO Determine maximum standard deviation
    // velocity is measured in units/s
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> std_dev(0, max_std_dev); // set the std dev extraction

    std::normal_distribution<double> x_sp(speed_x(), std_dev(gen));
    std::normal_distribution<double> y_sp(speed_y(), std_dev(gen));

    set_speed_x(x_sp(gen));
    set_speed_y(y_sp(gen));
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
    // compute distances
    for (int& index : Paths)
    {
        Location waypoint = Simulation::Waypoints[index];
        current_weight = weight_function(location.distance(waypoint), LATP_parameter);
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
    std::piecewise_constant_distribution<int> rand(std::begin(Paths), std::begin(Paths), probabilities.begin());
    this->set_target(Simulation::Waypoints[rand(gen)]);
    // TODO TESTING
}
/////////////////////////////////////////////////////
////////     CASE OF DIRECT HOME FLIGHT       ///////
/////////////////////////////////////////////////////
// move a person in a straight line to its home location
void Person::move_home()
{
    update_speed();
    double delta_x = std::abs(location.X() - home.X());
    double delta_y = std::abs(location.Y() - home.Y());
    double theta = atan(delta_y / delta_x); // angle connecting the target through a straight line
    // now determine the displacement on each axis based on the current person speed
    delta_x = (speed() * cos(theta)) * TIME_STEP; // moving of delta_x on the x axis
    delta_y = (speed() * sin(theta)) * TIME_STEP; // moving of delta_y on the y axis
    double displacement = sqrt(delta_x * delta_x + delta_y * delta_y);

    if (location.distance(target) < displacement)
    {
        set_location(home);
        at_place = true;
    }
    Location new_location{location.X() + delta_x, location.Y() + delta_y};
    set_location(new_location);
}
/////////////////////////////////////////////////////
/// CASE OF PERSON MOVING TO A NON-HOME LOCATION ////
/////////////////////////////////////////////////////
void Person::move_toward()
// move a person by an amount determined by the current speed slightly varying the angle
{
    update_speed();
    double x_displacement = speed_x() * TIME_STEP; // x = v_x * ∆t
    double y_displacement = speed_y() * TIME_STEP; // y = v_y * ∆t
    double displacement = sqrt(x_displacement * x_displacement + y_displacement * y_displacement);

    if (location.distance(target) < displacement)
    {
        set_location(target);
        at_place = true;
        return;
    }

    double delta_x = std::abs(get_location().X() - target.X());
    double delta_y = std::abs(get_location().Y() - target.Y());
    double theta = atan(delta_y / delta_x); // angle connecting the target through a straight line

    std::random_device rd;
    std::mt19937 gen(rd());
    // generate an angle between velocity vector direction and the target's one so that will not point precisely to the
    // target
    std::uniform_real_distribution<double> angle(direction(), theta);

    double final_angle = angle(gen);
    // calculate new displacement replacing the previous velocity vector with a new one with the same magnitude but
    // different direction so recalculating the new v_x and v_y to determine x_displacement and y_displacement
    x_displacement = speed() * cos(final_angle) * TIME_STEP;
    y_displacement = speed() * sin(final_angle) * TIME_STEP;

    Location new_location{location.X() + x_displacement, location.Y() + y_displacement};

    set_location(new_location);
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
int determine_fill_size(const Person& person)
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
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> rand(starting_index, ending_index);

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
    int random_index = 0;
    for (int i = 0; i < missing_waypoints; ++i)
    {
        random_index = rand(gen);
        for (unsigned long j = 0; j < already_chosen_indeces.size(); ++j)
        {
            if (already_chosen_indeces[j] == random_index)
            {
                random_index = rand(gen); // try a new one
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

    std::random_device rd; // set the seed
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> rand(0.0, 1.0);

    double u = rand(gen); // number in range [0,1)

    term1 =
        (u * pow(MAX_PAUSE, PAUSE_EXPONENT)) - (u * pow(MIN_PAUSE, PAUSE_EXPONENT)) - pow(MAX_PAUSE, PAUSE_EXPONENT);
    term2 = pow(MAX_PAUSE, PAUSE_EXPONENT) * pow(MIN_PAUSE, PAUSE_EXPONENT);
    term3 = -(term1 / term2);
    term4 = pow(term3, (-1 / PAUSE_EXPONENT));
    pause_time = term4;

    return round(pause_time);
}

} // namespace SMOOTH
