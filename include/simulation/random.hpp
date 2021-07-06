//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The wrapper class 'Random' implements the random features needed for this project gotten from the header library //
// https://gist.github.com/imneme/540829265469e673d045 which guarantees high entropy seeding which sometimes may not be
// // achieved through std::random_device. Additionally it implements some random operations useful for our Simulation.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef RANDOM_HPP
#define RANDOM_HPP
#include "../../extern/randutils.hpp"
#include <vector>

using namespace randutils;

namespace smooth_sim
{

class Random
{
  private:
    mt19937_rng eng; // Marsenne Twister engine
  public:
    // Default construct a 256-bit seeded engine
    Random();
    //    mt19937_rng engine() const { return eng; } // return the engine to perform other actions

    // returns a double uniformly in [lower,upper) range
    double uniform(double lower, double upper);

    // extract an int uniformly in [lower,upper] range
    int int_uniform(int lower, int upper);

    // returns a double extracted from a normal distribution
    double gauss(double mean, double stddev);

    // returns a int gotten from rounding the double extracted from the normal distribution
    int rounded_gauss(double mean, double stddev);

    // returns an int in the range [0,weights.size()-1] based on the input weights vector
    int discrete(std::vector<double> weights);

    // returns true based on an input probability(uniform)(used to determine if an event has to happen or not)
    bool try_event(double prob);

    // returns a stay(measured in steps of the simulation) extracted from the chosen TPL(Truncated Power Law)
    int rand_stay();

    // returns a uniformly generated random speed(then assigned to a person)
    double rand_speed();

    // returns a random radius according to the chosen parameters
    double rand_radius();
};

} // namespace smooth_sim

#endif // RANDOM_HPP
