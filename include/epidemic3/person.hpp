#ifndef PERSON_HPP
#define PERSON_HPP

#include "area.hpp"
#include "location.hpp"
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
    int label;                  // label of the cluster
    Location home;              //home address
    Location location;          //current location
    Location* target_location;  // ptr to next target location
    double velocity[2];         // [0] == x_speed , [1] == y_speed
  public:
    std::vector<int> Paths; // vector with indeces to waypoints in Waypoints array

    Person();
    int cluster_label() const { return label; }
    double speed_x() const { return velocity[0]; }
    double speed_y() const { return velocity[1]; }
    double speed() const;
    double direction() const { return atan(velocity[1] / velocity[0]); }
    Location get_location() const { return location; };
    Location get_home() const { return home; };
    bool at_home();

    void set_speed_x(double x_speed) { velocity[0] = x_speed; }
    void set_speed_y(double y_speed) { velocity[1] = y_speed; }
    void set_cluster(int n) { label = n; }
    void set_home(Location loc) { home = loc; }
    void set_target(Location loc) { target_location = &loc; }
    void set_location(Location loc) { location = loc; }
};

  void move_toward(Person* person, Location target);
  void update_speed(Person* person);
  void upgrade_condition(Person*);   //TODO Define
  bool try_infect();                 //TODO Define

} // namespace SMOOTH

#endif // PERSON_HPP
