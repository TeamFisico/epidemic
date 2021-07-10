#ifndef MOBILITY_MODEL_HPP
#define MOBILITY_MODEL_HPP
#include "location.hpp"
#include "person.hpp"
#include "random.hpp"
#include <vector>
namespace smooth_sim
{
class Mobility_model
{
  private:
    Person pers;                 // Person subject to the mobility model
    std::vector<Location*> Path; // Vector with pointer to the location the person need to visit, in no particular order
    Location* target_location;   // Current target_location
    int stay;                    // How much time people stay at a location
    double home_probability;     // Probability for a person to stay home
    bool at_home;                // Boolean keeping track whether the person subject to the mob model is home or not
    bool going_home;             // Boolean keeping track whether the person has target == home
  public:
    // Constructor
    Mobility_model(Person person, int stay, double home_probability, bool at_home);

    // Prevent the compiler from generating the default constructor since no use of it should be made
    Mobility_model() = delete;

    // Let Person class access Mobility_model members
    friend class Person;

    // Returns the person subject to the model
    Person get_person() const;

    // Returns the target location
    Location* get_target_location();

    // Returns true if the person is currently inside its current target location(in location radius), false otherwise
    bool at_target_location() const;

    // Returns the number of steps the person still has to wait at the current location
    int stay_time() const;

    // Returns true if the person is currently at home
    bool is_at_home() const;

    // Returns the probability that a person won't move from his home location
    double home_prob() const;

    // Returns the index of the cluster in which the pointed person reside
    int get_label() const;

    // Set the person status as at_home/not at_home
    void set_is_at_home(bool is_at_home);

    // Decreases the stay counter, since the person will stay at it's current location during the current step
    void decrease_stay();

    // Returns a reference to the person subject to the model
    Person& person();

    // Reference to path of the person(containing location targets)
    std::vector<Location*>& path();

    // Randomly select the new location to visit; or home if you do
    // not have anymore location to visit
    void next_location(double cluster_LATP_parameter, Random& engine);

    // Move a person toward his/her target(by moving its position toward target)
    void move(double speed, Random& engine);

    // Set the person target to be home
    void recall_home();
};

} // namespace smooth_sim

#endif
