#include "person.hpp"

namespace SMOOTH
{
Person::Person()
{
    status = Status::Susceptible;
    label = 0;
    home = {0.0, 0.0};
    location = {0.0, 0.0};
}
double Person::speed() const
{
    return sqrt( velocity[0] * velocity[0] + velocity[1]* velocity[1]);
}


} // namespace SMOOTH