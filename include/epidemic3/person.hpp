#ifndef PERSON_HPP
#define PERSON_HPP

#include "../random.hpp"
#include "location.hpp"
#include <vector>

namespace smooth_simulation
{

enum class Status
{
    Susceptible = 0,
    Exposed,
    Infected,
    Recovered,
    Dead
};

class Person
{
  private:
    Status status[2];  // status[0] == current_status, status[2] == new_status
    int label;         // label of the belonging cluster(where house is located
    Location home;     // home address
    Position position; // current person position
    int target_i;      // next target location(index
    bool at_place;     // is the person staying at a place
    bool going_home;
    bool changed_status;
    double speed;       // [0] == x_speed , [1] == y_speed
    int stay_counter;         // time a person will spend to a place
    std::vector<int> Paths_i; // indeces to waypoints to visit in Waypoints array
  public:
    friend class Simulation;
    Person(Status status, int cluster_label, Location home, Position current_position, int target_index,
           bool is_at_place, bool going_home, bool changed_status, double x_speed, double y_speed, int stay_time);
    Person(); // default constructor
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Status current_status() const { return status[0]; }
    Status new_status() const { return status[1]; }
    int home_cluster() const { return label; }
    double direction() const { return atan(velocity[1] / velocity[0]); }
    Position get_position() const { return position; };
    Location get_home() const { return home; };
    bool at_home() const; // is the person staying home
    bool empty_path() const { return Paths_i.size() == 0; }
    int path_size() const { return Paths_i.size(); }
    int get_target_i(){ return target_i; }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void set_current_status(Status st) { status[0] = st; }
    void set_new_status(Status st) { status[1] = st; }
    void set_cluster_label(int n) { label = n; }
    void set_home(Location loc) { home = loc; }
    void set_target_i(int target_index) { target_i = target_index; }
    void set_position(Position const& pos) { position = pos; }
    void set_changed_status(bool has_changed) { changed_status = has_changed; }
    void set_at_home();
    void add_to_path(int const& wpt_index) { Paths_i.push_back(wpt_index); }
    bool is_staying();    // determine if person can move
    void update_status(); // pass to new condition
  private:
    void move_toward(Random& engine);
    void move_home(Random& engine);
    void update_speed(Random& engine);
    void update_target(Random& engine);
    void remove_target_i(int target_i);

  public:
    void pathfinder(Random& engine);
    void move(Random& engine); // move a person if it should
};

bool are_white_available(const Person& person);                 // return true if there are any white clusters available
void pathfinder_white(Person& person, Random& engine);          // find paths for a person in white Zone
void pathfinder_orange(Person& person, Random& engine);         // find paths for a person in yellow Zone
void pathfinder_yellow(Person& person, Random& engine);         // find paths for a person in orange Zone
void pathfinder_red(Person& person, Random& engine);            // find paths for a person in red Zone
void remove_index(std::vector<int>& indeces_v, int& to_remove); // remove an integer(index from a vector<int>
void weights_fill(Person const& person, std::vector<int>& white_labels, std::vector<double>& weights);
std::vector<int> get_close_people_in_cluster(
    Person const& person); // returns indeces of in-spread-radius people inside person's cluster
std::vector<int> get_close_people(Person const& person); // returns indeces of in-spread-radius people

} // namespace smooth_simulation

#endif // PERSON_HPP
