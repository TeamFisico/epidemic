#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include "group.hpp"
#include "person.hpp"

namespace smooth_simulation
{

struct Data
{
    int S;
    int E;
    int I;
    int R;
    int D;
    Data(int susceptible, int latent, int infected, int recovered, int dead);
};
enum class Zone
{
    White = 0,
    Yellow,
    Orange,
    Red
};

class Cluster
{
  private:
    int sz;   // num of waypoints
    int lbl;  // corresponding index into Cluster array 0 <= lbl <= clusters_size-1
    double w; // weight to be chosen by a person
    Zone zone;           //Restriction type for this zone
    double alpha;        //LATP_parameter of this Cluster
    std::array<double, 4> limits; // [0]= lower_x, [1] = upper_x, [2] = lower_y, [3] = upper_y
    Data data;

  private:
    void determine_groups_sizes(); // determine the n. of waypoints associated to every group
    void move_people_white();      //move people considering this is  a White cluster
    void move_people_non_white();  //move people considering this is not a White cluster
  public:
    std::vector<int> People_i; // ref to People array for people in this clust(assign_to_clust() initialized)
    std::vector<Group> Groups;     // groups of waypoints in cluster
    // constructors
    Cluster(int size, int label, double weight, Zone zone,double alpha, double x_low, double x_up, double y_low, double y_up,Data cluster_data);
    Cluster();
    // non-modifying members
    int size() const { return sz; }
    Zone zone_type() const { return zone; }
    int label() const { return lbl; }
    double weight() const { return w; }
    double lower_x() const { return limits[0]; }
    double upper_x() const { return limits[1]; }
    double lower_y() const { return limits[2]; }
    double upper_y() const { return limits[3]; }
    double LATP_parameter() const { return alpha; }
    Data get_data() const { return data; }
    // modifying members
    void set_limits();
    void set_size(int n) { sz = n; }
    void set_label(int n) { lbl = n; }
    void set_weight(double weight) { w = weight; };
    void set_zone(Zone newZone) { zone = newZone; }
    void partition_in_groups();

    void move();  //move people belonging to this cluster
};

} // namespace smooth_simulation

#endif // CLUSTER_HPP
