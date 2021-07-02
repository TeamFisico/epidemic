#ifndef POSITION_HPP
#define POSITION_HPP

#define PI 3.14159265358979323846 // pi

#include "../random.hpp"

namespace smooth_simulation
{
class Position
{
  private:
    double x;
    double y;

  public:
    Position(double X, double Y); // constructor
    Position();                   // default constructor

    double get_X() const { return x; }
    double get_Y() const { return y; }

    double distance_to(Position const& other_pos) const;
    bool in_radius(Position const& other_pos, double r) const;
    void check_bounds_and_resize(double area_side);
    void move_toward(Position target, double speed, Random& engine);
};

bool operator==(const Position& p1, const Position& p2);
Position generate_close_position(Position const& center, double max_distance,Random& engine);

} // namespace smooth_simulation





#endif // POSITION_HPP