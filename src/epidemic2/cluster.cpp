#include "cluster.hpp"
#include <random>

using namespace sim;

Cluster::Cluster(int S, int E, int I, int R, int number_of_location, Rectangle Area, Color color, int cluster_index)
: Area{Area}, color{color}, cluster_index{cluster_index}
{
    int N = S + E + I + R;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution rand(1,5);  //number of people in a home
    Population.clear();
    Population.reserve(N); //reserve space for the population vector
    //generate the population vector; initialize all person as susceptible and at home
    for(int i = 0; i < N;){
        int home_pop = rand(gen); // number of people in the current home
        Location current_home = rand_loc(Area.get_blh_corner(), Area.get_trh_corner(), HOME_RADIUS); //TODO add a macro for HOME_RADIUS value
        for(int j = 0; j < home_pop && i < N; ++j){
            Population.emplace_back(State::S,current_home.get_pos(),State::S,current_home, cluster_index);
            ++i;
        }
    }
    //change person's condition and next condition to have the right number of States
    for(int i = 0; i < E + I + R; ++i){
        if(i < E){
            Population.operator[](i).set_conditions(State::E);
        }
        else if(i < E + I){
            Population.operator[](i).set_conditions(State::I);
        }
        else if(i < E + I + R){
            Population.operator[](i).set_conditions(State::R);
        }
    }
    //Determine the number of groups
    std::normal_distribution<double> rnum(number_of_location/8, number_of_location/32);
    int number_of_groups = nearbyint(rnum(gen));
    // Create a Vector which will have the partition of locations in group, every group has at least one location
    std::vector<int> loc_num(number_of_groups,1);
    //std::uniform_int_distribution<int> rand_index(0, number_of_groups - 1);
    int loc_left = number_of_location - number_of_groups;
    for(int i = 0; i < number_of_groups; ++i){
        if (loc_left == 0){
            i = number_of_groups; //end the loop
        }
        else if (loc_left == 1){
            ++loc_num.operator[](i);
            i = number_of_groups; //end the loop
        }
        else
        {
            std::uniform_int_distribution<> rand_num(1, nearbyint(loc_left / 2));
            int rnum = rand_num(gen);
            loc_num.operator[](i) += rnum;
            loc_left -= rnum;
        }
    }

    groups.clear();
    groups.reserve(number_of_groups);
    //fill the group vector
    for(int i = 0; i < number_of_groups; ++i){
        groups.emplace_back(loc_num.operator[](i), gen_group_center(loc_num.operator[](i)));
    }
}

std::vector<Location *> Cluster::Location_list()
{
    std::vector<Location *> result;
    result.clear();
    for (unsigned int i = 0; i < groups.size(); ++i)
    {
        std::vector<Location *> temporary = groups.operator[](i).Location_list();
        for (unsigned int j = 0; j < temporary.size(); ++j)
        {
            result.push_back(temporary.operator[](j));
        }
    }
    return result;
}

std::vector<Person *> Cluster::Person_list()
{
    std::vector<Person *> result;
    result.clear();
    for (unsigned int i = 0; i <= Population.size(); ++i)
    {
        result.push_back(&Population.operator[](i));
    }
    return result;
}

int Cluster::number_of_locations()
{
    int sum{};
    for (auto &a : groups)
    {
        sum += a.Location_list().size();
    }
    return sum;
}

int Cluster::number_of_people()
{
    return Population.size();
}

Position Cluster::gen_group_center(int num_of_loc)
{
    bool end_loop = false;
    Position new_center{};
    while(!end_loop){
        end_loop = true;
        new_center = rand_pos(Area.get_blh_corner(), Area.get_trh_corner()); //generate random center position
        for(auto& a: groups){ // check if this center is far enough from other groups center
            if(a.get_center().distance_to(new_center) <= (a.size() + num_of_loc)*TRANSMISSION_RANGE/10){
                end_loop = false;
                break;
            }
        }
    }
    return new_center;

}