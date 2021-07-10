#include "simulation.hpp"
#include "doctest.h"

using namespace smooth_sim;

TEST_CASE("sim generation")
{
    Random rng;
    for(int i = 0; i < 10; ++i) //test 10 time if it generate all correctly
    {
        //randomize the various parameters, making sure they are in the decided boundaries
        int n_clusters = rng.int_uniform(1,40);
        int n_location = rng.int_uniform(n_clusters*10, n_clusters*200);
        int side = rng.int_uniform(n_location/2, 2*n_location);
        int S = rng.int_uniform(n_clusters*10, n_clusters*200);
        int E = rng.int_uniform(n_clusters*10, n_clusters*200);
        int I = rng.int_uniform(n_clusters*10, n_clusters*200);
        int R = rng.int_uniform(n_clusters*10, n_clusters*200);
        Simulation test{S, E, I,R,n_clusters,n_location,side,0.1,0.1,0.1,1}; //generate the simulation
        //Check if everything is generated correctly
        CHECK(test.world().size() == n_clusters);
        CHECK(test.world().locations_num() == n_location);
        CHECK(test.world().get_side() == side);
        Data data = test.get_data();
        CHECK(data.S == S);
        CHECK(data.E == E);
        CHECK(data.I == I);
        CHECK(data.R == R);
    }
}