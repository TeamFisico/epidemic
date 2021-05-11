#include "person.hpp"
#include "group.hpp"

namespace sim{

class Cluster{
  private:
    std::vector<Person> Population;
    std::vector<Group> groups;
  public:
    std::vector<Location*> Location_list();//get a vector of pointer to all locations in the cluster
    std::vector<Person*> Person_list();//get a vector of pointer to all People in the cluster
    int number_of_locations(); //return the number of locations in the cluster
    int number_of_people(); //return the number of people in the cluster

};

}