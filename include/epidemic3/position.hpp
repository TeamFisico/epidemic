#ifndef POSITION_HPP
#define POSITION_HPP

#define PI 3.14159265358979323846 // pi

namespace smooth_simulation
{
  class Position{
    private:
      double x;
      double y;

    public:
      Position(double X,double Y);  //constructor
      Position();                   //default constructor

      double get_X() const { return x; }
      double get_Y() const { return y; }
      void set_x(double X) { x = X; }
      void set_y(double Y) { y = Y; }

      double distance_to(Position const& other_pos) const;
      bool in_radius(Position const& other_pos,double r) const;

  };

  bool operator==(const Position& p1, const Position& p2);
  Position generate_close_position(Position const& center,double max_distance);


}



#endif //POSITION_HPP