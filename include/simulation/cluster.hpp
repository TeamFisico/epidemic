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
    // Constructor
    Cluster(int S, int E, int I, int R, int cluster_locations_num, Rectangle Area, Zone cluster_zone, int cluster_label,
            double cluster_LATP_parameter);

    // Prevent the compiler from generating the default constructor since no use of it should be made
    Cluster() = delete;

  private:
    // Construct people(Mobility_model) in this cluster: assign them home by families and set them home;set also their
    // initial status as Status::Susceptible(some will have it changed in set_E_I_R_individuals()
    void generate_people(int people_num, int cluster_label);

    // Generate the various groups, distributing the locations
    void generate_groups(int locations_num);

    // Generate a valid center location used by group
    Position gen_group_center(int num_of_loc);

    // Set Status::Exposed,Status::Infected,Status::Recovered in such a way to have the right initial E,I,R individuals
    void set_E_I_R_individuals(int E, int I, int R);

    // Return a pointer to a group location by index(used in path generation)
    Location* select_location(int n);

  public:
    // Returns people vector in this cluster
    std::vector<Mobility_model> get_people() const;

    // Number of people in this cluster
    int people_num() const;

    // Returns the number of groups in this cluster
    unsigned size() const;

    // Number of locations in this cluster
    int locations_num() const;

    // Returns cluster current zone
    Zone get_zone() const;

    // Returns the index of it into clusters vector in world
    int get_label() const;

    // Returns LATP parameter used by people in this cluster when calling next_location()
    double get_LATP() const;

    // Returns a reference to people vector in this cluster
    std::vector<Mobility_model>& people();

    // Returns reference to groups vector of this cluster
    std::vector<Group>& groups();

    // Returns a reference to the cluster rectangular area(used when graphically representing cluster)
    Rectangle& area();

    // Sets cluster zone
    void set_zone(Zone cluster_zone);

    // Sets LATP parameter used by people in this cluster when calling next_location()
    void set_LATP(double new_LATP_parameter);

    // Fills path vector with pointers to location to visit
    void generate_cluster_path(int to_visit, std::vector<Location*>& path);
};

} // namespace smooth_sim

#endif