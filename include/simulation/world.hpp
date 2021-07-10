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
    int side;

  public:
    // Constructor
    World(int side_length, int number_of_clusters, int number_of_locations, int S, int E, int I, int R);

    // Prevent the compiler from generating the default constructor since no use of it should be made
    World() = delete;

  private:
    // Fill the input vector with the number of locations to be assigned to each cluster(corresponding index)
    void fill_with_locations_num(unsigned clusters_num, int locations_num, std::vector<int>& loc_num);

    // Fill input vector with S individuals with an index corresponding to their belonging cluster
    void fill_with_S_individuals(unsigned clusters_num, int S, std::vector<int>& S_v);

    // Fill 3 vectors with the number of (E,I,R respectively) individuals for each cluster using an uniform distribution
    // assigning a block of people at a time(or the remaining ones)
    void fill_with_E_I_R_individuals(unsigned clusters_num, int E, int I, int R, std::vector<int>& E_v,
                                     std::vector<int>& I_v, std::vector<int>& R_v);

  public:
    // Let Simulation be a friend, being able to access to private members
    friend class Simulation;

    // Number of clusters in this World
    unsigned size() const;

    // Number of locations in this World
    unsigned locations_num() const;

    // Number of people in this World
    unsigned people_num() const;

    // Return side of Area
    unsigned get_side() const;

    // Returns reference to the vector containing all World's clusters
    std::vector<Cluster>& clusters();

    // Generates path(locations to visit) by invoking each cluster's path generation
    void generate_path(int to_visit, const std::vector<double>& weights, std::vector<Location*>& path, Random& engine);
};

} // namespace smooth_sim

#endif