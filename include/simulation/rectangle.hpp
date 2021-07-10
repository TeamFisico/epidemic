#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP
#include "position.hpp"
#include <vector>

namespace smooth_sim
{

class Rectangle
{
  private:
    Position blh_corner; // Bottom-left hand corner
    Position trh_corner; // Top-right hand corner

  public:
    // Constructor
    Rectangle(Position& blh_corner, Position& trh_corner);
    // Default Constructor
    Rectangle();

    // Return the Area of the rectangle
    double get_area();

    // Return the bottom-left corner of the rectangle
    Position get_blh_corner();

    // Return the top-right corner of the rectangle
    Position get_trh_corner();

    // Split the rectangle in two randomly uneven rectangle
    std::vector<Rectangle> split(Random& engine);

    // Divide the Rectangle in n part using Split() function
    std::vector<Rectangle> divide(int areas_to_divide, Random& engine);
};

} // namespace smooth_sim

#endif