#include "person.hpp"
#include "simulation.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

namespace smooth_simulation
{
Person::Person(Status status, int cluster_label, Location home, Location current_location, Location target_location,
               bool is_at_place, bool going_home, double x_speed, double y_speed, int stay_time)
    : status{status, status}, label{cluster_label}, home{home}, location{current_location}, target{target_location},
      at_place{is_at_place}, going_home{going_home}, stay_counter{stay_time}, velocity{x_speed, y_speed}
{
}
const Person& default_person()
{
    static Location def_loc{};
    static Person def_per{Status::Susceptible, 0, def_loc, def_loc, def_loc, false, false, 0.0, 0.0, 0};
    return def_per;
}
Person::Person()
    : status{default_person().status[0], default_person().status[1]}, label{default_person().label},
      home{default_person().home}, location{default_person().location}, target{default_person().target},
      at_place{default_person().at_place}, stay_counter{default_person().stay_counter}, velocity{STARTING_VELOCITY[0],
                                                                                                 STARTING_VELOCITY[1]}
{
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
////////          VELOCITY UPDATE             ///////
/////////////////////////////////////////////////////
// uniformly generate a new velocity stddev in the range [0,MAXIMUM_VELOCITY_STDDEV) and then extract
// the x and y components from a normal distributions centerd on AVERAGE_VELOCITY
void Person::update_velocity(Random& engine)
{
    velocity[0] = engine.rand_speed();
    velocity[1] = engine.rand_speed();
}

/////////////////////////////////////////////////////
////////            TARGET UPDATE             ///////
/////////////////////////////////////////////////////
void Person::update_target(Random& engine)
// use the Least action trip planning(LATP) algorithm to determine new person target
{
    double LATP_parameter = Simulation::Clusters[label].LATP_parameter(); // LATP parameter from the person cluster

    std::vector<double> distances;
    std::vector<double> probabilities;
    distances.reserve(Paths_i.size());     // allocate the space
    probabilities.reserve(Paths_i.size()); // allocate the space
    double current_weight = 0.0;

    for (int& index : Paths_i) // compute distances and calculate weight
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
    // select a waypoint index weighted on the just calculated probabilities
    int i =
        engine.discrete(probabilities); // extract an number from 1 to weights.size()-1 based on weights(probabilities)
    int index = Paths_i[i];             // correspondin waypoint index
    // Still need to check if this waypoint is inside a White Cluster:things could've changed in the meanwhile
    Zone waypoint_zone = Simulation::Clusters[Simulation::Waypoints[index].get_label()].zone_type();
    if (waypoint_zone == Zone::White) // ok nothing's changed
    {
        set_target(Simulation::Waypoints[index]);
        return;
    }
    else // now the zone is not White anymore
    {
        remove_target_index(*this, index);
        update_target(engine); // apply again LATP algo and look for a new one
    }
    // TODO TESTING
}
/////////////////////////////////////////////////////
////////     CASE OF DIRECT HOME FLIGHT       ///////
/////////////////////////////////////////////////////
// move a person in a straight line to its home location
// when the person arrives refill path
void Person::move_home(Random& engine)
{
    // compute data for shortest possible path
    double delta_x = std::abs(location.get_X() - target.get_X());
    double delta_y = std::abs(location.get_Y() - target.get_Y());
    double direct_angle = atan(delta_y / delta_x); // angle connecting the target through a straight line

    update_velocity(engine); // assign this person a new velocity vector
    // now determine the displacement on each axis based on the current person speed
    double x_displacement = (speed() * cos(direct_angle)) * TIME_STEP; // moving of delta_x on the x axis
    double y_displacement = (speed() * sin(direct_angle)) * TIME_STEP; // moving of delta_y on the y axis

    Position new_position{location.get_X() + x_displacement, location.get_Y() + y_displacement};

    // check if the person will enter the target location radius
    if (new_position.in_radius(home.get_position(), HOME_RADIUS)) // the person has arrived home
    {
        location.set_position(home.get_position()); // set new position
        at_place = true;                            // the person is now at a place
        stay_counter = engine.rand_stay();          // how much time he/she will spend there
        going_home = false;
    }

    location.set_position(new_position); // set new position
}
/////////////////////////////////////////////////////
/// CASE OF PERSON MOVING TO A NON-HOME LOCATION ////
/////////////////////////////////////////////////////
void Person::move_toward(Random& engine)
// move a person by an amount determined by the current speed slightly varying the angle
{
    // compute data for shortest possible path
    double delta_x = std::abs(location.get_X() - target.get_X());
    double delta_y = std::abs(location.get_Y() - target.get_Y());
    double direct_angle = atan(delta_y / delta_x); // angle connecting the target through a straight line

    update_velocity(engine); // assign this person a new velocity vector

    // generate an angle between velocity vector direction and the target's one so that will not point precisely to the
    // target
    // TODO valuta se ammettere una variazione uniforme di 5 gradi in entrambi i sensi
    double final_angle = 0.0;
    if (direct_angle >= direction()) { final_angle = engine.uniform(direction(), direct_angle); }
    else
    {
        final_angle = engine.uniform(direct_angle, direction());
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
        stay_counter = engine.rand_stay();   // how much time he/she will spend there
                                             // erase the current target from Paths_i
        remove_target(*this, target);        // remove target from path
        return;
    }

    location.set_position(new_position); // set new position
}
/////////////////////////////////////////////////////
////////            MOVE A PERSON             ///////
/////////////////////////////////////////////////////
void Person::move()
{
    Random engine; // seeded engine to use for this person
    if (at_place)
    {
        if (is_staying()) // still has to wait
        {
            --stay_counter;
            return;
        }
        else // the waiting time is up
        {
            if (at_home()) // is at home
            {
                pathfinder(engine); // refill path
                update_target(engine);
                move_toward(engine);
                return;
            }
            else // the person is at a place other than home
            {
                if (empty_path()) // there are no more available waypoints
                {
                    going_home = true;
                    move_home(engine);
                    return;
                }
                else // there are available targets
                {
                    update_target(engine);
                    move_toward(engine);
                    return;
                }
            }
        }
    }
    else // not at a place
    {
        if (going_home)
        {
            move_home(engine);
            return;
        }
        else
        {
            move_toward(engine);
            return;
        }
    }
}
/////////////////////////////////////////////////////
////////         CAN THE PERSON MOVE?         ///////
/////////////////////////////////////////////////////
// return true if the person's time to spend to the current place is up
bool Person::is_staying()
{
    if (stay_counter == 0) return true;
    else
        return false;
}
/////////////////////////////////////////////////////
////////              UPDATE STATUS           ///////
/////////////////////////////////////////////////////
//updates person status: the new one become current one
void Person::update_status()
{
    status[0] = status[1];
}
//*************************************************** HELPER FUNCTIONS
//*************************************************************//
/////////////////////////////////////////////////////
////////            PATHFINDER                ///////
/////////////////////////////////////////////////////
void Person::pathfinder(Random& engine)
{
    Zone const& zone = Simulation::Clusters[label].zone_type();
    switch (zone)
    {
    case Zone::White:
        pathfinder_white(*this, engine);
        break;
    case Zone::Yellow:
        pathfinder_yellow(*this, engine);
        break;
    case Zone::Orange:
        pathfinder_orange(*this, engine);
        break;
    case Zone::Red:
        pathfinder_red(*this, engine);
        break;
    default:
        break;
    }
}

/////////////////////////////////////////////////////
////////      DETERMINE HOW TO FILL PATH      ///////
/////////////////////////////////////////////////////
// return true if there are any white clusters available
bool are_white_available(const Person& person)
{
    for (auto& cluster : Simulation::Clusters)
    {
        if (cluster.zone_type() == Zone::White && cluster.label() != person.home_cluster()) { return true; }
    }
    return false;
}
/////////////////////////////////////////////////////
///   FIND PATHS FOR PEOPLE IN WHITE CLUSTERS     ///
/////////////////////////////////////////////////////
// Fills person 's Path_i with indeces to waypoints. Total waypoints== VISITING_PERCENTAGE * cluster_size
// Case no other white cluster is available : fill with wpts just from the person homw cluster
// Case there are other white clusters available : fill path with 50% wpts from the home clusters and the other 50%
// waypoints taken from other white clusters proportionally to their sizes. In particular the weight of the cluster
// of the person is chosen so that the probability to get a wpt from there is 1- OTHER_CLUSTERS_PROBABILITY
void pathfinder_white(Person& person, Random& engine)
{
    std::vector<int> white_clust_labels = available_white_clusters(person.home_cluster());
    if (white_clust_labels.empty())
    {
        const int n_waypoints = (int)(VISITING_PERCENTAGE_WHITE * Simulation::Clusters[person.home_cluster()].size());

        // now picking n_waypoints from person home cluster
        std::vector<int> already_chosen; // contains already chosen waypoints indeces

        int waypoint_index = 0;
        int lw_index = Simulation::Clusters[person.home_cluster()].lower_index();
        int up_index = Simulation::Clusters[person.home_cluster()].upper_index();
        for (int i = 0; i < n_waypoints; ++i)
        {
            waypoint_index = engine.int_uniform(lw_index, up_index); // uniformly extract an index
            for (unsigned long j = 0; j < already_chosen.size(); ++j)
            {
                if (waypoint_index == already_chosen[j]) // already chosen!
                {
                    waypoint_index = engine.int_uniform(lw_index, up_index); // try with a new one
                    j = 0;
                    continue; // restart the loop
                }
            }
            person.Paths_i.push_back(waypoint_index); // add it to path
            already_chosen.push_back(waypoint_index); // take track of it
        }
    }
    else // there are available white clusters
    {
        std::vector<double> weights; // will contain cluster's weights
        // now fill weights vector:the last element will account for the person's cluster's calculated weight
        weights_fill(person, white_clust_labels, weights);

        std::vector<int> already_chosen; // contains already chosen waypoints indeces
        // picking waypoints according to weights through discrete distribution
        int waypoint_index = 0;
        for (int& clust_index : white_clust_labels)
        {
            int lw_index = Simulation::Clusters[clust_index].lower_index();
            int up_index = Simulation::Clusters[clust_index].upper_index();
            waypoint_index = engine.int_uniform(lw_index, up_index); // uniformly extract an index
            for (unsigned long i = 0; i < already_chosen.size(); ++i)
            {
                if (waypoint_index == already_chosen[i]) // already chosen!
                {
                    waypoint_index = engine.int_uniform(lw_index, up_index); // try with a new one
                    i = 0;
                    continue; // restart the loop
                }
            }
            person.Paths_i.push_back(waypoint_index); // add it to path
            already_chosen.push_back(waypoint_index); // take track of it
        }
    }
}
/////////////////////////////////////////////////////
///   FIND PATHS FOR PEOPLE IN YELLOW CLUSTERS    ///
/////////////////////////////////////////////////////
void pathfinder_yellow(Person& person, Random& engine)
{
    const int n_waypoints = (int)(VISITING_PERCENTAGE_YELLOW * Simulation::Clusters[person.home_cluster()].size());

    // now picking n_waypoints from person home cluster
    std::vector<int> already_chosen; // contains already chosen waypoints indeces

    int waypoint_index = 0;
    int lw_index = Simulation::Clusters[person.home_cluster()].lower_index();
    int up_index = Simulation::Clusters[person.home_cluster()].upper_index();
    for (int i = 0; i < n_waypoints; ++i)
    {
        waypoint_index = engine.int_uniform(lw_index, up_index); // uniformly extract an index
        for (unsigned long j = 0; j < already_chosen.size(); ++j)
        {
            if (waypoint_index == already_chosen[j]) // already chosen!
            {
                waypoint_index = engine.int_uniform(lw_index, up_index); // try with a new one
                j = 0;
                continue; // restart the loop
            }
        }
        person.Paths_i.push_back(waypoint_index); // add it to path
        already_chosen.push_back(waypoint_index); // take track of it
    }
}
/////////////////////////////////////////////////////
///   FIND PATHS FOR PEOPLE IN ORANGE CLUSTERS    ///
/////////////////////////////////////////////////////
void pathfinder_orange(Person& person, Random& engine)
{
    const int n_waypoints = (int)(VISITING_PERCENTAGE_ORANGE * Simulation::Clusters[person.home_cluster()].size());

    // now picking n_waypoints from person home cluster
    std::vector<int> already_chosen; // contains already chosen waypoints indeces

    int waypoint_index = 0;
    int lw_index = Simulation::Clusters[person.home_cluster()].lower_index();
    int up_index = Simulation::Clusters[person.home_cluster()].upper_index();
    for (int i = 0; i < n_waypoints; ++i)
    {
        waypoint_index = engine.int_uniform(lw_index, up_index); // uniformly extract an index
        for (unsigned long j = 0; j < already_chosen.size(); ++j)
        {
            if (waypoint_index == already_chosen[j]) // already chosen!
            {
                waypoint_index = engine.int_uniform(lw_index, up_index); // try with a new one
                j = 0;
                continue; // restart the loop
            }
        }
        person.Paths_i.push_back(waypoint_index); // add it to path
        already_chosen.push_back(waypoint_index); // take track of it
    }
}
/////////////////////////////////////////////////////
///   FIND PATHS FOR PEOPLE IN RED CLUSTERS       ///
/////////////////////////////////////////////////////
// fills like path_finder_white but n_waypoints is 15% the ones in white zone
void pathfinder_red(Person& person, Random& engine)
{
    const int n_waypoints = (int)(VISITING_PERCENTAGE_RED * Simulation::Clusters[person.home_cluster()].size());

    // now picking n_waypoints from person home cluster
    std::vector<int> already_chosen; // contains already chosen waypoints indeces

    int waypoint_index = 0;
    int lw_index = Simulation::Clusters[person.home_cluster()].lower_index();
    int up_index = Simulation::Clusters[person.home_cluster()].upper_index();
    for (int i = 0; i < n_waypoints; ++i)
    {
        waypoint_index = engine.int_uniform(lw_index, up_index); // uniformly extract an index
        for (unsigned long j = 0; j < already_chosen.size(); ++j)
        {
            if (waypoint_index == already_chosen[j]) // already chosen!
            {
                waypoint_index = engine.int_uniform(lw_index, up_index); // try with a new one
                j = 0;
                continue; // restart the loop
            }
        }
        person.Paths_i.push_back(waypoint_index); // add it to path
        already_chosen.push_back(waypoint_index); // take track of it
    }
}
/////////////////////////////////////////////////////
///      REMOVE VISITED LOCATION FROM PATH        ///
/////////////////////////////////////////////////////
// remove a visited target from person.Path: since we don't care about the targets order, move the one to remove to
// the end and remove it preventing moving all items after it
void remove_target(Person& person, Location to_remove)
{
    for (int& targ_index : person.Paths_i)
    {
        if (Simulation::Waypoints[targ_index] == to_remove)
        {
            std::swap(targ_index, person.Paths_i.back()); // swap the last element with the one to be removed
            person.Paths_i.pop_back();                    // erase the last element of the vector
            return;
        }
    }
    // if we get here there's a problem
    std::cerr << "The target is not in person.Paths_i!.Aborting.";
}
/////////////////////////////////////////////////////
///   REMOVE VISITED LOCATION INDEX FROM PATH     ///
/////////////////////////////////////////////////////
// remove a visited target index from person.Path: since we don't care about the targets order, move the one to remove
// to the end and remove it preventing moving all items after it
void remove_target_index(Person& person, int index_to_remove)
{
    std::swap(index_to_remove, person.Paths_i.back()); // swap the last element with the one to be removed
    person.Paths_i.pop_back();                         // erase the last element of the vector
}
/////////////////////////////////////////////////////
///   WEIGHTS COMPUTATION FOR PATHFINDER_WHITE    ///
/////////////////////////////////////////////////////
// calculate weights for each cluster so to respect the condition of OTHER_CLUSTERS_PROBABILITY
void weights_fill(Person const& person, std::vector<int>& white_labels, std::vector<double>& weights)
{
    assert(weights.empty());                  // gotta make sure weights vector is empty since it'll be filled
    weights.reserve(white_labels.size() + 1); // allocate the needed space

    double person_cluster_weight = 0.0;

    // fill the first white_clust.size() elements w/ other white clusters weights
    for (int& index : white_labels)
    {
        double current_weight = Simulation::Clusters[index].weight();
        weights.push_back(current_weight);
        person_cluster_weight += current_weight; // add upp the the weights
    }

    // this way this cluster's waypoints will account for the right percentage in the extraction
    person_cluster_weight *= (1 - OTHER_CLUSTERS_PROBABILITY) / OTHER_CLUSTERS_PROBABILITY;

    // now adding this person's cluster weight
    white_labels.push_back(person.home_cluster());
    weights.push_back(person_cluster_weight); // last weight corresponds to the person's cluster's one
}

} // namespace smooth_simulation
