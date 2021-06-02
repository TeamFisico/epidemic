#include "mobility_model.hpp"
#include <cassert>
#include <random>
#include <cmath>

namespace SMOOTH
{

MobilityModel::MobilityModel(Person *person, Location *Waypoints_to_visit, int still_to_visit,
                             Location *target_location, double speed, double stay)
{
}
/////////////////////////////////////////////////////
////////            PATH ASSIGNMENT           ///////
/////////////////////////////////////////////////////
// select y% waypoints to put into Path among the available from the belonging cluster
void MobilityModel::select_waypoints()
{

    int belonging_cluster_size = Simulation::world.Clusters[person->cluster_label()].size();

    std::random_device rd;
    std::mt19937 gen(rd());

    int selected_size = static_cast<int>(belonging_cluster_size * Simulation::y);
    // now determining the this cluster's indeces range inside Waypoints array

    int starting_index = 0;
    int ending_index = 0;

    if (person->cluster_label() > 0) // this is not the first clust
    {
        for (int i = 0; i < person->cluster_label(); ++i) // check the previous clusters
        {
            starting_index += Simulation::world.Clusters[i].size();
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

    assert(person->Paths.size() == 0);
    person->Paths.reserve(selected_size);
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
        person->Paths.push_back(random_index);
    }
}
/////////////////////////////////////////////////////
////////       TARGET ASSIGNMENT : LATP       ///////
/////////////////////////////////////////////////////
void MobilityModel::choose_target()
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
    for (int &index : person->Paths)
    {
        Location waypoint = Simulation::world.Waypoints[index];
        current_weight = compute_weight(person_location.get_distance(waypoint));
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

    person->set_target(Simulation::world.Waypoints[rand(gen)]);
    //TODO TESTING
}

double MobilityModel::compute_weight(double distance)
{
    return 1 / std::pow(distance, alpha);
}

void MobilityModel::update_speed()
{
    const double max_std_dev = 1;//TODO Determine this constant
    //velocity is measured in units/s
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> std_dev(0,max_std_dev);   //set the std dev extraction

    std::normal_distribution<double> x_sp(person->speed_x(),std_dev(gen));
    std::normal_distribution<double> y_sp(person->speed_y(),std_dev(gen));

    person->set_speed_x(x_sp(gen));
    person->set_speed_y(y_sp(gen));

}
/////////////////////////////////////////////////////
////////  MOVE A PERSON TOWARD A LOCATION     ///////
/////////////////////////////////////////////////////
void MobilityModel::move_toward(Location target)
//move a person by an amount determined by the current speed
{
    //determine the next displacement based on person's current velocity
    double x_displacement = person->speed_x() * TIME_STEP;  // x = v_x * ∆t
    double y_displacement = person->speed_y() * TIME_STEP;  // y = v_y * ∆t
    double displacement = sqrt(x_displacement*x_displacement + y_displacement*y_displacement);

    if (person->get_location().get_distance(target) < displacement)
    {
        person->set_location(target);
        person->at_place = true;
        update_speed();
        return;
    }

    double delta_x = std::abs(person->get_location().X() - target.X());
    double delta_y = std::abs(person->get_location().Y() - target.Y());
    double theta = atan(delta_y / delta_x);      //angle connecting the target through a straight line

    std::random_device rd;
    std::mt19937 gen(rd());
    //generate an angle between velocity vector direction and the target's one so that will not point precisely to the target
    std::uniform_real_distribution<double> angle(person->direction(),theta);

    double final_angle = angle(gen);
    //calculate new displacement
    //replace the precvious velocity vector with a new one with the same magnitude but different direction
    //so recalculating the new v_x and v_y to determine x_displacement and y_displacement
     x_displacement = person->speed() * cos(final_angle) * TIME_STEP;
     y_displacement = person->speed() * sin(final_angle) * TIME_STEP;

     Location new_location {person->get_location().X() + x_displacement,person->get_location().Y() + y_displacement};

     person->set_location(new_location);

     update_speed(); //set new person speed
}

} // namespace SMOOTH





