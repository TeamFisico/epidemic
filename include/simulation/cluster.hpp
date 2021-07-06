#ifndef CLUSTER_HPP
#define CLUSTER_HPP
#include "group.hpp"
#include "mobility_model.hpp"
#include "person.hpp"
#include "random.hpp"
#include "rectangle.hpp"

namespace smooth_sim
{

enum class Zone
{
    Green,
    Yellow,
    Red
};

class Cluster
{
  private:
    std::vector<Mobility_model> People; // Mobility models in this cluster
    std::vector<Group> Groups;          // Groups in this Cluster
    Rectangle Area;                     // Area occupied by this cluster on the simulation Area
    Zone zone;                          // Current cluster Zone(White,Yellow,Red)
    int index;                          // Index of this cluster corresponding to index in clusters vector(in World)
    double LATP_alpha;                  // LATP algorithm parameter(depends on the zone)
    Random cl_engine;

  public:
    Cluster(int S, int E, int I, int R, int cluster_locations_num, Rectangle Area, Zone cluster_zone, int cluster_label,
            double cluster_LATP_parameter);
    Cluster() = delete;

  private:
    // construct people(Mobility_model) in this cluster: assign them home by families and set them home;set also their
    // initial status as Status::Susceptible(some will have it changed in set_E_I_R_individuals()
    void generate_people(int people_num, int cluster_label);

    void generate_groups(int locations_num);

    // generate a valid center location used by group
    Position gen_group_center(int num_of_loc);

    // set Status::Exposed,Status::Infected,Status::Recovered in such a way to have the right initial E,I,R individuals
    void set_E_I_R_individuals(int E, int I, int R);

    // return a pointer to a group location by index(used in path generation)
    Location* select_location(int n);

  public:
    // returns people vector in this cluster
    std::vector<Mobility_model> get_people() const;

    // number of people in this cluster
    int people_num() const;

    // returns the number of groups in this cluster
    unsigned size() const;

    // number of locations in this cluster
    int locations_num() const;

    // returns cluster current zone
    Zone get_zone() const;

    // returns label of this cluster(index of it into clusters vector(World class)
    int get_label() const;

    // returns LATP parameter used by people in this cluster when calling next_location()
    double get_LATP() const;

    // returns a reference to people vector in this cluster
    std::vector<Mobility_model>& people();

    // returns reference to groups vector of this cluster
    std::vector<Group>& groups();

    // returns a reference to the cluster rectangular area(used when graphically representing cluster)
    Rectangle& area(); // TODO condider making a getter of area rather than a reference

    // sets cluster zone
    void set_zone(Zone cluster_zone);

    // sets LATP parameter used by people in this cluster when calling next_location()
    void set_LATP(double new_LATP_parameter);

    // fills path vector with pointers to location to visit
    void generate_path(int to_visit, std::vector<Location*>& path);
};

} // namespace smooth_sim

#endif

// TODO Unused

//    returns base of the cluster rectangular Area
//    double base();
//
//    // returns height of the cluster rectangular Area
//    double height();

// select the nth location and return his pointer, used for generate_path
//    std::vector<Location*> Location_list(); // get a vector of pointer to all locations in the cluster
//    std::vector<Person*> Person_list();     // get a vector of pointer to all People in the cluster