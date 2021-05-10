
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
};

Position rand_pos(double x_min, double x_max, double y_min, double y_max); // get a random position
double rand_gauss(double mean, double deviation); // get a random number from gaussian distribution

} // namespace sim