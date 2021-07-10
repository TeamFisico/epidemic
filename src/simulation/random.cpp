#include "random.hpp"
#include "parameters.hpp"
#include <algorithm>
#include <cassert>

namespace smooth_sim
{
/////////////////////////////////////////////////////
////////     RANDOM DEFAULT CONSTRUCTOR       ///////
/////////////////////////////////////////////////////
Random::Random()
{
    mt19937_rng default_engine; // Default seed engine with 256 bits of entropy (See
                                // https://www.pcg-random.org/posts/simple-portable-cpp-seed-entropy.html)
    eng = default_engine;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////           PUBLIC METHODS            /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////// UNIFORM DISTRIBUTION EXTRACTION /////////////////
double Random::uniform(double lower, double upper)
{
    return eng.uniform(lower, upper);
}
///////////////// INT UNIFORM DISTRIBUTION EXTRACTION /////////////////
int Random::int_uniform(int lower, int upper)
{
    return eng.variate<int, std::uniform_int_distribution>(lower, upper);
}
///////////////// NORMAL DISTRIBUTION EXTRACTION /////////////////
double Random::gauss(double mean, double stddev)
{
    return eng.variate<double, std::normal_distribution>(mean, stddev);
}
int Random::rounded_gauss(double mean, double stddev)
{
    return round(eng.variate<double, std::normal_distribution>(mean, stddev));
}
///////////////// DISCRETE DISTRIBUTION EXTRACTION /////////////////
// https://en.cppreference.com/w/cpp/numeric/random/discrete_distribution/discrete_distribution
int Random::discrete(std::vector<double> weights)
{
    return eng.variate<int, std::discrete_distribution>(weights.begin(), weights.end());
}
///////////////// EVENT OCCURENCE DETERMINATION /////////////////
bool Random::try_event(double probability)
{
    return uniform(0, 1) <= probability;
}
///////////////// RANDOM STAY DETERMINATION /////////////////
int Random::rand_stay()
{
    double t1{};
    double t2{};
    double t3{};
    double t4{};
    double pause_time{};

    // Number in range [0,1)
    double u = uniform(0, 1);

    // Apply the power rule of SMOOTH paper
    t1 = (u * pow(MAX_PAUSE, PAUSE_EXPONENT)) - (u * pow(MIN_PAUSE, PAUSE_EXPONENT)) - pow(MAX_PAUSE, PAUSE_EXPONENT);
    t2 = pow(MAX_PAUSE, PAUSE_EXPONENT) * pow(MIN_PAUSE, PAUSE_EXPONENT);
    t3 = -(t1 / t2);
    t4 = pow(t3, (-1 / PAUSE_EXPONENT));
    pause_time = t4;
    return round(pause_time);
}
///////////////// RANDOM SPEED DETERMINATION /////////////////
double Random::rand_speed()
{
    return std::abs(gauss(MEAN_SPEED, SPEED_STDDEV));
}
///////////////// RANDOM LOCATION RADIUS DETERMINATION /////////////////
double Random::rand_radius()
{
    // Generate the random radius
    double radius = gauss(LOCATION_RADIUS_MEAN, LOCATION_RADIUS_DEV);
    // Check if it is bigger than the minimum radius
    if (radius < MINIMUM_LOCATION_RADIUS) { radius = MINIMUM_LOCATION_RADIUS; }
    return radius;
}

} // namespace smooth_sim
