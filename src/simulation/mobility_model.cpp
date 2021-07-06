#include "mobility_model.hpp"
#include "random.hpp"
#include <cassert>
#include <numeric>

namespace smooth_sim
{

////////////////////////////////////////////////////////
/////         MOBILITY MODEL CONSTRUCTOR          //////
////////////////////////////////////////////////////////
Mobility_model::Mobility_model(Person person, int stay, double home_probability, bool at_home)
    : person{person},
      target_location{nullptr},
      stay{stay},
      home_probability{home_probability},
      at_home{at_home},
      going_home{false}
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////           PRIVATE METHODS           /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////           PUBLIC METHODS            /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mobility_model::next_location(Random& engine, double cluster_LATP_parameter)
{
    if (going_home)
    { // called when person is at home with target location home
        at_home = true;
        going_home = false;
        stay = engine.rand_stay();
    }
    if (Path.empty())
    { // if Path vector empty select home
        target_location = person.get_home_pointer();
        going_home = true;
    }
    else if (Path.size() == 1)
    { // if Path ha only one element select that element
        target_location = Path.operator[](0);
        Path.clear();
        stay = engine.rand_stay();
    }
    else // if Path vector has more than one element ran the LATP Algorithm to select next Location
    {
        std::vector<double> inverse_distances; // vector where we store the inverse_distance elevated to alpha of the
                                               // pointed location with the same index in Path
        inverse_distances.clear();
        for (auto& a : Path)
        { // fill the inverse_distances vector
            double dist = 1 / pow(a->get_pos().distance_to(person.pos()), cluster_LATP_parameter);
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
        int index_result = engine.discrete(probabilities);
        target_location =
            Path.operator[](index_result); // set the target Location to the Location found using LATP algorithm
        // remove the selected location pointer from the Path vector
        auto it = Path.begin(); // generate an iterator to the star of the Path vector
        it = it + index_result; // make sure the iterator point to the selected location
        Path.erase(it);         // erase the selected Location from the Path vector
        stay = engine.rand_stay();
    }
}

bool Mobility_model::at_target_location()
{
    return person.at_location(target_location);
}

void Mobility_model::move(double speed, Random& engine)
{
    person.pos().move_toward(target_location->get_pos(), speed, engine);
}

void Mobility_model::recall_home()
{
    Path.clear();
    stay = 0;
    target_location = person.get_home_pointer();
}

// void Mobility_model::change_home_prob(double prob)
//{
//    home_probability = prob;
//}

int Mobility_model::cluster_index()
{
    return person.get_label();
}

double rand_speed(double min, double max, Random& engine)
{
    return engine.uniform(min, max);
}
} // namespace smooth_sim
