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
    std::vector<mobility_model> people; // Mobility models in this cluster
    std::vector<Group> groups;          // Groups in this Cluster
    Rectangle Area;                     // Area occupied by this cluster on the simulation Area
    Zone zone;                          // Current cluster Zone(White,Yellow,Red)
    int label;                          // Index of this cluster corresponding to index in clusters vector(in World)
    double LATP_alpha;                  // LATP algorithm parameter(depends on the zone)
    Random cl_engine;

  public:
    Cluster(unsigned S, unsigned E, unsigned I, unsigned R, int number_of_location, Rectangle Area, Zone cluster_zone,
            int cluster_label, double cluster_LATP_parameter);
    Cluster() = delete;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    Rectangle& cluster_area() { return Area; }
    Zone get_zone() const { return zone; }
    void set_zone(Zone cluster_zone) { zone = cluster_zone; }
    void set_LATP(double new_LATP_parameter) { LATP_alpha = new_LATP_parameter; }
    std::vector<mobility_model>& people_ref() { return people; }
    std::vector<mobility_model> get_people() const { return people; }
    int get_label() const { return label; }
    unsigned size() const { return groups.size(); } // num of groups in this cluster
    std::vector<Group>& Groups() { return groups; }
    double base();            // base of Area
    double height();          // height of Area
    unsigned locations_num(); // number of locations in this cluster
    unsigned people_num();    // number of people in the cluster
    double get_LATP() const { return LATP_alpha; }
    Position gen_group_center(int num_of_loc); // generate a valid center location for the next group
    void generate_path(int to_visit, std::vector<Location*>& path, Random& rng);
    Location* select_location(unsigned n); // select the nth location and return his pointer, used for generate_path
    //    std::vector<Location*> Location_list(); // get a vector of pointer to all locations in the cluster
    //    std::vector<Person*> Person_list();     // get a vector of pointer to all People in the cluster
};

} // namespace smooth_sim

#endif