#include "../include/random.hpp"
#include "../include/epidemic3/parameters.hpp"
#include <cassert>

namespace smooth_simulation
{
/////////////////////////////////////////////////////
////////     RANDOM DEFAULT CONSTRUCTOR       ///////
/////////////////////////////////////////////////////
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
int Random::discrete(std::vector<double> weights)
{
    return eng.variate<int, std::discrete_distribution>(std::begin(weights), std::end(weights));
}
int Random::rand_stay()
{
    double term1 = 0.0;
    double term2 = 0.0;
    double term3 = 0.0;
    double term4 = 0.0;
    double pause_time = 0.0;

    double u = uniform(0, 1); // number in range [0,1)

    term1 =
        (u * pow(MAX_PAUSE, PAUSE_EXPONENT)) - (u * pow(MIN_PAUSE, PAUSE_EXPONENT)) - pow(MAX_PAUSE, PAUSE_EXPONENT);
    term2 = pow(MAX_PAUSE, PAUSE_EXPONENT) * pow(MIN_PAUSE, PAUSE_EXPONENT);
    term3 = -(term1 / term2);
    term4 = pow(term3, (-1 / PAUSE_EXPONENT));
    pause_time = term4;
    return round(pause_time);
}
double Random::rand_speed()
{
    return gauss(MEAN_SPEED, uniform(0, MAXIMUM_VELOCITY_STDDEV));
}
bool Random::try_event(double probability)
{
    return uniform(0, 1) <= probability;
}

} // namespace smooth_simulation