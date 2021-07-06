#ifndef WORLD_HPP
#define WORLD_HPP
#include "cluster.hpp"

namespace smooth_sim
{
class World
{
  private:
    Rectangle Area;
    std::vector<Cluster> Clusters;
    Random wrld_eng;

  public:
    // Constructor
    World(double Side_length, int number_of_clusters, int number_of_locations, int S, int E, int I, int R);

    // Disable compiler generated default constructor
    World() = delete;

  private:
    // fill the input vector with the number of locations to be assigned to each cluster(corresponding index)
    void fill_with_locations_num(unsigned clusters_num, int locations_num, std::vector<int>& loc_num);

    // fill input vector with S individuals with an index corresponding to their belonging cluster
    void fill_with_S_individuals(unsigned clusters_num, int S, std::vector<int>& S_v);

    // fill 3 vectors with the number of (E,I,R respectively) individuals for each cluster using an uniform distribution
    // assigning a block of people at a time(or the remaining ones)
    void fill_with_E_I_R_individuals(unsigned clusters_num, int E, int I, int R, std::vector<int>& E_v,
                                     std::vector<int>& I_v, std::vector<int>& R_v);

  public:
    // Let Simulation be a friend, being able to access to private members
    friend class Simulation;

    // number of clusters in this World
    unsigned size() const;

    // number of locations in this World
    unsigned locations_num() const;

    // number of people in this World
    unsigned people_num() const;

    // returns vector containing clusters in World
    std::vector<Cluster> get_clusters() const;

    // returns reference to the vector containing all World's clusters
    std::vector<Cluster>& clusters();

    // generates path(locations to visit) by invoking each cluster's path generation
    void generate_path(int to_visit, const std::vector<double>& weights, std::vector<Location*>& path, Random& engine);
};

// unused functions
//    std::vector<Location*> Location_list();  // get a vector of pointer to all locations in the world
//    std::vector<Person*> Total_Population(); // get a vector of pointer to all Person in the world
//    Cluster* select_cluster();               // randomly chose a cluster based on number of Locations
//    Cluster* get_cluster(int index);         // return a pointer to index cluster
// std::vector<Location*> generate_path(std::vector<Location*> list, double mean,double dev); // generate a random
// number of pointer to Location from the provided one

} // namespace smooth_sim

#endif