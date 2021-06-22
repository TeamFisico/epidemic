//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The wrapper class 'Random' implements the random features needed for this project gotten from the header library //
// https://gist.github.com/imneme/540829265469e673d045 which guarantees high entropy seeding which sometimes may not be
// // achieved through std::random_device. Additionally it implements some random operations useful for our Simulation.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef RANDOM_HPP
#define RANDOM_HPP
#include "randutils.hpp"
#include <vector>

using namespace randutils;

namespace sim
{

class Random
{
  private:
    mt19937_rng eng; // Marsenne Twister engine
  public:
    Random();                                  // engine constructor
    mt19937_rng engine() const { return eng; } // return the engine to perform other actions

    // extract one number from a distributions
    double uniform(double lower, double upper); // extract a double uniformly in [lower,upper) range
    int int_uniform(int lower, int upper);      // extract an int uniformly in [lower,upper] range
    double gauss(double mean, double stddev);
    int rounded_gauss(double mean, double stddev);
    int discrete(std::vector<double> weights);
    int piecewise(std::vector<int> nums,
                  std::vector<double> weights); // fill a container with numbers generated according to a distribution
                                                // (see randutils::generate)
    bool try_event(double prob);
    int rand_stay();
};

} // namespace smooth_simulation

#endif // RANDOM_HPP