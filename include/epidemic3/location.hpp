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
    // define copy!!

    double X() const { return x; }
    double Y() const { return y; }

    double &X() { return x; }
    double &Y() { return y; }

    double get_distance(Location loc2) const;
    bool in_radius(Location loc2, double r) const;
};

} // namespace SMOOTH

#endif // LOCATION_HPP