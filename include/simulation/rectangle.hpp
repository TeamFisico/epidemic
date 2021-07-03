#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP
#include "position.hpp"
#include <vector>

namespace smooth_sim
{
// create a Rectangle class to divide the World in n rectangles, that will be clusters
class Rectangle
{
  private:
    Position blh_corner;
    Position trh_corner;

  public:
    Rectangle(Position& blh_corner, Position& trh_corner); // constructor
    Rectangle();
    double get_area(); // return the Area of the rectangle
    Position get_blh_corner() { return blh_corner; }
    Position get_trh_corner() { return trh_corner; }
    std::vector<Rectangle> Split();       // Split the rectangle in two randomly uneven rectangle
    std::vector<Rectangle> Divide(int n); // Divide the Rectangle in n part using Split() function
};

} // namespace smooth_sim

#endif