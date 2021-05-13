#include "person.hpp"
#include "group.hpp"
#include "rectangle.hpp"

namespace sim{

class Cluster{
  private:
    std::vector<Person> Population;
    std::vector<Group> groups;
    Rectangle Area;
  public:
    Cluster(int N, int S, int E, int I, int R, int number_of_groups, Position blh_corner, Position trh_corner);
    Cluster();
    std::vector<Location*> Location_list();//get a vector of pointer to all locations in the cluster
    std::vector<Person*> Person_list();//get a vector of pointer to all People in the cluster
    int number_of_locations(); //return the number of locations in the cluster
    int number_of_people(); //return the number of people in the cluster

};

}