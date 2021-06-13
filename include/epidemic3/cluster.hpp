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
              //    Data data;
    Zone zone;
    std::array<double, 4> limits; // [0]= lower_x, [1] = upper_x, [2] = lower_y, [3] = upper_y

  private:
    void determine_groups_sizes(); // determine the n. of waypoints associated to every group
  public:
    std::vector<int>
        People_index; // indeces of People array to people belonging to this cluster-->see assign_to_cluster() method
    std::vector<Group> Groups; // groups of waypoints in cluster
    // constructors
    Cluster(int size, int label, double weight, Zone zone /*,Data data*/, double x_low, double x_up, double y_low,
            double y_up);
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
    Data get_data() const;
    // modifying members
    void set_limits();
    void set_size(int n) { sz = n; }
    void set_label(int n) { lbl = n; }
    void set_weight(double weight) { w = weight; };
    void set_zone(Zone newZone) { zone = newZone; }
    void partition_in_groups();
};

} // namespace SMOOTH

#endif // CLUSTER_HPP
