#include "location.hpp"
#include <vector>

namespace sim{

class Group{
  private:
    std::vector<Location> locations;
  public:
    Group(int number_of_locations, Position center, double Radius);
    Group();
    Location* get_location(int i); // get pointer to i_nth Location
    std::vector<Location*> Location_list(); // function that return pointer to all locations in the Group

};

}