#ifndef LOCATION_HPP
#define LOCATION_HPP

namespace SMOOTH
{

struct Location
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

    double get_distance(Location loc2) const;
    bool in_radius(Location loc2, double r) const;

};

// define when two locations are equal
bool operator==(const Location& l1, const Location& l2)
{
    return (l1.X() == l2.X() && l1.Y() == l2.Y());
}
} // namespace SMOOTH

#endif // LOCATION_HPP