
#include "seir1.hpp"
#include "seir2.hpp"
#include "doctest.h"

TEST_CASE("test risoluzione eq diff")
{
    Euler::State inizio1{800, 100, 80, 20, 1000};
    Euler::sim prova1{inizio1, 0.15, 0.4, 0.24};
    seir::State inizio2{800,100,80,20};
    seir::ode prova2{1000,100,inizio2,0.4,0.15,0.24};
    auto const result1 = prova1.generate_all_points(100);
    auto it1 = result1.end();
    --it1;
    std::vector<seir::State> result2;
    seir::simulation(prova2, result2);
    auto it2 = result2.end();
    --it2;
    CHECK(doctest::Approx(it1->S + it1->E + it1->I + it1->R) == (it1->N));
    CHECK(doctest::Approx(it2->S + it2->E + it2->I + it2->R) == 1000.0);
}