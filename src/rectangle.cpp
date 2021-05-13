#include "rectangle.hpp"
#include <random>

using namespace sim;

Rectangle::Rectangle(Position &blh_corner, Position &trh_corner)
: blh_corner{blh_corner}, trh_corner{trh_corner}
{
}

std::vector<Rectangle> Rectangle::Split()
{
    std::vector<Rectangle> result;
    result.clear();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution side(0,1); //decide the random side to divide
    int selected_side = side(gen); // if 0 select the x-axis side, if 1 select the y-axis side
    int min{};
    int max{};
    if(selected_side == 0){ //select the parameter to generate the coordinate for the new rectangles
        min = blh_corner.get_x() + blh_corner.get_x()/6;
        max = trh_corner.get_x() - trh_corner.get_x()/6;
    }
    if(selected_side == 1){
        min = blh_corner.get_y() + blh_corner.get_y()/6;
        max = trh_corner.get_y() - trh_corner.get_y()/6;
    }
    std::uniform_real_distribution<> rand(min, max);
    double rnum = rand(gen);
    if(selected_side == 0){
        Position first_top_corner{rnum, trh_corner.get_y()};
        Rectangle res1{blh_corner, first_top_corner};
        Position second_bottom_corner{rnum, blh_corner.get_y()};
        Rectangle res2{second_bottom_corner, trh_corner};
        result.push_back(res1);
        result.push_back(res2);
    }
    if(selected_side == 1){
        Position first_top_corner{trh_corner.get_x(), rnum};
        Rectangle res1{blh_corner, first_top_corner};
        Position second_bottom_corner{blh_corner.get_x(), rnum};
        Rectangle res2{second_bottom_corner, trh_corner};
        result.push_back(res1);
        result.push_back(res2);
    }
    return result;
}

//TODO Add error checking to the following function when n < 1
std::vector<Rectangle> Rectangle::Divide(int n)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<Rectangle> result = {{blh_corner,trh_corner}}; //construct result vector with the object as the only member
    if(n == 1){ //return the object rectangle in a vector
        return result;
    }
    for(int i = 1; i < n; ++i){ //Divide the rectangle in n random part
        //select a random member of the result vector
        int last_index = result.size() - 1;
        std::uniform_int_distribution rand(0, last_index);
        int index = rand(gen);
        auto it = result.begin();
        it += index;
        // Split the selected Rectangle in 2 rectangle
        std::vector<Rectangle> prev = it->Split();
        result.erase(it); //remove the splitted rectangle from vector
        for (auto a: prev){ //Add the 2 resulting rectangle to result vector
            result.push_back(a);
        }
    }
    return result;
}

Rectangle * sim::rand_rec(std::vector<Rectangle *> list)
{
    int last_index = list.size() - 1;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution index(0,last_index); //decide the random index
    return list.operator[](index(gen));
}
