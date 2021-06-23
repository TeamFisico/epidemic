#include "person.hpp"
#include "simulation.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>

namespace smooth_simulation
{
Person::Person(Status status, int cluster_label, Location home, Location current_location, Location target_location,
               bool is_at_place, bool going_home,double x_speed, double y_speed, int stay_time)
    : status{status}, label{cluster_label}, home{home}, location{current_location}, target{target_location},
      at_place{is_at_place}, going_home{going_home},stay_counter{stay_time}
{
    velocity[0] = x_speed;
    velocity[1] = y_speed;
}
const Person& default_person()
{
    static Location def_loc{};
    static Person def_per{Status::Susceptible, 0, def_loc, def_loc, def_loc, false, false,0.0, 0.0, 0};
    return def_per;
}
Person::Person()
    : status{default_person().status}, label{default_person().label}, home{default_person().home},
      location{default_person().location}, target{default_person().target}, at_place{default_person().at_place},
      stay_counter{default_person().stay_counter}
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
void Person::update_target()
// use the Least action trip planning(LATP) algorithm to determine new person target
{
    double LATP_parameter = Simulation::Clusters[label].LATP_parameter();  //LATP parameter from the person cluster

    std::vector<double> distances;
    std::vector<double> probabilities;
    distances.reserve(Paths_i.size());     // allocate the space
    probabilities.reserve(Paths_i.size()); // allocate the space
    double current_weight = 0.0;

    for (int& index : Paths_i)     // compute distances and calculate weight
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
    //select a waypoint index weighted on the just calculated probabilities
    Random rng{};
    int i = rng.discrete(probabilities); //extract an number from 1 to weights.size()-1 based on weights(probabilities)
    int index = Paths_i[i]; //correspondin waypoint index
    // Still need to check if this waypoint is inside a White Cluster:things could've changed in the meanwhile
    Zone waypoint_zone = Simulation::Clusters[Simulation::Waypoints[index].get_label()].zone_type();
    if (waypoint_zone == Zone::White) //ok nothing's changed
    {
        set_target(Simulation::Waypoints[index]);
        return;
    }
    else //now the zone is not White anymore
    {
        remove_target_index(*this,index);
        update_target(); //apply again LATP algo and look for a new one
    }
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
    if (new_position.in_radius(home.get_position(),HOME_RADIUS)) // the person has arrived home
    {
        location.set_position(home.get_position()); // set new position
        at_place = true;                     // the person is now at a place
        stay_counter = generate_pause_time();       // how much time he/she will spend there
        going_home = false;
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
        stay_counter = generate_pause_time();       // how much time he/she will spend there
                                             //erase the current target from Paths_i
        remove_target(*this, target); //remove target from path
        return;
    }

    location.set_position(new_position); // set new position
}
/////////////////////////////////////////////////////
////////            MOVE A PERSON             ///////
/////////////////////////////////////////////////////
void Person::move()
{
    if (at_place)
    {
        if (is_staying())  //still has to wait
        {
            --stay_counter;
            return;
        }
        else            //the waiting time is up
        {
            if (at_home())  // is at home
            {
                //TODO FILL PATH refill path
                pathfinder(*this);
                update_target();
                move_toward();
                return;
            }
            else  //the person is at a place other than home
            {
                if (empty_path()) //there are no more available waypoints
                {
                    going_home = true;
                    move_home();
                    return;
                }
                else  //there are available targets
                {
                    update_target();
                    move_toward();
                    return;
                }
            }
        }
    }
    else  //not at a place
    {
        if (going_home)
        {
            move_home();
            return;
        }
        else
        {
            move_toward();
            return;
        }
    }
}
/////////////////////////////////////////////////////
////////         CAN THE PERSON MOVE?         ///////
/////////////////////////////////////////////////////
//return true if the person's time to spend to the current place is up
bool Person::is_staying()
{
    if (stay_counter == 0) return true;
    else return false;
}
//*************************************************** HELPER FUNCTIONS *************************************************************//
/////////////////////////////////////////////////////
////////            PATHFINDER                ///////
/////////////////////////////////////////////////////
void pathfinder(Person& person)
{
    Zone const& zone = Simulation::Clusters[person.home_cluster()].zone_type();
    switch (zone)
    {
    case Zone::White:
        pathfinder_white(person);
        break;
    case Zone::Yellow:
        pathfinder_yellow(person);
        break;
    case Zone::Orange:
        pathfinder_orange(person);
        break;
    case Zone::Red:
        pathfinder_red(person);
        break;
    default:
        break;
    }
}
/////////////////////////////////////////////////////
////////        DETERMINE PATH SIZE          ///////
/////////////////////////////////////////////////////
//TODO Delete this function cuz the path is filled only when it's empty

int determine_fill_size(Person const& person)
{
    int current_Paths_i_size = person.Paths_i.size();
    double selected_size = Simulation::Clusters[person.home_cluster()].size() * VISITING_PERCENTAGE; // Paths_i final size
    return static_cast<int>(selected_size - current_Paths_i_size);
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
///   FIND PATHS FOR PEOPLE IN WHITE CLUSTERS     ///
/////////////////////////////////////////////////////
// Fills person 's Path_i with indeces to waypoints. Total waypoints== VISITING_PERCENTAGE * cluster_size
//Case no other white cluster is available : fill with wpts just from the person homw cluster
//Case there are other white clusters available : fill path with 50% wpts from the home clusters and the other 50%
// waypoints taken from other white clusters proportionally to their sizes. In particular the weight of the cluster
// of the person is chosen so that the probability to get a wpt from there is 1- OTHER_CLUSTERS_PROBABILITY
void pathfinder_white(Person& person)
{
    std::vector<int> white_clust = available_white_clusters(person.home_cluster());
    if (white_clust.empty())
    {

    }else //there are available white clusters
    {
        std::vector<double> weights;
        weights.reserve(white_clust.size() + 1); //allocate the needed space

        //fill the first white_clust.size() elements w/ other white clusters weights
        for (int& index : white_clust)
        {
            double current_weight = Simulation::Clusters[index].weight();
            weights.push_back(current_weight);
        }

        double person_cluster_weight = 0.0;
        for (int& index : white_clust)
        {
            person_cluster_weight += Simulation::Clusters[index].weight(); //add i-th white clust weight
        }
        //this way this cluster's waypoints will account for the right percentage in the extraction
        person_cluster_weight *= (1 - OTHER_CLUSTERS_PROBABILITY) / OTHER_CLUSTERS_PROBABILITY;

        //now adding this person's cluster weight
        white_clust.push_back(person.home_cluster());
        weights.push_back(person_cluster_weight);

        //now extracting from the discrete distribution indeces i from 0...weights.size()-1
        // we then get the corresponding cluster index ,namely, white_clust[i]
        int n_waypoints = VISITING_PERCENTAGE * Simulation::Clusters[person.home_cluster()].size();
        std::vector<int> result; //vector containing labels of the chosen clusters to choose wpts from
        result.reserve(n_waypoints);

        Random rng{};  //seeded engine
        for (int i = 0; i < n_waypoints; ++i)
        {
           int white_clust_label = white_clust[rng.discrete(weights)];
           result.push_back(white_clust_label);
        }

        //TODO now use result vector to fill path
    }
}
/////////////////////////////////////////////////////
///   FIND PATHS FOR PEOPLE IN YELLOW CLUSTERS    ///
/////////////////////////////////////////////////////
void pathfinder_yellow(Person& person)
{}
/////////////////////////////////////////////////////
///   FIND PATHS FOR PEOPLE IN ORANGE CLUSTERS    ///
/////////////////////////////////////////////////////
void pathfinder_orange(Person& person)
{}
/////////////////////////////////////////////////////
///   FIND PATHS FOR PEOPLE IN RED CLUSTERS       ///
/////////////////////////////////////////////////////
void pathfinder_red(Person& person)
{}
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
    int missing_waypoints = determine_fill_size(person); // number of waypoints to add to person.Paths_i

    std::vector<int> already_chosen_indeces;

    person.Paths_i.reserve(missing_waypoints); // avoid reallocation when pushing back
    already_chosen_indeces.reserve(person.Paths_i.size() + person.Paths_i.size());

    assert(already_chosen_indeces.empty()); // TODO consider deleting
    // keep track of the indeces of the waypoints already in person.Paths_i
    for (int& taken_index : person.Paths_i)
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
        person.Paths_i.push_back(random_index);
    }
}

/////////////////////////////////////////////////////
//////        DETERMINE STAY AT A PLACE         /////
/////////////////////////////////////////////////////
// generating pause time at a place according to TPL(Truncated power Law) [see paper for details]
int generate_pause_time()
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
    for (int& targ_index : person.Paths_i)
    {
        if (Simulation::Waypoints[targ_index] == to_remove)
        {
            std::swap(targ_index,person.Paths_i.back()); //swap the last element with the one to be removed
            person.Paths_i.pop_back(); //erase the last element of the vector
            return;
        }
    }
    //if we get here there's a problem
    std::cerr<<"The target is not in person.Paths_i!.Aborting.";
}
//remove a visited target index from person.Path: since we don't care about the targets order, move the one to remove to
// the end and remove it preventing moving all items after it
void remove_target_index(Person& person,int index_to_remove)
{
     std::swap(index_to_remove,person.Paths_i.back()); //swap the last element with the one to be removed
     person.Paths_i.pop_back(); //erase the last element of the vector
}


} // namespace smooth_simulation



















