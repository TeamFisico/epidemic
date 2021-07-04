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
    Person person;
    std::vector<Location*> Path; // Vector with pointer to the location the person need to visit, in no particular order
    Location* target_location;   // current target_location
    int stay;                    // How much time people stay at a location
    double home_probability;     // probability for a person to stay home
    bool at_home;                // boolean keeping track whether the person subject to the mob model is home or not
    bool going_home;             // boolean keeping track whether the person has target == home
  public:
    friend class Person;
    Mobility_model(Person person, int stay, double home_probability, bool at_home); // Constructor
    Mobility_model() = delete;                                                      // Default constructor
    void next_location(Random& engine,
                       double cluster_LATP_parameter); // Randomly select the new location to visit; or home if you do
                                                       // not have anymore location to visit
    bool at_target_location(); // return true if the pointed person is inside pointed location Radius, return false
    void move(double speed, Random& engine); // move the pointed Person to the pointed target_location
    void recall_home(); // set the target_location to home. to be called when curfew start or at a cluster color change
                        //    void change_home_prob(double prob);     // change home_probability
    bool is_at_home() const { return at_home; }
    void set_is_at_home(bool is_at_home) { at_home = is_at_home; }
    double home_prob() const { return home_probability; }
    void decrease_stay() { --stay; }
    int stay_time() const { return stay; }
    int cluster_index(); // return the index of the cluster in which the pointed person reside
    std::vector<Location*>& path() { return Path; }
    Person& person_ref() { return person; }
    Person get_person() const { return person; }
};

} // namespace smooth_sim

#endif