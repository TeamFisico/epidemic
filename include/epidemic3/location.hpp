#ifndef LOCATION_HPP
#define LOCATION_HPP

namespace SMOOTH
{

class Location
{
  private:
    double x;
    double y;

  public:
    Location(double X, double Y); // constructor
    Location();                   // default constructor

    double X() const { return x; }
    double Y() const { return y; }
    double& X() { return x; }
    double& Y() { return y; }

    double distance(Location loc2) const;
    bool in_radius(Location loc2, double r) const;
    bool within_transmission_range(Location loc2) const;
};

  bool operator==(const Location& l1, const Location& l2);  //equality operator


} // namespace SMOOTH

#endif // LOCATION_HPP