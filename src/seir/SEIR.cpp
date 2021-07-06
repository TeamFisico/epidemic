#include "seir1.hpp"

using namespace Euler;

// Constructor
sim::sim(const State& initial_state, double alpha, double beta, double gamma)
    : state0{initial_state},
      alpha{alpha},
      beta{beta},
      gamma{gamma}
{
}

// default values
const sim& default_sim()
{
    State df{99999, 0, 1, 0, 100000};
    static sim def{df, 0.5, 0.7, 0.2};
    return def;
}

// default constructor
sim::sim()
    : state0{default_sim().state0},
      alpha{default_sim().alpha},
      beta{default_sim().beta},
      gamma{default_sim().gamma}
{
}

std::vector<State> sim::generate_all_points(int time_in_days)
{
    std::vector<State> result{state0};
    for (int i = 1; i < time_in_days; ++i)
    {
        State prev = result.back(); // previous State
        State new_state{};
        new_state.S = prev.S - (beta * (prev.I) * (prev.S)) / (prev.N);
        new_state.E = prev.E + (beta * (prev.I) * (prev.S)) / (prev.N) - alpha * (prev.E);
        new_state.I = prev.I + alpha * (prev.E) - gamma * (prev.I);
        new_state.R = prev.R + gamma * (prev.I);
        new_state.N = prev.N;
        result.push_back(new_state);
    }
    return result;
}

bool sim::is_valid()
{
}
