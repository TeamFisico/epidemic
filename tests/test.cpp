#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../src/SEIR.cpp"

TEST_CASE("test risoluzione eq diff") {
    State inizio{800, 100, 80, 20, 1000};
    sim prova{inizio, 0.15, 0.4, 0.24};
    auto const result = prova.generate_all_points(100);
    auto it = result.end();
    --it;
    CHECK((it->S + it->E + it->I + it->R) == (it->N));
}