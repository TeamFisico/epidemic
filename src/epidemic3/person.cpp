#include "person.hpp"

namespace SMOOTH
{
Person::Person()
{
    status = Status::Susceptible;
    label = 0;
    home = {0.0, 0.0};
    current_location = {0.0, 0.0};
}

Location *Person::get_home()
{
    return &home;
}

} // namespace SMOOTH