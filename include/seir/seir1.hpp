#ifndef SEIR_HPP
#define SEIR_HPP

#include <vector>

namespace Euler
{

struct State
{
    double S;
    double E;
    double I;
    double R;
    int N;
};

class sim
{
  private:
    State state0; // initial State
    double alpha; // inverse of incubation period
    double beta;  // average number of people infected by an infected in a day
    double gamma; // probability of recovery(or death) of an infected
  public:
    sim(State const& initial_state, double a, double b, double c);
    sim();
    bool is_valid();
    std::vector<State> generate_all_points(int time_in_days);
};

} // namespace Euler

#endif