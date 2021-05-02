
#include <epidemic1/SEIR.hpp>

inline sim::sim(const State &initial_state, double a, double b, double c)
        : state0{initial_state}, alpha{a}, beta{b}, gamma{c}
{
}

inline std::vector<State> sim::generate_all_points(int time_in_days) {
    std::vector<State> result{state0};
    auto it = result.begin(); //iterator pointed to the first element of the vector
    for(int i = 0; i < time_in_days; ++i)
    {
        State new_state{};
        new_state.S = it->S - (beta*(it->I)*(it->S)/(it->N));
        new_state.E = it->E + (beta*(it->I)*(it->S)/(it->N)) - alpha*(it->E);
        new_state.I = it->I + alpha*(it->E) - gamma*(it->I);
        new_state.R = it->R;
        result.push_back(new_state);
        ++it;
    }
    return result;
}



