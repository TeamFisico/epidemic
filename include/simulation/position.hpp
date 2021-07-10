#ifndef POSITION_HPP
#define POSITION_HPP
#include "random.hpp"

namespace smooth_sim
{
class Position
{
  private:
    double x; // x coordinate of the point(position)
    double y; // y coordinate of the point(position)

  public:
    // Constructor
    Position(double X, double Y);

    // Default constructor
    Position();

    // Returns x coordinate
    double get_x() const;

    // Returns y coordinate
    double get_y() const;

    // Returns the value of the distance between this position and another position
    double distance_to(Position const& a) const;

    // Returns true if two positions are close enough(within r to each other)
    bool in_radius(Position other, double r) const;

    // Move this position closer to a specific target at a certain speed; do that randomly varying the angle connecting
    // the two positions(points)
    void move_toward(Position target, double speed, Random& engine);
};

// Returns a randomly generated position inside the rectangular area defined by blh and trh borders
Position rand_pos(Position blh_corner, Position trh_corner, Random& eng);

} // namespace smooth_sim

#endif // POSITION_HPP