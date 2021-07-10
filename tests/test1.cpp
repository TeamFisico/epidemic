
#include "seir.hpp"
#include "doctest.h"
#include <vector>

TEST_CASE("Differential equation solving test")
{
    seir::State starting_cond {800,100,80,20};

    seir::SEIR euler_trial{1000,100,starting_cond,0.4,0.15,0.24};
    seir::SEIR rk_trial{1000,100,starting_cond,0.4,0.15,0.24};

    std::vector<seir::State> result1;
    euler_trial.evolve(result1,false);   // use Euler
    auto it1 = result1.end();
    --it1;

    std::vector<seir::State> result2;
    rk_trial.evolve(result2,true);  //use RK_4
    auto it2 = result2.end();
    --it2;

    CHECK(doctest::Approx(it1->S + it1->E + it1->I + it1->R) == 1000.0);
    CHECK(doctest::Approx(it2->S + it2->E + it2->I + it2->R) == 1000.0);
}