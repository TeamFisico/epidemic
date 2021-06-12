#include "random.hpp"
#include <cassert>

namespace SMOOTH
{
Random::Random(int seeding_type)
{
    assert(seeding_type == 256 || seeding_type == 128);
        if (seeding_type == 128)
        {
            mt19937_rng eng{auto_seed_128{}};  //128 bit seeding
            engine = eng;
        } else if (seeding_type == 256)
        {
            mt19937_rng eng{auto_seed_256{}};   //256 bit seeding
            engine = eng;
        }
}

Random::Random()
{
     mt19937_rng default_engine; //default seed engine with 256 bits of entropy (See https://www.pcg-random.org/posts/simple-portable-cpp-seed-entropy.html)
     engine = default_engine;
}

}