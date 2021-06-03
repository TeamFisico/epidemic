#ifndef CLUSTER_HPP
#define CLUSTER_HPP
#include "person.hpp"
#include "group.hpp"
#include "rectangle.hpp"
#include "mobility_model.hpp"

namespace sim{

enum class Color{Green, Yellow, Red};

class Cluster{
  private:
    std::vector<mobility_model> Population;
    std::vector<Group> groups;
    Rectangle Area;
    Color color;
    int cluster_index;
  public:
    Cluster(int S, int E, int I, int R, int number_of_location, Rectangle Area, Color color, int cluster_index);
    Cluster();
    std::vector<Location*> Location_list();//get a vector of pointer to all locations in the cluster
    std::vector<Person*> Person_list();//get a vector of pointer to all People in the cluster
    std::vector<Group> &Groups(){ return groups; }
    int number_of_locations(); //return the number of locations in the cluster
    int number_of_people(); //return the number of people in the cluster
    Position gen_group_center(int num_of_loc); //generate a valid center location for the next group
    Color get_color(){ return color; }
    std::vector<mobility_model> &population(){ return Population; }
    std::vector<Location*> generate_path(double mean, double dev);

};

}

#endif