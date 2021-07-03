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
    Cluster(int S, int E, int I, int R, int number_of_location, Rectangle Area, Color color, int cluster_index,
            Random const& wrld_engine);
    Cluster() = default;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    Rectangle& cluster_area() { return Area; }
    Color get_color() const { return color; }
    void set_color(Color cluster_color) { color = cluster_color; }
    std::vector<mobility_model>& population() { return Population; }
    int index() const { return cluster_index; }
    std::vector<Group>& Groups() { return groups; }
    double base();
    double height();
    int number_of_locations();                 // return the number of locations in the cluster
    int number_of_people();                    // return the number of people in the cluster
    Position gen_group_center(int num_of_loc); // generate a valid center location for the next group
    void generate_path(int to_visit, std::vector<Location*>& path, Random& rng);
    Location* select_location(int n); // select the nth location and return his pointer, used for generate_path
    //    std::vector<Location*> Location_list(); // get a vector of pointer to all locations in the cluster
    //    std::vector<Person*> Person_list();     // get a vector of pointer to all People in the cluster
};

} // namespace smooth_sim

#endif