#ifndef POSITION_HPP
#define POSITION_HPP

namespace smooth_simulation
{
  class Position{
    private:
      double x;
      double y;

    public:
      Position(double X,double Y);  //constructor
      Position();       //default constructor

      double get_x() const { return x; }
      double get_y() const { return y; }
      void set_x(double X) { x = X; }
      void set_y(double Y) { y = Y; }

      double distance_to(Position const& other_pos) const;
      bool in_radius(Position const& other_pos,double r) const;

  };

  bool operator==(const Position& p1, const Position& p2);


}



#endif //POSITION_HPP