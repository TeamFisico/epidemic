#include "person.hpp"
#include "simulation.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

namespace smooth_simulation
{
/////////////////////////////////////////////////////
////////        PERSON CONSTRUCTOR            ///////
/////////////////////////////////////////////////////
Person::Person(Status status, int cluster_label, Location home, Position current_position, int target_index,
               bool is_at_home, bool going_home, bool changed_status,int stay_time)
       : status{status, status},
         label{cluster_label},
         home{home},
         position{current_position},
         target_i{target_index},
         at_home{is_at_home},
         going_home{going_home},
         changed_status{changed_status},
         stay_counter{stay_time}
{
}

const Person& def_person()
{
    static Location def_loc{};
    static Position def_pos{};
    static Person def_p{Status::Susceptible, 0, def_loc, def_pos, 0, true, false, false, 0};
    return def_p;
}
Person::Person()
       : status{def_person().status[0], def_person().status[1]},
         label{def_person().label},
         home{def_person().home},
         position{def_person().position},
         target_i{def_person().target_i},
         at_home{def_person().at_home},
         going_home{def_person().going_home},
         changed_status{def_person().changed_status},
         stay_counter{def_person().stay_counter}
{
}
/////////////////////////////////////////////////////
/////       SET CURRENT LOCATION ==  HOME      //////
/////////////////////////////////////////////////////
void Person::set_at_home()
{
    position = home.get_position();
}
/////////////////////////////////////////////////////
/////    IS THE PERSON AT TARGET LOCATION      //////
/////////////////////////////////////////////////////
bool Person::at_target_location()
{
    return Simulation::Waypoints[target_i].in_radius(position);
}

///////////////////////////////////////////////////////
//////////            TARGET UPDATE             ///////
///////////////////////////////////////////////////////
//void Person::update_target(Random& engine)
//// use the Least action trip planning(LATP) algorithm to determine new person target
//{
//    //TODO think if making the case of out-of-his cluster
//    double LATP_parameter = Simulation::Clusters[label].LATP_parameter(); // LATP parameter from the person cluster
//
//    std::vector<double> distances;
//    std::vector<double> probabilities;
//    distances.reserve(Paths_i.size());     // allocate the space
//    probabilities.reserve(Paths_i.size()); // allocate the space
//
//    double current_weight = 0.0;
//
//    for (int& index : Paths_i) // compute distances and calculate weight
//    {
//        Location const& waypoint = Simulation::Waypoints[index];
//        current_weight = weight_function(position.distance_to(waypoint.get_position()), LATP_parameter);
//        distances.push_back(current_weight);
//    }
//
//    double sum = std::accumulate(std::begin(distances), std::end(distances), 0.0);
//
//    for (double curr_weight : distances)
//    {
//        probabilities.push_back(curr_weight / sum);
//    }
//
//    // choose an index through discrete distribution(weights correspond to the just calculated probabilities)
//    int chosen_cl_index = engine.discrete(probabilities);
//    int& chosen_target_index = Paths_i[chosen_cl_index];     // ref to corresponging waypoint
//
//    // is the waypoint still in a white cluster?
//    Zone chosen_wpt_zone = Simulation::Clusters[Simulation::Waypoints[chosen_target_index].get_label()].zone_type();
//    if (chosen_wpt_zone == Zone::White) // ok nothing's changed
//    {
//        target_i = chosen_target_index;
//        return;
//    }
//    else // now the zone is not White anymore
//    {
//        std::cout << " The wpt is not in a white cluster anymore" << std::endl;
//        remove_by_ref<int>(Paths_i, chosen_target_index);  // remove the bad target from Paths_i
//        update_target(engine);                          // reapply the LATP algorithm
//    }
//}
/////////////////////////////////////////////////////
///   REMOVE VISITED LOCATION INDEX FROM PATH     ///
/////////////////////////////////////////////////////
// remove the target_index by value: find it in Paths_i vector and then remove it
void Person::remove_target_i(int target_i)
{
    remove_by_value<int>(Paths_i,target_i);
}
///////////////////////////////////////////////////////
//////////     CASE OF DIRECT HOME FLIGHT       ///////
///////////////////////////////////////////////////////
//// move a person in a straight line to its home location
//// when the person arrives refill path
//void Person::move_home(Random& engine)
//{
//    // compute data for shortest possible path
//    double dx = position.get_X() - home.get_X();
//    double dy = position.get_Y() - home.get_Y();
//    double theta = atan2(dy,dx); // angle connecting the target through a straight line
//
//    // vary uniformly the angle in a range defined by MAXIMUM_ANGLE_VARIATION
//    double delta_theta = engine.uniform(-1.0 * MAXIMUM_ANGLE_VARIATION, MAXIMUM_GROUP_PROBABILITY);
//
//    while(position.distance_to(home.get_position()) < speed * TIME_STEP) //the speed is too high
//    {
//        update_speed(engine);
//    }
//    //coordinate of the translation vector
//    double v_x = speed * cos(theta + delta_theta) * TIME_STEP; // person's x_displacement: v_x*delta_t
//    double v_y = speed * sin(theta + delta_theta) * TIME_STEP; // person's x_displacement: v_y*delta_t
//
//    Position new_pos = {position.get_X() + v_x, position.get_Y() + v_y};
//
//    // has the person arrived?
//    if (new_pos.in_radius(home.get_position(), HOME_RADIUS))
//    {
//        set_at_home();                     // set new position
//        at_place = true;                   // the person is now at a place
//        stay_counter = engine.rand_stay(); // how much time he/she will spend there
//        going_home = false;
//    }
//    else  //the person hasn't gotten home
//    {
//        position = new_pos;
//    }
//}
///////////////////////////////////////////////////////
///// CASE OF PERSON MOVING TO A NON-HOME LOCATION ////
///////////////////////////////////////////////////////
//void Person::move_toward(Random& engine)
//// move a person by an amount determined by the current speed slightly varying the angle
//{
//    Location& target = Simulation::Waypoints[target_i];  // reference to current target
//
//    // compute data for shortest possible path
//    double dx = position.get_X() - target.get_X();
//    double dy = position.get_Y() - target.get_Y();
//    double theta = atan2(dy , dx); // angle connecting the target through a straight line
//    // vary uniformly the angole in a range defined by MAXIMUM_ANGLE_VARIATION
//    double delta_theta = engine.uniform(-1.0 * MAXIMUM_ANGLE_VARIATION, MAXIMUM_GROUP_PROBABILITY);
//
//    while(position.distance_to(target.get_position()) < speed * TIME_STEP) //the speed is too high
//    {
//        update_speed(engine);
//    }
//    //coordinates of the translation vector
//    double v_x = speed * cos(theta + delta_theta) * TIME_STEP;  // v_x*delta_t
//    double v_y = speed * sin(theta + delta_theta) * TIME_STEP;  // v_y*delta_t
//
//    Position new_pos = {position.get_X() + v_x, position.get_Y() + v_y};
//
//    if (new_pos.in_radius(target.get_position(), target.get_radius())) // the person has arrived to the target
//    {
//        position = new_pos;             // set new position
//        at_place = true;                   // the person is now at a place
//        stay_counter = engine.rand_stay(); // how much time he/she will spend there
//        remove_target_i(target_i);         // remove target from path
//    }
//    else //the person han't gotten to the target
//    {
//        position = new_pos;
//    }
//}
/////////////////////////////////////////////////////
////////            MOVE A PERSON             ///////
/////////////////////////////////////////////////////
void Person::move(Random& engine)
{
    if (going_home)
    {
        position.move_toward(home.get_position(),engine.rand_speed(),engine);
    }
    else
    {
    Position const& target_pos = Simulation::Waypoints[target_i].get_position();
    position.move_toward(target_pos,engine.rand_speed(),engine);
    }
//    //VECCHIO MOVE
//    if (at_place)
//    {
//        if (is_staying()) // still has to wait
//        {
//            --stay_counter;
//            return;
//        }
//        else // the waiting time is up
//        {
//            if (at_home()) // is at home
//            {
//                pathfinder(engine); // fill targets path
//                update_target(engine);
//                move_toward(engine);
//                at_place = false;
//                return;
//            }
//            else // the person is at a place other than home
//            {
//                if (empty_path()) // there are no more available waypoints
//                {
//                    at_place = false;
//                    going_home = true;
//                    move_home(engine);
//                    return;
//                }
//                else // there are available targets
//                {
//                    update_target(engine);
//                    move_toward(engine);
//                    return;
//                }
//            }
//        }
//    }
//    else // not at a place
//    {
//        if (going_home)
//        {
//            move_home(engine);
//            return;
//        }
//        else //moving to a general target
//        {
//            move_toward(engine);
//            return;
//        }
//    }
}
/////////////////////////////////////////////////////
////////         CAN THE PERSON MOVE?         ///////
/////////////////////////////////////////////////////
// return true if the person has to spend other time(steps) to a place
bool Person::is_staying()
{
    if (stay_counter > 0) {return true; }
    else{ return false; }
}
/////////////////////////////////////////////////////
////////              UPDATE STATUS           ///////
/////////////////////////////////////////////////////
// updates person status: the new one become current one
void Person::update_status()
{
    status[0] = status[1];
}
/////////////////////////////////////////////////////
///   FIND PATHS FOR PEOPLE IN WHITE CLUSTERS     ///
/////////////////////////////////////////////////////
// Fills person 's Path_i with indeces to waypoints. Total waypoints== VISITING_PERCENTAGE * cluster_size
// Case no other white cluster is available : fill with wpts just from the person homw cluster
// Case there are other white clusters available : fill path with 50% wpts from the home clusters and the other 50%
// waypoints taken from other white clusters proportionally to their sizes. In particular the weight of the cluster
// of the person is chosen so that the probability to get a wpt from there is 1- OTHER_CLUSTERS_PROBABILITY
void Person::pathfinder_white(Random& engine)
{
    std::vector<int> white_clust_labels = available_white_clusters(label);
    if (white_clust_labels.empty())
    {
        const int n_waypoints = (int)(VISITING_PERCENTAGE_WHITE * Simulation::Clusters[label].size());

        // now picking n_waypoints from person home cluster
        std::vector<int> already_chosen; // contains already chosen waypoints indeces

        int waypoint_index = 0;
        int lw_index = Simulation::Clusters[label].lower_index();
        int up_index = Simulation::Clusters[label].upper_index();
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
            add_to_path(waypoint_index);
            already_chosen.push_back(waypoint_index); // take track of it
        }
    }
    else // there are available white clusters
    {
        std::vector<double> weights; // will contain cluster's weights
        // now fill weights vector:the last element will account for the person's cluster's calculated weight
        weights_fill(*this, white_clust_labels, weights);

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
            add_to_path(waypoint_index);
            already_chosen.push_back(waypoint_index); // take track of it
        }
    }
}
/////////////////////////////////////////////////////
///   FIND PATHS FOR PEOPLE IN YELLOW CLUSTERS    ///
/////////////////////////////////////////////////////
void Person::pathfinder_yellow(Random& engine)
{
    const int n_waypoints = (int)(VISITING_PERCENTAGE_YELLOW * Simulation::Clusters[label].size());

    // now picking n_waypoints from person home cluster
    std::vector<int> already_chosen; // contains already chosen waypoints indeces

    int waypoint_index = 0;
    int lw_index = Simulation::Clusters[label].lower_index();
    int up_index = Simulation::Clusters[label].upper_index();
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
        add_to_path(waypoint_index);
        already_chosen.push_back(waypoint_index); // take track of it
    }
}
/////////////////////////////////////////////////////
///   FIND PATHS FOR PEOPLE IN ORANGE CLUSTERS    ///
/////////////////////////////////////////////////////
void Person::pathfinder_orange(Random& engine)
{
    const int n_waypoints = (int)(VISITING_PERCENTAGE_ORANGE * Simulation::Clusters[label].size());

    // now picking n_waypoints from person home cluster
    std::vector<int> already_chosen; // contains already chosen waypoints indeces

    int waypoint_index = 0;
    int lw_index = Simulation::Clusters[label].lower_index();
    int up_index = Simulation::Clusters[label].upper_index();
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
        add_to_path(waypoint_index);
        already_chosen.push_back(waypoint_index); // take track of it
    }
}
/////////////////////////////////////////////////////
///   FIND PATHS FOR PEOPLE IN RED CLUSTERS       ///
/////////////////////////////////////////////////////
// fills like path_finder_white but n_waypoints is 15% the ones in white zone
void Person::pathfinder_red(Random& engine)
{
    const int n_waypoints = (int)(VISITING_PERCENTAGE_RED * Simulation::Clusters[label].size());

    // now picking n_waypoints from person home cluster
    std::vector<int> already_chosen; // contains already chosen waypoints indeces

    int waypoint_index = 0;
    int lw_index = Simulation::Clusters[label].lower_index();
    int up_index = Simulation::Clusters[label].upper_index();
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
        add_to_path(waypoint_index);
        already_chosen.push_back(waypoint_index); // take track of it
    }
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
/////////////////////////////////////////////////////
/// REMOVE WPTS FROM PATHNOT IN WHITE ZONE ANYMORE///
/////////////////////////////////////////////////////
void Person::clean_path()
{
    for (auto& wpt_i : Paths_i)
    {
        //zone where the i-th wpt in person.Paths_i is located
        Zone const& wpt_zone = Simulation::Clusters[Simulation::Waypoints[wpt_i].get_label()].zone_type();
        if (wpt_zone != Zone::White)
        {
            remove_by_ref<int>(Paths_i,wpt_i); //remove from path
        }
    }
}
/////////////////////////////////////////////////////
///    SELECT NEXT LOCATION VIA LATP ALGORITHM    ///
/////////////////////////////////////////////////////
void Person::next_location(Random& engine)
{
    if(going_home) //case when person is at home with target location home
    {
        at_home = true;
        going_home = false;
        stay_counter = engine.rand_stay(); //TODO set random stay from 2 to 5, to change into the power rule
    }
    if (Paths_i.empty()) // if Path vector empty select home
    {
//        target_location = person.get_home();
        going_home = true;
    }
    else if (Paths_i.size() == 1)
    { // if Path ha only one element select that element
        target_i = Paths_i[0];
        Paths_i.clear();
        stay_counter = engine.rand_stay();
    }
    else // if Path vector has more than one element ran the LATP Algorithm to select next Location
    {
        //TODO reserve space for vectors
        double LATP_alpha = Simulation::Clusters[label].LATP_parameter();
        std::vector<double> inverse_distances; // vector where we store the inverse_distance elevated to alpha of the
        // pointed location with the same index in Path

        for (auto& wpt_i : Paths_i) // fill the inverse_distances vector
        {
            Position const& curr_position = Simulation::Waypoints[wpt_i].get_position();
            double dist = 1 / pow(curr_position.distance_to(position), LATP_alpha);
            inverse_distances.push_back(dist);
        }
        std::vector<double> probabilities; // vector where we store the probabilities of the same index in Path
        // calculated with LATP algorithm
        // calculate the denominator for the probabilities formula of the LATP algorithm
        double denom = std::accumulate(inverse_distances.begin(), inverse_distances.end(),0);
        for (auto &a : inverse_distances)
        { // fill the probabilities vector
            probabilities.push_back(a / denom);
        }
        // select next_location to visit based on the probabilities vector
        int path_index_result = engine.discrete(probabilities); //chosen index of Paths_i
        int& wpt_index_result = Paths_i[path_index_result];             //corresponding wpt index
        target_i = wpt_index_result;      //corresponding waypoint

        //remove the target from path
        remove_by_ref(Paths_i,wpt_index_result);
        stay_counter = engine.rand_stay();
    }
}
/////////////////////////////////////////////////////
///   REMOVE ELEMENT FROM A VECTOR BY REFERENCE   ///
/////////////////////////////////////////////////////
// remove an element from a vector having a reference of it. Since not caring about order just swaps the values of the
// reference element ahd the last one and then eliminates the last-->Prevent moving elements in a new resized vector
template<class T>
void remove_by_ref(std::vector<T>& vec, T& to_remove)
{
    std::swap(to_remove, vec.back());
    vec.pop_back();
}
/////////////////////////////////////////////////////
///     REMOVE ELEMENT FROM A VECTOR BY VALUE     ///
/////////////////////////////////////////////////////
// remove an element from a vector knowing its value. Finds the corresponding element and,since not caring about order,
// just swaps the values of the element to remove ahd the last one and then eliminates the last-->Prevent moving elements in a new resized vector
template<class T>
void remove_by_value(std::vector<T>& vec, T to_remove)
{
    auto it = std::find(std::begin(vec),std::end(vec),to_remove);
    std::swap(*it, vec.back());
    vec.pop_back();

}
} // namespace smooth_simulation














