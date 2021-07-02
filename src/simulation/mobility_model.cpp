#include "mobility_model.hpp"
#include "parameters.hpp"
#include "random.hpp"
#include <cassert>
#include <iostream>
#include <numeric>

namespace smooth_sim
{
// Constructor
mobility_model::mobility_model(Person person, int stay, double home_probability, bool at_home)
    : person{person}, target_location{nullptr}, stay{stay}, home_probability{home_probability}, at_home{at_home},
      going_home{false}
{
}

// Default Constructor WIP
/*mobility_model::mobility_model()
{
}*/

void mobility_model::next_location(Random& rng)
{
    if (going_home)
    { // called when person is at home with target location home
        at_home = true;
        going_home = false;
        stay = rng.rand_stay();
    }
    if (Path.empty())
    { // if Path vector empty select home
        target_location = person.get_home();
        going_home = true;
    }
    else if (Path.size() == 1)
    { // if Path ha only one element select that element
        target_location = Path.operator[](0);
        Path.clear();
        stay = rng.rand_stay();
    }
    else // if Path vector has more than one element ran the LATP Algorithm to select next Location
    {
        std::vector<double> inverse_distances; // vector where we store the inverse_distance elevated to alpha of the
                                               // pointed location with the same index in Path
        inverse_distances.clear();
        for (auto& a : Path)
        { // fill the inverse_distances vector
            double dist = 1 / pow(a->get_pos().distance_to(person.get_pos()), alpha);
            inverse_distances.push_back(dist);
        }
        std::vector<double> probabilities; // vector where we store the probabilities of the same index in Path
                                           // calculated with LATP algorithm
        probabilities.clear();
        double denom =
            std::accumulate(inverse_distances.begin(), inverse_distances.end(),
                            0); // calculate the denominator for the probabilities formula of the LATP algorithm
        for (auto& a : inverse_distances)
        { // fill the probabilities vector
            probabilities.push_back(a / denom);
        }
        // select next_location to visit based on the probabilities vector
        int index_result = rng.discrete(probabilities);
        target_location =
            Path.operator[](index_result); // set the target Location to the Location found using LATP algorithm
        // remove the selected location pointer from the Path vector
        auto it = Path.begin(); // generate an iterator to the star of the Path vector
        it = it + index_result; // make sure the iterator point to the selected location
        Path.erase(it);         // erase the selected Location from the Path vector
        stay = rng.rand_stay();
    }
}

bool mobility_model::at_target_location()
{
    return person.at_location(target_location);
}

void mobility_model::move(double speed, Random& rng)
{
    person.get_pos().move_toward(target_location->get_pos(), speed, rng);
}

void mobility_model::recall_home()
{
    Path.clear();
    stay = 0;
    target_location = person.get_home();
}

void mobility_model::change_home_prob(double prob)
{
    home_probability = prob;
}

int mobility_model::cluster_index()
{
    return person.get_cluster_index();
}

double rand_speed(double min, double max)
{
    Random rng;
    return rng.uniform(min, max);
}
} //namespace smooth_sim