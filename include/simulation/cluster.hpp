#ifndef CLUSTER_HPP
#define CLUSTER_HPP
#include "group.hpp"
#include "mobility_model.hpp"
#include "person.hpp"
#include "random.hpp"
#include "rectangle.hpp"

namespace smooth_sim
{

enum class Color
{
    Green,
    Yellow,
    Red
};

class Cluster
{
  private:
    std::vector<mobility_model> Population;
    std::vector<Group> groups;
    Rectangle Area;
    Color color;
    int cluster_index;
    Random cl_engine;

  public:
    Cluster(int S, int E, int I, int R, int number_of_location, Rectangle Area, Color color, int cluster_index,Random wrld_engine);
    Cluster();
    Rectangle& area() { return Area; }
    double base() { return Area.get_trh_corner().get_x() - Area.get_blh_corner().get_x(); }
    double height() { return Area.get_trh_corner().get_y() - Area.get_blh_corner().get_y(); }
//    std::vector<Location*> Location_list(); // get a vector of pointer to all locations in the cluster
//    std::vector<Person*> Person_list();     // get a vector of pointer to all People in the cluster
    std::vector<Group>& Groups() { return groups; }
    int index() { return cluster_index; }
    int number_of_locations();                 // return the number of locations in the cluster
    int number_of_people();                    // return the number of people in the cluster
    Position gen_group_center(int num_of_loc); // generate a valid center location for the next group
    Color get_color() const { return color; }
    void set_color(Color cluster_color){ color = cluster_color; }
    std::vector<mobility_model>& population() { return Population; }
    void generate_path(int to_visit, std::vector<Location*>& path, Random& rng);
    Location* select_location(int n); // select the nth location and return his pointer, used for generate_path
};

} // namespace smooth_sim

#endif