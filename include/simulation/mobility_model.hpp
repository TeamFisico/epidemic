#ifndef MOBILITY_MODEL_HPP
#define MOBILITY_MODEL_HPP
#include "location.hpp"
#include "person.hpp"
#include "random.hpp"
#include <vector>
namespace smooth_sim
{
class mobility_model
{
  private:
    Person person;
    std::vector<Location*> Path; // Vector with pointer to the location the person need to visit, in no particular order
    Location* target_location;   // current target_location
    int stay;                    // How much time people stay at a location //TODO make some of these variables as macro
    double home_probability;
    bool at_home;    // true if the pointed person is at home, false otherwise
    bool going_home; // true if it is moving home, false otherwise
    double alpha;    // exponent of the LATP algorithm, used to calculate the next location
  public:
    friend class Person;
    mobility_model(Person person, int stay, double home_probability, bool at_home); // Constructor
    mobility_model() = delete;                                                      // Default constructor
    void next_location(Random& engine); // Randomly select the new location to visit; or home if you do not have anymore
                                        // location to visit
    bool at_target_location(); // return true if the pointed person is inside pointed location Radius, return false
    void move(double speed, Random& engine); // move the pointed Person to the pointed target_location
    void recall_home(); // set the target_location to home. to be called when curfew start or at a cluster color change
                        //    void change_home_prob(double prob);     // change home_probability
    void not_at_home() { at_home = false; } // set at_home to false
    bool is_at_home() const { return at_home; }
    double home_prob() const { return home_probability; }
    void decrease_stay() { --stay; }
    int stay_time() const { return stay; }
    int cluster_index(); // return the index of the cluster in which the pointed person reside
    std::vector<Location*>& path() { return Path; }
    Person& person_ref() { return person; }
};

double rand_speed(double min, double max);
} // namespace smooth_sim

#endif