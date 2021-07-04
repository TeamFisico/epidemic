#include "seir2.hpp"
#include <iostream>
#include <stdexcept>

namespace seir
{

void error(std::string s)
{
    throw std::runtime_error(s);
}

///////////////////////////////////////////////////////////////
//                     CONSTRUCTOR                           //
///////////////////////////////////////////////////////////////
ode::ode(int population, int time, State initial_state, double beta, double alpha, double gamma)
    : N{population},
      t{time},
      S_0{initial_state},
      beta{beta},
      alpha{alpha},
      gamma{gamma}
{
    is_valid(*this);
}
///////////////////////////////////////////////////////////////
//  DEFAULT CONDITIONS : POPULATION: 100000,TIME: 80 DAYS, 1 //
//   INFECTED, BETA = 0.7, ALPHA = 0.5 , GAMMA = 0.2       //
///////////////////////////////////////////////////////////////
const ode& default_ode()
{
    State df{99999, 0, 1, 0};
    static ode def{100000, 80, df, 0.7, 0.5, 0.2};
    return def;
}
///////////////////////////////////////////////////////////////
//                   DEFAULT CONSTRUCTOR                     //
///////////////////////////////////////////////////////////////
ode::ode()
    // default constructor:CHECK IF IT MAKES SENSE
    : N{default_ode().N},
      t{default_ode().t},
      S_0{default_ode().S_0},
      beta{default_ode().beta},
      alpha{default_ode().alpha},
      gamma{default_ode().gamma}
{
}

// argument checking in object construction
bool ode::is_valid(ode obj)
{
    if (obj.t <= 0)
    {
        error("The simulation duration have a non-positive value!");
        return false;
    }
    else if (obj.t < 10)
    {
        error("The simulation period is too small for the simulation to be "
              "accurate!");
        return false;
    }
    if (obj.N <= 0)
    {
        error("The population has a non-positive value!");
        return false;
    }
    else if (obj.N < 100)
    {
        error("The population sample is too small for the simulation to be "
              "accurate!");
        return false;
    }
    if (obj.S_0.E == 0 && obj.S_0.I == 0)
    { // 0 people with, at least, virus in latent phase
        error("The disease will not spread if nobody has taken the virus in "
              "the initial state!");
        return false;
    }
    if (obj.beta <= 0.0 || obj.gamma <= 0.0 || obj.alpha <= 0.0)
    {
        error("The parameters can't be negative or 0!");
        return false;
        // add some more constraints
    }

    if (obj.S_0.S + obj.S_0.E + obj.S_0.I + obj.S_0.R != obj.N)
    {
        error("The sum of susceptibles, latent, infected and removed individuals "
              "must equal the total population!");
    }

    return true;
}
////////////////////////////////
//     RUNGE KUTTA METHOD     //
////////////////////////////////
// 4TH order method with an error of O(h^5)
State ode::RungeKuttaSolver(const State& oldState)
// method solving solving SEIR system of differential equations
// using Runge Kutta 4th order method
{
    State newState;
    State updatedState;

    // 1st order term in formula for S,E,I,R
    // k1 = f(S,E,I,R)
    std::vector<double> k1(4);
    k1[0] = time_step * oldState.dS_dt(beta, N);
    k1[1] = time_step * oldState.dE_dt(beta, alpha, N);
    k1[2] = time_step * oldState.dI_dt(alpha, gamma);
    k1[3] = time_step * oldState.dR_dt(gamma);

    // update the state by half a time_step
    updatedState.S = oldState.S + k1[0] / 2.0;
    updatedState.E = oldState.E + k1[1] / 2.0;
    updatedState.I = oldState.I + k1[2] / 2.0;
    updatedState.R = oldState.R + k1[3] / 2.0;

    // 2nd order term in formula for S,E,I,R
    // k2 = f(S+h/2*k1 ,E+h/2*k1 ,I+h/2*k1 ,R+h/2*k1)
    std::vector<double> k2(4);
    k2[0] = time_step * updatedState.dS_dt(beta, N);
    k2[1] = time_step * updatedState.dE_dt(beta, alpha, N);
    k2[2] = time_step * updatedState.dI_dt(alpha, gamma);
    k2[3] = time_step * updatedState.dR_dt(gamma);

    // update the state by half a time_step
    updatedState.S = oldState.S + k2[0] / 2.0;
    updatedState.E = oldState.E + k2[1] / 2.0;
    updatedState.I = oldState.I + k2[2] / 2.0;
    updatedState.R = oldState.R + k2[3] / 2.0;

    // 3rd order term in formula for S,E,I,R
    // k3 = f(S+h/2*k ,E+h/2*k2 ,I+h/2*k2 ,R+h/2*k2)
    std::vector<double> k3(4);
    k3[0] = time_step * updatedState.dS_dt(beta, N);
    k3[1] = time_step * updatedState.dE_dt(beta, alpha, N);
    k3[2] = time_step * updatedState.dI_dt(alpha, gamma);
    k3[3] = time_step * updatedState.dR_dt(gamma);

    // update the state by the whole time_step
    updatedState.S = oldState.S + k3[0];
    updatedState.E = oldState.E + k3[1];
    updatedState.I = oldState.I + k3[2];
    updatedState.R = oldState.R + k3[3];

    // 4th order term in formula for S,E,I,R
    // k4 = f(S+h ,E+h ,I+h ,R+h)
    std::vector<double> k4(4);
    k4[0] = time_step * updatedState.dS_dt(beta, N);
    k4[1] = time_step * updatedState.dE_dt(beta, alpha, N);
    k4[2] = time_step * updatedState.dI_dt(alpha, gamma);
    k4[3] = time_step * updatedState.dR_dt(gamma);

    // calculating the values of S,E,I,R for the new state
    // yn+1= yn + 1/6(k1+2k2+2k3+k4)
    newState.S = oldState.S + (k1[0] + 2.0 * k2[0] + 2.0 * k3[0] + k4[0]) / 6.0;
    newState.E = oldState.E + (k1[1] + 2.0 * k2[1] + 2.0 * k3[1] + k4[1]) / 6.0;
    newState.I = oldState.I + (k1[2] + 2.0 * k2[2] + 2.0 * k3[2] + k4[2]) / 6.0;
    newState.R = oldState.R + (k1[3] + 2.0 * k2[3] + 2.0 * k3[3] + k4[3]) / 6.0;

    // if one of SEIR is <0 set it to 0
    return newState;
}

////////////////////////////////
//     SIMULATION FUNCTION    //
////////////////////////////////
void simulation(ode sim, Simulation& result)
{
    result.clear(); // empty the vector
    State current_state = sim.initial_state();
    State new_state;

    result.push_back(current_state);
    // sistema stop controlla
    for (double t = 0.0; t < sim.simulation_time() - 1; t += time_step)
    {
        new_state = sim.RungeKuttaSolver(current_state); // calculate new state with Runge Kutta
        result.push_back(new_state);                     // push into the Simulation
        current_state = new_state;
    }
}

} // namespace seir
