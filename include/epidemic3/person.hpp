
#include "location.hpp"
#include "mobility_model.hpp"

namespace SMOOTH
{

enum class Status
{
    Susceptible = 0,
    Latent,
    Infected,
    Removed
};

class Person
{
  private:
    Location location;
    double y;
    Status disease_status;
    MobilityModel *mobility_model;
};

} // namespace SMOOTH