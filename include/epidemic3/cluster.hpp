#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include "group.hpp"
#include "person.hpp"

namespace SMOOTH
{

enum class Zone
{
    white = 0,
    yellow,
    orange,
    red
};

class Cluster
{
  private:
    int sz;   // num of waypoints
    int lbl;  // corresponding index into Cluster array 0 <= lbl <= clusters_size-1
    double w; // weight to be chosen by a person
    Zone zone;
    std::array<double, 4> limits; // [0]= lower_x, [1] = upper_x, [2] = lower_y, [3] = upper_y
  public:
    std::vector<Group> Groups; // groups of waypoints in cluster
    std::vector<Person*> Cluster_People;  //pointer to people currently in this cluster

    explicit Cluster(int size); // constructor
    Cluster();                  // default constructor

    // non-modifying members
    int size() const { return sz; }
    Zone zone_type() const { return zone; }
    int label() const { return lbl; }
    double weight() const { return w; }
    double lower_x() const { return limits[0]; }
    double upper_x() const { return limits[1]; }
    double lower_y() const { return limits[2]; }
    double upper_y() const { return limits[3]; }

    void set_weight(double weight) { w = weight; };
    void set_label(int n) { lbl = n; }
    void set_zone(Zone newZone) { zone = newZone; }

    int &size() { return sz; }
    void determine_groups_sizes(); // determine the n. of waypoints associated to every group
    void set_limits();
};

} // namespace SMOOTH

#endif // CLUSTER_HPP
