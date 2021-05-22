#ifndef POSITION_HPP
#define POSITION_HPP

namespace sim
{
class Position
{
  private:
    double x;
    double y;

  public:
    Position(double x, double y); // Constructor
    Position();                   // Default constructor
    bool InRadius(Position other,
                  double r);                         // Function that determine if two position are close enough
    void move_toward(Position target, double speed); // move position closer to target
    double distance_to(Position const &a);           // return the distance from the 2 Position
    double get_x();                                  // return the x coordinate
    double get_y();                                  // return the y coordinate
};

Position rand_pos(Position blh_corner, Position trh_corner); // get a random position
double rand_gauss(double mean, double deviation); // get a random number from gaussian distribution
int rounded_norm(double mean,
                 double deviation); // generate a random number using rand_gauss and round it to closest integer.
bool try_event(double probability);

} // namespace sim

#endif // POSITION_HPP