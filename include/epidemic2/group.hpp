#include "location.hpp"
#include <vector>

namespace sim{

class Group{
  private:
    std::vector<Location> locations;
  public:
    Location* get_location(int i); // get pointer to i_nth Location
    std::vector<Location*> Location_list(); // function that return pointer to all locations in the Group

};

}