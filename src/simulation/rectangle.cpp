#include "rectangle.hpp"
#include "random.hpp"
#include <random>

namespace smooth_sim
{

Rectangle::Rectangle(Position& blh_corner, Position& trh_corner) : blh_corner{blh_corner}, trh_corner{trh_corner}
{
}

Rectangle::Rectangle() : blh_corner{}, trh_corner{}
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////           PUBLIC METHODS            /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////// GET AREA /////////////////
double Rectangle::get_area()
{
    return (trh_corner.get_x() - blh_corner.get_x()) * (trh_corner.get_y() - blh_corner.get_y());
}
///////////////// GET BOTTOM-LEFT HAND CORNER /////////////////
Position Rectangle::get_blh_corner()
{
    return blh_corner;
}
///////////////// GET TOP-RIGHT HAND CORNER /////////////////
Position Rectangle::get_trh_corner()
{
    return trh_corner;
}
///////////////// SPLIT /////////////////
std::vector<Rectangle> Rectangle::split(Random& engine)
{
    std::vector<Rectangle> result;
    result.clear();
    // Select the random side to divide, if 0 select the x-axis side, if 1 select the y-axis side
    int selected_side = engine.int_uniform(0, 1);
    // Make sure that rectangle that have a side at least three times the other have the longest_side as the selected
    if ((trh_corner.get_x() - blh_corner.get_x()) <= 2 * (trh_corner.get_y() - blh_corner.get_y()))
    {
        selected_side = 1;
    }
    if (2 * (trh_corner.get_x() - blh_corner.get_x()) >= (trh_corner.get_y() - blh_corner.get_y()))
    {
        selected_side = 0;
    }
    double min{};
    double max{};
    // select the parameters to generate the coordinate for the new rectangles, based on selected side
    if (selected_side == 0)
    {
        min = (3 * blh_corner.get_x() + trh_corner.get_x()) / 4;
        max = (3 * trh_corner.get_x() + blh_corner.get_x()) / 4;
    }
    if (selected_side == 1)
    {
        min = (3 * blh_corner.get_y() + trh_corner.get_y()) / 4;
        max = (3 * trh_corner.get_y() + blh_corner.get_y()) / 4;
    }
    // Generate the Position where the selected_side is going to be splitted
    double rnum = engine.uniform(min, max);
    // Split the Rectangle
    if (selected_side == 0)
    {
        Position first_top_corner{rnum, trh_corner.get_y()};
        Rectangle res1{blh_corner, first_top_corner};
        Position second_bottom_corner{rnum, blh_corner.get_y()};
        Rectangle res2{second_bottom_corner, trh_corner};
        result.push_back(res1);
        result.push_back(res2);
    }
    if (selected_side == 1)
    {
        Position first_top_corner{trh_corner.get_x(), rnum};
        Rectangle res1{blh_corner, first_top_corner};
        Position second_bottom_corner{blh_corner.get_x(), rnum};
        Rectangle res2{second_bottom_corner, trh_corner};
        result.push_back(res1);
        result.push_back(res2);
    }
    return result;
}
///////////////// DIVIDE  /////////////////
std::vector<Rectangle> Rectangle::divide(int areas_to_divide, Random& engine)
{
    // Construct result vector with the object as the only member
    std::vector<Rectangle> result = {{blh_corner, trh_corner}};
    if (areas_to_divide == 1)
    {
        // Return the object rectangle in a vector
        return result;
    }
    // Divide the rectangle in n random part
    for (int i = 1; i < areas_to_divide; ++i)
    {
        // Select a random member of the result vector
        // Vector that store the square of the areas of the rectangle at the same index in result
        std::vector<double> Area2;
        Area2.clear();
        // Fill the Area2 vector
        for (auto a : result)
        {
            Area2.push_back(a.get_area() * a.get_area());
        }
        // Select the cluster to divide using the Area2 vector as weight
        int index = engine.discrete(Area2);
        auto it = result.begin();
        it += index;
        // Split the selected Rectangle in 2 rectangle
        std::vector<Rectangle> prev = it->split(engine);
        // Remove the splitted rectangle from vector
        result.erase(it);
        // Add the 2 resulting rectangle to result vector
        for (auto a : prev)
        {
            result.push_back(a);
        }
    }
    return result;
}
} // namespace smooth_sim
