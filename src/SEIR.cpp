
#include "../include/epidemic1/SEIR.hpp"

inline sim::sim(const State &initial_state, double a, double b, double c)
        : state0{initial_state}, alpha{a}, beta{b}, gamma{c}
{
}

inline std::vector<State> sim::generate_all_points(int time_in_days) {
    std::vector<State> result{state0};
    for(int i = 1; i < time_in_days; ++i)
    {
        State prev = result.back(); //previous State
        State new_state{0,0,0,0,0};
        new_state.S = prev.S - (beta*(prev.I)*(prev.S))/(prev.N);
        new_state.E = prev.E + (beta*(prev.I)*(prev.S))/(prev.N) - alpha*(prev.E);
        new_state.I = prev.I + alpha*(prev.E) - gamma*(prev.I);
        new_state.R = prev.R + gamma*(prev.I);
        new_state.N = prev.N;
        result.push_back(new_state);
    }
    return result;
}



