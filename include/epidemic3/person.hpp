#ifndef PERSON_HPP
#define PERSON_HPP

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
    bool at_home;     // is the person staying at a place
    bool going_home;
    bool changed_status;
    int stay_counter;         // time a person will spend to a place
    std::vector<int> Paths_i; // indeces to waypoints to visit in Waypoints array
  public:
    friend class Simulation;
    Person(Status status, int cluster_label, Location home, Position current_position, int target_index,
           bool is_at_home, bool going_home, bool changed_status,int stay_time);
    Person(); // default constructor
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Status current_status() const { return status[0]; }
    Status new_status() const { return status[1]; }
    int home_cluster() const { return label; }
    Position get_position() const { return position; };
    Location get_home() const { return home; };
    bool is_at_home() const { return at_home; }; // is the person staying home
    bool empty_path() const { return Paths_i.size() == 0; }
    int path_size() const { return Paths_i.size(); }
    int get_target_i(){ return target_i; }
    int get_stay() const { return stay_counter; }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void set_current_status(Status st) { status[0] = st; }
    void set_new_status(Status st) { status[1] = st; }
    void set_cluster_label(int n) { label = n; }
    void set_home(Location loc) { home = loc; }
    void set_is_at_home(bool is_at_home){ at_home = is_at_home; }
    void set_target_i(int target_index) { target_i = target_index; }
    void set_position(Position pos) { position = pos; }
    void set_changed_status(bool has_changed) { changed_status = has_changed; }
    void set_at_home();
    void add_to_path(int const& wpt_index) { Paths_i.push_back(wpt_index); }
    bool is_staying();    // determine if person can move
    bool is_going_home()const { return going_home; }
    void update_status(); // pass to new condition
    bool at_target_location();
    void move_toward(Random& engine);
    void move_home(Random& engine);
    void decrease_stay_counter(){ --stay_counter; }
  private:

    void update_speed(Random& engine);
    void update_target(Random& engine);
    void remove_target_i(int target_i);

  public:
    void move(Random& engine);
    void next_location(Random& engine);
    void pathfinder_white(Random& engine);
    void pathfinder_orange(Random& engine);
    void pathfinder_yellow(Random& engine);
    void pathfinder_red(Random& engine);
    void clean_path();
};

bool are_white_available(const Person& person);

void weights_fill(Person const& person, std::vector<int>& white_labels, std::vector<double>& weights);
std::vector<int> get_close_people_in_cluster(Person const& person);
std::vector<int> get_close_people(Person const& person);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> void remove_by_ref(std::vector<T>& vec, T& to_remove); // remove an integer(index from a vector<int>
template<class T> void remove_by_value(std::vector<T>& vec, T to_remove); // remove an integer(index from a vector<int>

} // namespace smooth_simulation

#endif // PERSON_HPP
