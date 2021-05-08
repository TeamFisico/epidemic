
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
    bool InRadius(Position &other,
                  double r); // Function that determine if two position are close enough
};

Position rand_pos(double x_limit, double y_limit);

}