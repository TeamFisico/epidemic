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
