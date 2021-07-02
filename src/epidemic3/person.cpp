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
Person::Person(Status current_status,Status next_status, int cluster_label, Location home, Position current_position, int target_index,
               bool is_at_home, bool going_home,int stay_time)
       : curr_status{current_status},
         next_status{next_status},
         label{cluster_label},
         home{home},
         position{current_position},
         target_i{target_index},
         at_home{is_at_home},
         going_home{going_home},
         stay_counter{stay_time}
{
}

const Person& def_person()
{
    static Location def_loc{};
    static Position def_pos{};
    static Person def_p{Status::Susceptible,Status::Susceptible,0,def_loc,def_pos,0,true,false,0};
    return def_p;
}
Person::Person()
       : curr_status{def_person().curr_status},
         next_status{def_person().next_status},
         label{def_person().label},
         home{def_person().home},
         position{def_person().position},
         target_i{def_person().target_i},
         at_home{def_person().at_home},
         going_home{def_person().going_home},
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
/////////////////////////////////////////////////////
////////            MOVE A PERSON             ///////
/////////////////////////////////////////////////////
void Person::move(Random& engine)
{
    if (going_home) { position.move_toward(home.get_position(), engine.rand_speed(), engine); }
    else
    {
        Position const& target_pos = Simulation::Waypoints[target_i].get_position();
        position.move_toward(target_pos, engine.rand_speed(), engine);
    }
}
/////////////////////////////////////////////////////
////////         CAN THE PERSON MOVE?         ///////
/////////////////////////////////////////////////////
// return true if the person has to spend other time(steps) to a place
bool Person::is_staying()
{
    if (stay_counter > 0) { return true; }
    else{ return false; }
}
/////////////////////////////////////////////////////
////////              UPDATE STATUS           ///////
/////////////////////////////////////////////////////
// updates person status: the new one become current one
void Person::update_status()
{
    curr_status = next_status;
}
///////////////////////////////////////////////////////
/////   FIND PATHS FOR PEOPLE IN WHITE CLUSTERS     ///
///////////////////////////////////////////////////////
//// Fills person 's Path_i with indeces to waypoints. Total waypoints== VISITING_PERCENTAGE * cluster_size
//// Case no other white cluster is available : fill with wpts just from the person homw cluster
//// Case there are other white clusters available : fill path with 50% wpts from the home clusters and the other 50%
//// waypoints taken from other white clusters proportionally to their sizes. In particular the weight of the cluster
//// of the person is chosen so that the probability to get a wpt from there is 1- OTHER_CLUSTERS_PROBABILITY
//void Person::pathfinder_white(Random& engine)
//{
//    std::vector<int> white_clust_labels = available_white_clusters(label);
//    if (white_clust_labels.empty())
//    {
//        const int n_waypoints = (int)(VISITING_PERCENTAGE_WHITE * Simulation::Clusters[label].size());
//
//        // now picking n_waypoints from person home cluster
//        std::vector<int> already_chosen; // contains already chosen waypoints indeces
//
//        int waypoint_index = 0;
//        int lw_index = Simulation::Clusters[label].lower_index();
//        int up_index = Simulation::Clusters[label].upper_index();
//        for (int i = 0; i < n_waypoints; ++i)
//        {
//            waypoint_index = engine.int_uniform(lw_index, up_index); // uniformly extract an index
//            for (unsigned long j = 0; j < already_chosen.size(); ++j)
//            {
//                if (waypoint_index == already_chosen[j]) // already chosen!
//                {
//                    waypoint_index = engine.int_uniform(lw_index, up_index); // try with a new one
//                    j = 0;
//                    continue; // restart the loop
//                }
//            }
//            add_to_path(waypoint_index);
//            already_chosen.push_back(waypoint_index); // take track of it
//        }
//    }
//    else // there are available white clusters
//    {
//        std::vector<double> weights; // will contain cluster's weights
//        // now fill weights vector:the last element will account for the person's cluster's calculated weight
//        weights_fill(*this, white_clust_labels, weights);
//
//        std::vector<int> already_chosen; // contains already chosen waypoints indeces
//        // picking waypoints according to weights through discrete distribution
//        int waypoint_index = 0;
//        for (int& clust_index : white_clust_labels)
//        {
//            int lw_index = Simulation::Clusters[clust_index].lower_index();
//            int up_index = Simulation::Clusters[clust_index].upper_index();
//            waypoint_index = engine.int_uniform(lw_index, up_index); // uniformly extract an index
//            for (unsigned long i = 0; i < already_chosen.size(); ++i)
//            {
//                if (waypoint_index == already_chosen[i]) // already chosen!
//                {
//                    waypoint_index = engine.int_uniform(lw_index, up_index); // try with a new one
//                    i = 0;
//                    continue; // restart the loop
//                }
//            }
//            add_to_path(waypoint_index);
//            already_chosen.push_back(waypoint_index); // take track of it
//        }
//    }
//}
///////////////////////////////////////////////////////
/////   FIND PATHS FOR PEOPLE IN YELLOW CLUSTERS    ///
///////////////////////////////////////////////////////
//void Person::pathfinder_yellow(Random& engine)
//{
//    const int n_waypoints = (int)(VISITING_PERCENTAGE_YELLOW * Simulation::Clusters[label].size());
//
//    // now picking n_waypoints from person home cluster
//    std::vector<int> already_chosen; // contains already chosen waypoints indeces
//
//    int waypoint_index = 0;
//    int lw_index = Simulation::Clusters[label].lower_index();
//    int up_index = Simulation::Clusters[label].upper_index();
//    for (int i = 0; i < n_waypoints; ++i)
//    {
//        waypoint_index = engine.int_uniform(lw_index, up_index); // uniformly extract an index
//        for (unsigned long j = 0; j < already_chosen.size(); ++j)
//        {
//            if (waypoint_index == already_chosen[j]) // already chosen!
//            {
//                waypoint_index = engine.int_uniform(lw_index, up_index); // try with a new one
//                j = 0;
//                continue; // restart the loop
//            }
//        }
//        add_to_path(waypoint_index);
//        already_chosen.push_back(waypoint_index); // take track of it
//    }
//}
///////////////////////////////////////////////////////
/////   FIND PATHS FOR PEOPLE IN ORANGE CLUSTERS    ///
///////////////////////////////////////////////////////
//void Person::pathfinder_orange(Random& engine)
//{
//    const int n_waypoints = (int)(VISITING_PERCENTAGE_ORANGE * Simulation::Clusters[label].size());
//
//    // now picking n_waypoints from person home cluster
//    std::vector<int> already_chosen; // contains already chosen waypoints indeces
//
//    int waypoint_index = 0;
//    int lw_index = Simulation::Clusters[label].lower_index();
//    int up_index = Simulation::Clusters[label].upper_index();
//    for (int i = 0; i < n_waypoints; ++i)
//    {
//        waypoint_index = engine.int_uniform(lw_index, up_index); // uniformly extract an index
//        for (unsigned long j = 0; j < already_chosen.size(); ++j)
//        {
//            if (waypoint_index == already_chosen[j]) // already chosen!
//            {
//                waypoint_index = engine.int_uniform(lw_index, up_index); // try with a new one
//                j = 0;
//                continue; // restart the loop
//            }
//        }
//        add_to_path(waypoint_index);
//        already_chosen.push_back(waypoint_index); // take track of it
//    }
//}
///////////////////////////////////////////////////////
/////   FIND PATHS FOR PEOPLE IN RED CLUSTERS       ///
///////////////////////////////////////////////////////
//// fills like path_finder_white but n_waypoints is 15% the ones in white zone
//void Person::pathfinder_red(Random& engine)
//{
//    const int n_waypoints = (int)(VISITING_PERCENTAGE_RED * Simulation::Clusters[label].size());
//
//    // now picking n_waypoints from person home cluster
//    std::vector<int> already_chosen; // contains already chosen waypoints indeces
//
//    int waypoint_index = 0;
//    int lw_index = Simulation::Clusters[label].lower_index();
//    int up_index = Simulation::Clusters[label].upper_index();
//    for (int i = 0; i < n_waypoints; ++i)
//    {
//        waypoint_index = engine.int_uniform(lw_index, up_index); // uniformly extract an index
//        for (unsigned long j = 0; j < already_chosen.size(); ++j)
//        {
//            if (waypoint_index == already_chosen[j]) // already chosen!
//            {
//                waypoint_index = engine.int_uniform(lw_index, up_index); // try with a new one
//                j = 0;
//                continue; // restart the loop
//            }
//        }
//        add_to_path(waypoint_index);
//        already_chosen.push_back(waypoint_index); // take track of it
//    }
//}
///////////////////////////////////////////////////////
/////   WEIGHTS COMPUTATION FOR PATHFINDER_WHITE    ///
///////////////////////////////////////////////////////
//// calculate weights for each cluster so to respect the condition of OTHER_CLUSTERS_PROBABILITY
//void weights_fill(Person const& person, std::vector<int>& white_labels, std::vector<double>& weights)
//{
//    assert(weights.empty());                  // gotta make sure weights vector is empty since it'll be filled
//    weights.reserve(white_labels.size() + 1); // allocate the needed space
//
//    double person_cluster_weight = 0.0;
//
//    // fill the first white_clust.size() elements w/ other white clusters weights
//    for (int& index : white_labels)
//    {
//        double current_weight = Simulation::Clusters[index].weight();
//        weights.push_back(current_weight);
//        person_cluster_weight += current_weight; // add upp the the weights
//    }
//
//    // this way this cluster's waypoints will account for the right percentage in the extraction
//    person_cluster_weight *= (1 - OTHER_CLUSTERS_PROBABILITY) / OTHER_CLUSTERS_PROBABILITY;
//
//    // now adding this person's cluster weight
//    white_labels.push_back(person.home_cluster());
//    weights.push_back(person_cluster_weight); // last weight corresponds to the person's cluster's one
//}
/////////////////////////////////////////////////////
/// REMOVE WPTS FROM PATHNOT IN WHITE ZONE ANYMORE///
/////////////////////////////////////////////////////
void Person::clean_path()
{
    for (int j = 0; j < Paths_i.size() ;++j)
    {
        int wpt_i = Paths_i[j];
        //zone where the i-th wpt in person.Paths_i is located
        Zone const& wpt_zone = Simulation::Clusters[Simulation::Waypoints[wpt_i].get_label()].zone_type();
        if (wpt_zone != Zone::White)
        {
            remove_by_ref<int>(Paths_i,wpt_i); //remove from path
            --j;
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
        stay_counter = engine.rand_stay();
    }
    if (Paths_i.empty()) // if Path vector empty select home
    {
        going_home = true;
    }
    else if (Paths_i.size() == 1) // if Path has only one element select that element
    {
        target_i = Paths_i[0];
        Paths_i.clear();
        stay_counter = engine.rand_stay();
    }
    else // if Path vector has more than one element ran the LATP Algorithm to select next Location
    {
        //TODO reserve space for vectors
        double LATP_alpha = Simulation::Clusters[label].LATP_parameter();
        std::vector<double> inverse_distances;  // vector where we store the inverse_distance elevated to alpha of the
        // pointed location with the same index in Path
        std::vector<double> probabilities; // vector where we store the probabilities of the same index in Path
        inverse_distances.reserve(Paths_i.size());
        probabilities.reserve(Paths_i.size());

        for (auto& wpt_i : Paths_i) // fill the inverse_distances vector
        {
            Position const& curr_position = Simulation::Waypoints[wpt_i].get_position();
            double dist = 1 / pow(curr_position.distance_to(position), LATP_alpha);
            inverse_distances.push_back(dist);
        }
        // calculated with LATP algorithm
        // calculate the denominator for the probabilities formula of the LATP algorithm
        double denom = std::accumulate(std::begin(inverse_distances), std::end(inverse_distances),0.0);
        for (auto &a : inverse_distances)
        {
            probabilities.push_back(a / denom); // fill w/ i-th probability
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
//////////////////////// POSSIBLE NEW PATHFINDER ////////////
//call in move function
//std::vector<double> weights(world.Clusters().size(),0); //weights for every inth cluster
//dichiara weights in move
void fill_with_weights(std::vector<double>& weights)
{
    weights.reserve(CLUSTERS_SIZE);
    for (auto& cl : Simulation::Clusters)
    { // fill the weight fot the various cluster, except for the current
        if (cl.zone_type() == Zone::White)
        {
            weights.push_back(cl.weight());
        }
    }
}

//world.generate_path(rng.rounded_gauss(4,1)+1,weights,a.path(),rng);
void Person::pathfinder_white(int to_visit,Random& engine) //the vector weight has to be created in Simulation::move() for every cluster so that the weight of the current cluster is equal the sum of the other weights
{
    std::vector<double> weights;
    fill_with_weights(weights); //filled w/ weights
    Paths_i.reserve(to_visit);

    std::vector<int> choose(CLUSTERS_SIZE,0); // number of locations to visit from the corresponding cluster(index)
    for(int i = 0; i < to_visit; ++i)
    {
        ++choose[engine.discrete(weights)];  //one loc from this cluster will be chosen
    }

    int to_set = 0;
    int waypoint_index = 0;
    std::vector<int> already_chosen;
    for (int i = 0; i < CLUSTERS_SIZE; ++i)
    {
        to_set = choose[i]; //loc to set from this cluster
        Cluster& curr_cl = Simulation::Clusters[i];
        while(to_set > 0)
        {
            waypoint_index = engine.int_uniform(curr_cl.lower_index(), curr_cl.upper_index()); // uniformly extract an index
            for (unsigned long i = 0; i < already_chosen.size(); ++i)
            {
                if (waypoint_index == already_chosen[i]) // already chosen!
                {
                    waypoint_index = engine.int_uniform(curr_cl.lower_index(), curr_cl.upper_index()); // try with a new one
                    i = 0;
                    continue; // restart the inner loop
                }
            }
            add_to_path(waypoint_index);
            already_chosen.push_back(waypoint_index); // take track of it
            --to_set;
        }
    }
}
void Person::pathfinder_non_white(int to_visit,Random& engine) //the vector weight has to be created in Simulation::move() for every cluster so that the weight of the current cluster is equal the sum of the other weights
{
    Paths_i.reserve(to_visit);
    int to_set = to_visit;
    int waypoint_index = 0;
    std::vector<int> already_chosen;
    Cluster& person_cl = Simulation::Clusters[label];

    for (int i = 0; i < CLUSTERS_SIZE; ++i)
    {
        while(to_set > 0)
        {
            waypoint_index = engine.int_uniform(person_cl.lower_index(), person_cl.upper_index()); // uniformly extract an index
            for (unsigned long i = 0; i < already_chosen.size(); ++i)
            {
                if (waypoint_index == already_chosen[i]) // already chosen!
                {
                    waypoint_index = engine.int_uniform(person_cl.lower_index(), person_cl.upper_index()); // try with a new one
                    i = 0;
                    continue; // restart the inner loop
                }
            }
            add_to_path(waypoint_index);
            already_chosen.push_back(waypoint_index); // take track of it
            --to_set;
        }
    }
}














} // namespace smooth_simulation














