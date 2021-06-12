//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The wrapper class 'Random' implements the random features needed for this project gotten from the header library     //
// https://gist.github.com/imneme/540829265469e673d045 which guarantees high entropy seeding which sometimes may not be //
// achieved through std::random_device. Additionally it implements some random operations useful for our Simulations.   //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef RANDOM_HPP
#define RANDOM_HPP
#include "../randutils.hpp"

using namespace randutils;

namespace SMOOTH
{

class Random
{
  private:
    mt19937_rng engine;   //Marsenne Twister engine
  public:
    explicit Random(int seeding_type);    //engine constructor
    Random();                             //default constructor
    double uniform(double lower, double upper); //extract a double uniformly in [lower,upper) range
    int rounded_uniform(double lower, double upper); //extract an int uniformly in [lower,upper) range
    double gauss(double mean, double stddev);
    int rounded_gauss(double mean, double stddev);

};

} // namespace SMOOTH


#endif // RANDOM_HPP