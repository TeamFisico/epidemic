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

double Rectangle::get_area()
{
    return (trh_corner.get_x() - blh_corner.get_x()) * (trh_corner.get_y() - blh_corner.get_y());
}

///////////////// SPLIT /////////////////
std::vector<Rectangle> Rectangle::split(Random& engine)
{
    std::vector<Rectangle> result;
    result.clear();
    int selected_side = engine.int_uniform(
        0, 1); // Select the random side to divide, if 0 select the x-axis side, if 1 select the y-axis side
    // make sure that rectangle that have a side at least three times the other have the longest_side as the selected
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
    if (selected_side == 0)
    { // select the parameter to generate the coordinate for the new rectangles
        min = (3 * blh_corner.get_x() + trh_corner.get_x()) / 4;
        max = (3 * trh_corner.get_x() + blh_corner.get_x()) / 4;
    }
    if (selected_side == 1)
    {
        min = (3 * blh_corner.get_y() + trh_corner.get_y()) / 4;
        max = (3 * trh_corner.get_y() + blh_corner.get_y()) / 4;
    }
    double rnum = engine.uniform(min, max);
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

// TODO Add error checking to the following function when n < 1, and change the random selection of what rectangle
// divide to an algorithm that is based on the area,
// as the current method favor a too uneven division.
///////////////// DIVIDE  /////////////////
std::vector<Rectangle> Rectangle::divide(int areas_to_divide)
{
    Random wrld_partition_engine{};
    std::vector<Rectangle> result = {
        {blh_corner, trh_corner}}; // construct result vector with the object as the only member
    if (areas_to_divide == 1)
    { // return the object rectangle in a vector
        return result;
    }
    for (int i = 1; i < areas_to_divide; ++i)
    { // Divide the rectangle in n random part
        // select a random member of the result vector
        std::vector<double> Area2; // vector that store the areas of the rectangle at the same index in result
        Area2.clear();
        for (auto a : result)
        { // fill the Area vector
            Area2.push_back(a.get_area() * a.get_area());
        }
        //        double total_area = std::accumulate(Area2.begin(), Area2.end(), 0.);
        int index = wrld_partition_engine.discrete(Area2);
        auto it = result.begin();
        it += index;
        // Split the selected Rectangle in 2 rectangle
        std::vector<Rectangle> prev = it->split(wrld_partition_engine);
        result.erase(it);   // remove the splitted rectangle from vector
        for (auto a : prev) // Add the 2 resulting rectangle to result vector
        {
            result.push_back(a);
        }
    }
    return result;
}
} // namespace smooth_sim
