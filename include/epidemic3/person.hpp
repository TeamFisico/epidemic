#ifndef PERSON_HPP
#define PERSON_HPP

#include "location.hpp"
#include <vector>

namespace SMOOTH
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
    double velocity[2]; // [0] == x_speed , [1] == y_speed

  public:
    std::vector<int> Paths; // indeces to waypoints to visit in Waypoints array

    friend class Simulation;
    Person(Status status, int cluster_label, Location home, Location current_location, Location target_location,
           bool at_place, double x_speed, double y_speed);
    Person(); // default constructor

    int home_cluster() const { return label; }
    double speed_x() const { return velocity[0]; }
    double speed_y() const { return velocity[1]; }
    double speed() const;
    double direction() const { return atan(velocity[1] / velocity[0]); }
    Location get_location() const { return location; };
    Location get_home() const { return home; };
    bool at_home() const; // is the person staying home
    bool empty_path() const { return Paths.size() == 0; }

    void set_speed_x(double x_speed) { velocity[0] = x_speed; }
    void set_speed_y(double y_speed) { velocity[1] = y_speed; }
    void set_cluster(int n) { label = n; }
    void set_home(Location loc) { home = loc; }
    void set_target(Location loc) { target = loc; }
    void set_location(Location loc) { location = loc; }

  private:
    void move_toward();
    void move_home();
    void update_speed();

  public:
    void upgrade_status();
    void update_target(double LATP_parameter);
    void move_person();
};
int determine_fill_size(const Person& person); // determine how many indeces Paths must be filled with
void fill_path_home(Person& person);           // select next visiting locations from the person cluster
void fill_path_white(Person& person); // TODO define               // select next visiting locations from green clusters
int determine_pause_time();
// bool try_infect(); // TODO Define

} // namespace SMOOTH

#endif // PERSON_HPP
