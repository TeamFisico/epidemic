#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../src/SEIR.cpp"

TEST_CASE("test risoluzione eq diff") {
    State inizio{1000000, 0, 0, 0, 1000000};
    sim prova{inizio, 0.15, 1.4, 0.24};
    std::vector<State> result = prova.generate_all_points(100);
    auto it = result.end - 1;
    CHECK(static_cast<int>((it->S + it->E + it->I + it->R + )) == N);
}