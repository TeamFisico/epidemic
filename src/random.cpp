#include "../include/random.hpp"
#include <cassert>

namespace smooth_simulation
{
Random::Random(int seeding_type)
{
    assert(seeding_type == 256 || seeding_type == 128);
    if (seeding_type == 128)
    {
        mt19937_rng eng1{auto_seed_128{}}; // 128 bit seeding
        eng = eng1;
    }
    else if (seeding_type == 256)
    {
        mt19937_rng eng2{auto_seed_256{}}; // 256 bit seeding
        eng = eng2;
    }
}

Random::Random()
{
    mt19937_rng default_engine; // default seed engine with 256 bits of entropy (See
                                // https://www.pcg-random.org/posts/simple-portable-cpp-seed-entropy.html)
    eng = default_engine;
}

double Random::uniform(double lower, double upper)
{
    return eng.uniform(lower, upper);
}
int Random::int_uniform(int lower, int upper)
{
    return eng.variate<int, std::uniform_int_distribution>(lower, upper);
}
double Random::gauss(double mean, double stddev)
{
    return eng.variate<double, std::normal_distribution>(mean, stddev);
}
int Random::rounded_gauss(double mean, double stddev)
{
    return round(eng.variate<double, std::normal_distribution>(mean, stddev));
}

} // namespace SMOOTH