#include "mobility_model.hpp"
#include <cassert>
#include <random>
#include <numeric>

using namespace sim;

// Constructor
mobility_model::mobility_model(Person *person, Location *target_location, double speed, int stay,
                               double home_probability, bool at_home)
    : person{person}, target_location{target_location}, speed{speed}, stay{stay},
      home_probability{home_probability}, at_home{at_home}
{
}

// Default Constructor WIP
/*mobility_model::mobility_model()
{
}*/


void mobility_model::next_location()
{
    std::vector<double> inverse_distances; //vector where we store the inverse_distance elevated to alpha of the pointed location with the same index in Path
    inverse_distances.clear();
    for(auto& a: Path){ //fill the inverse_distances vector
        double dist = 1/pow(a->get_pos().distance_to(*(person->get_pos())),alpha);
        inverse_distances.push_back(dist);
    }
    std::vector<double> probabilities; // vector where we store the probabilities of the same index in Path calculated with LATP algorithm
    probabilities.clear();
    double denom = std::accumulate(inverse_distances.begin(), inverse_distances.end(), 0); // calculate the denominator for the probabilities formula of the LATP algorithm
    for (auto& a : inverse_distances){ // fill the probabilities vector
        probabilities.push_back(a/denom);
    }
    // select next_location to visit based on the probabilities vector
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> rand(0,1);
    double rnum = rand(gen);
    int index_result{};
    for (int i = 0; i < probabilities.size(); ++i)
    {
        if(rnum <= probabilities.operator[](i)){
            index_result = i;
            break;
        }
        rnum -= probabilities.operator[](i);
    }
    target_location = Path.operator[](index_result); //set the target Location to the Location found using LATP algorithm
    // remove the selected location pointer from the Path vector
    auto it = Path.begin(); //generate an iterator to the star of the Path vector
    it = it + index_result; //make sure the iterator point to the selected location
    Path.erase(it); //erase the selected Location from the Path vector
}

void mobility_model::move()
{
    speed = rand_speed(0, 1);
}

double sim::rand_speed(double min, double max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> rand(min, max);
    return rand(gen);
}

int sim::rand_stay(int min, int max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> rand(min, max);
    return rand(gen);
}

