#include "cluster.hpp"

namespace sim{

class World{
  private:
    Rectangle Area;
    std::vector<Cluster> clusters;
  public:
    World(double Side_lenght, int number_of_clusters, int number_of_groups, int number_of_location, int Population, int S, int E, int I, int R);
    World();
    std::vector<Location*> Location_list(); //get a vector of pointer to all locations in the world
    std::vector<Person*> Total_Population(); //get a vector of pointer to all Person in the world
    Cluster* select_cluster(); //randomly chose a cluster based on number of Locations
    int number_of_locations(); //get the number of total locations in the world
    int number_of_people(); //get the number of total people in the world
};

std::vector<Location*> generate_path(std::vector<Location*> list, double mean, double dev); //generate a random number of pointer to Location from the provided one

}