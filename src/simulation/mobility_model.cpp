#include "mobility_model.hpp"
#include "random.hpp"
#include <numeric>

namespace smooth_sim
{

////////////////////////////////////////////////////////
/////         MOBILITY MODEL CONSTRUCTOR          //////
////////////////////////////////////////////////////////
Mobility_model::Mobility_model(Person person, int stay, double home_probability, bool at_home)
    : pers{person},
      target_location{nullptr},
      stay{stay},
      home_probability{home_probability},
      at_home{at_home},
      going_home{false}
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////           PUBLIC METHODS            /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////// PERSON OF THE MODEL /////////////////
Person Mobility_model::get_person() const
{
    return pers;
}
///////////////// TARGET LOCATION /////////////////
Location* Mobility_model::get_target_location()
{
    return target_location;
}
///////////////// IS THE PERSON AT HIS/HER TARGET LOCATION? /////////////////
bool Mobility_model::at_target_location() const
{
    return pers.at_location(target_location);
}
///////////////// STAY COUNTER /////////////////
int Mobility_model::stay_time() const
{
    return stay;
}
///////////////// IS THE PERSON AT HOME?  /////////////////
bool Mobility_model::is_at_home() const
{
    return at_home;
}
///////////////// PROBABILITY TO BE AT HOME /////////////////
double Mobility_model::home_prob() const
{
    return home_probability;
}
///////////////// LABEL OF THE CLUSTER THE PERSON BELONGS TO  /////////////////
int Mobility_model::get_label() const
{
    return pers.get_label();
}
///////////////// SET THE PERSON TO BE AT HOME/ NOT AT HOME /////////////////
void Mobility_model::set_is_at_home(bool is_at_home)
{
    at_home = is_at_home;
}
///////////////// DECREASE PERSON STAY AT A PLACE /////////////////
void Mobility_model::decrease_stay()
{
    --stay;
}
///////////////// REFERENCE TO PERSON OF THE MODEL /////////////////
Person& Mobility_model::person()
{
    return pers;
}
///////////////// REFERENCE TO PERSON PATH(POINTERS TO TARGET LOCATIONS) /////////////////
std::vector<Location*>& Mobility_model::path()
{
    return Path;
}
///////////////// DETERMINE NEXT LOCATION: LATP ALGORITHM /////////////////
void Mobility_model::next_location(double cluster_LATP_parameter, Random& engine)
{

    ///////// Case 1 : The person has arrived home /////////

    if (going_home)
    {
        at_home = true;
        going_home = false;
        stay = engine.rand_stay();
        return;
    }

    //////// Case 2 : The person has no more target locations available /////////

    if (Path.empty())
    {
        target_location = pers.get_home();
        going_home = true;
        return;
    }

    ///////// Case 3 : The person has just 1 waypoint left to visit /////////

    if (Path.size() == 1)
    {
        target_location = Path[0];
        Path.clear();
        stay = engine.rand_stay();
        return;
    }

    ///////// Case 4 : The person is ready to move and has more than 1 target available: apply LATP algorithm /////////

    if (Path.size() > 1)
    {
        // Vector containing weight function values with alpha == cluster_LATP_parameter; the correpondence between
        // weight functions and location is the following: weight_functions[index] <--> Path[index]
        std::vector<double> weight_functions{};
        weight_functions.reserve(Path.size());
        // Filling with LATP weights
        for (auto& a : Path)
        {
            double w_i = 1 / pow(a->get_position().distance_to(pers.get_position()), cluster_LATP_parameter);
            weight_functions.push_back(w_i);
        }

        // Vector containing probability values referring to locations(previous correspondence holds)
        std::vector<double> probabilities{};
        probabilities.reserve(Path.size());

        // Calculate denominator (sum of all weights)
        double weights_tot = std::accumulate(std::begin(weight_functions), std::end(weight_functions), 0.);

        // Now fill it with the correspondent probability
        for (auto& a : weight_functions)
        {
            probabilities.push_back(a / weights_tot);
        }
        // Select the next_location to visit based on the probabilities vector and set it as the new target
        int chosen_index = engine.discrete(probabilities);
        target_location = Path[chosen_index];

        // Eventually erase the selected location(pointer) from Path vector
        auto it = Path.begin();
        it = it + chosen_index;
        Path.erase(it);

        // Generate the stay time for the person
        stay = engine.rand_stay();
        return;
    }
}
///////////////// MOVE THE PERSON TOWARD HIS/HER TARGET /////////////////
void Mobility_model::move(double speed, Random& engine)
{
    pers.position().move_toward(target_location->get_position(), speed, engine);
}

///////////////// HAVE THE PERSON GOING HOME /////////////////
void Mobility_model::recall_home()
{
    Path.clear();
    stay = 0;
    target_location = pers.get_home();
}

} // namespace smooth_sim
