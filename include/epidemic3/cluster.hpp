#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include "group.hpp"
#include "person.hpp"

namespace smooth_simulation
{

struct Data
{
    unsigned int S;
    unsigned int E;
    unsigned int I;
    unsigned int R;
    unsigned int D;
    unsigned int ICU_capacity;
    Data(unsigned int susceptible, unsigned int latent, unsigned int  infected, unsigned int  recovered, unsigned int  dead,unsigned int capacity);;
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
    int sz;                        // num of waypoints
    int lbl;                       // corresponding index into Cluster array 0 <= lbl <= clusters_size-1
    double w;                      // weight to be chosen by a person
    Zone zone;                     // Restriction type for this zone
    double alpha;                  // LATP_parameter of this Cluster
    std::array<double, 4> limits;  // [0]= lower_x, [1] = upper_x, [2] = lower_y, [3] = upper_y
    std::array<int, 2> wpts_range; // starting and ending index of this groups waypoints inside Waypoints array
    Data data;                     // edpidemic data relative to the people in this cluster
    int ICU_sz;              //number of people that can be hospitalized in ICU 
  public:
    std::vector<int> People_i; // ref to People array for people in this clust(assign_to_clust() initialized)
    std::vector<Group> Groups; // groups of waypoints in cluster
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    Cluster(int size, int label, double weight, Zone zone, double alpha, double x_low, double x_up, double y_low,
            double y_up, Data cluster_data);
    Cluster();
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    int size() const { return sz; }
    Zone zone_type() const { return zone; }
    int label() const { return lbl; }
    double weight() const { return w; }
    double LATP_parameter() const { return alpha; }
    double lower_x() const { return limits[0]; }
    double upper_x() const { return limits[1]; }
    double lower_y() const { return limits[2]; }
    double upper_y() const { return limits[3]; }
    int lower_index() const { return wpts_range[0]; }
    int upper_index() const { return wpts_range[1]; }
    Data get_data() const { return data; }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    void set_limits();
    void set_size(int n) { sz = n; }
    void set_label(int n) { lbl = n; }
    void set_weight(double weight) { w = weight; };
    void set_LATP_parameter(double param){ alpha = param; }
    void set_zone(Zone newZone) { zone = newZone; }
    void set_lower_index(int n) { wpts_range[0] = n; }
    void set_upper_index(int n) { wpts_range[1] = n; }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    void update_data();
    void generate_groups(Random& engine); // determine the n. of waypoints associated to every group
    void partition_in_groups(Random& engine);
    void move(); // move people belonging to this cluster
    void clear_dead_people(); //removes dead people indeces from People_i -->won't be considered anymore
};
void generate_groups(Cluster const& cluster,Random& engine); // determine the n. of waypoints associated to every group
std::vector<int> available_white_clusters(int lbl);

} // namespace smooth_simulation

#endif // CLUSTER_HPP
