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
    Latent,
    Infected,
    Recovered,
    Dead
};

class Person
{
  private:
    Status status;
    int label;          // label of the belongin cluster(where house is located
    Location home;      // home address
    Location location;  // current location
    Location target;    // next target location
    bool at_place;      // is the person staying at a place
    bool going_home;
    double velocity[2]; // [0] == x_speed , [1] == y_speed
    int stay_counter;   // time a person will spend to a place

  public:
    std::vector<int> Paths_i; // indeces to waypoints to visit in Waypoints array

    friend class Simulation;
    Person(Status status, int cluster_label, Location home, Location current_location, Location target_location,
           bool at_place, bool going_home,double x_speed, double y_speed, int stay_time);
    Person(); // default constructor

    int home_cluster() const { return label; }
    double speed_x() const { return velocity[0]; }
    double speed_y() const { return velocity[1]; }
    double speed() const;
    double direction() const { return atan(velocity[1] / velocity[0]); }
    Location get_location() const { return location; };
    Location get_home() const { return home; };
    bool at_home() const; // is the person staying home
    bool empty_path() const { return Paths_i.size() == 0; }

    void set_speed_x(double x_speed) { velocity[0] = x_speed; }
    void set_speed_y(double y_speed) { velocity[1] = y_speed; }
    void set_cluster(int n) { label = n; }
    void set_home(Location loc) { home = loc; }
    void set_target(Location loc) { target = loc; }
    void set_location(Location loc) { location = loc; }
    bool is_staying();               //determine if person can move
  private:
    void move_toward();
    void move_home();
    void update_velocity();
    void update_target();
  public:
    void upgrade_status();
    void move();    //move a person if it should
    void refill();
};
// helper functions
int determine_fill_size(const Person& person);   // determine how many indeces Paths must be filled with
bool are_white_available(const Person& person);  // return true if there are any white clusters available
void pathfinder(Person& person);  //find paths
void pathfinder_white(Person& person);       //find paths for a person in white Zone
void pathfinder_orange(Person& person);      //find paths for a person in yellow Zone
void pathfinder_yellow(Person& person);      //find paths for a person in orange Zone
void pathfinder_red(Person& person);        //find paths for a person in red Zone
void fill_path_home(Person& person);
void fill_path_white(Person& person);          // TODO define , select next visiting locations from green clusters
int determine_pause_time();
void remove_target_index(Person& person,int index_to_remove);
void remove_target(Person& person,Location to_remove);   //remove a visited target from person.Path
std::vector<int> get_close_people_in_cluster(Person const& person);   //returns indeces of in-spread-radius people inside person's cluster
std::vector<int> get_close_people(Person const& person);   //returns indeces of in-spread-radius people

} // namespace smooth_simulation

#endif // PERSON_HPP
