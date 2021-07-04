#ifndef WORLD_HPP
#define WORLD_HPP
#include "cluster.hpp"

namespace smooth_sim
{
class World
{
  private:
    Rectangle Area;
    std::vector<Cluster> clusters;
    Random wrld_eng;

  public:
    World(double Side_length, int number_of_clusters, int number_of_location, int S, int E, int I, int R);
    World() = delete;
  private:

  public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    unsigned int size() const { return clusters.size(); }  //num of clusters in World
    unsigned int locations_num(); // get the number of total locations in the world
    unsigned int people_num();    // get the number of total people in the world
    std::vector<Cluster>& Clusters() { return clusters; };
    void generate_path(int to_visit, const std::vector<double>& weights, std::vector<Location*>& path,
                       Random& simulation_engine);
    // unused functions
    //    std::vector<Location*> Location_list();  // get a vector of pointer to all locations in the world
    //    std::vector<Person*> Total_Population(); // get a vector of pointer to all Person in the world
    //    Cluster* select_cluster();               // randomly chose a cluster based on number of Locations
    //    Cluster* get_cluster(int index);         // return a pointer to index cluster
};

// unused
// std::vector<Location*> generate_path(std::vector<Location*> list, double mean,double dev); // generate a random
// number of pointer to Location from the provided one

} // namespace smooth_sim

#endif