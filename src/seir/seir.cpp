#include "seir.hpp"
#include <array>
#include <cassert>
#include <stdexcept>

void error(std::string s)
{
    throw std::runtime_error(s);
}

namespace seir
{

//////////////////////////////////////////////
///////        SEIR CONSTRUCTOR         //////
//////////////////////////////////////////////
SEIR::SEIR(int population, int time, State initial_state, double alpha, double beta, double gamma)
    : N{population},
      t{time},
      S_0{initial_state},
      alpha{alpha},
      beta{beta},
      gamma{gamma}
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////           PRIVATE METHODS           /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////// DETERMINE NEW STATE WITH EULER METHOD /////////////////
State SEIR::EulerSolver(const State& current_state)
{
    State new_state{};
    // Calculate the new state
    new_state.S = current_state.S + current_state.dS_dt(beta, N);
    new_state.E = current_state.E + current_state.dE_dt(beta, alpha, N);
    new_state.I = current_state.I + current_state.dI_dt(alpha, gamma);
    new_state.R = current_state.R + current_state.dR_dt(gamma);

    return new_state;
}
///////////////// DETERMINE NEW STATE WITH RUNGE KUTTA METHOD/////////////////
State SEIR::RungeKuttaSolver(State const& current_state)
{
    State newState;
    State updatedState;

    // 1st order term
    // k1 = f(S,E,I,R)
    std::array<double, 4> k1{};
    k1[0] = TIME_STEP * current_state.dS_dt(beta, N);
    k1[1] = TIME_STEP * current_state.dE_dt(beta, alpha, N);
    k1[2] = TIME_STEP * current_state.dI_dt(alpha, gamma);
    k1[3] = TIME_STEP * current_state.dR_dt(gamma);

    // Update the state by half a TIME_STEP
    updatedState.S = current_state.S + k1[0] / 2.0;
    updatedState.E = current_state.E + k1[1] / 2.0;
    updatedState.I = current_state.I + k1[2] / 2.0;
    updatedState.R = current_state.R + k1[3] / 2.0;

    // 2nd order term
    // k2 = f(S+h/2*k1 ,E+h/2*k1 ,I+h/2*k1 ,R+h/2*k1)
    std::array<double, 4> k2{};
    k2[0] = TIME_STEP * updatedState.dS_dt(beta, N);
    k2[1] = TIME_STEP * updatedState.dE_dt(beta, alpha, N);
    k2[2] = TIME_STEP * updatedState.dI_dt(alpha, gamma);
    k2[3] = TIME_STEP * updatedState.dR_dt(gamma);

    // Update the state by half a TIME_STEP
    updatedState.S = current_state.S + k2[0] / 2.0;
    updatedState.E = current_state.E + k2[1] / 2.0;
    updatedState.I = current_state.I + k2[2] / 2.0;
    updatedState.R = current_state.R + k2[3] / 2.0;

    // 3rd order term
    // k3 = f(S+h/2*k ,E+h/2*k2 ,I+h/2*k2 ,R+h/2*k2)
    std::array<double, 4> k3{};
    k3[0] = TIME_STEP * updatedState.dS_dt(beta, N);
    k3[1] = TIME_STEP * updatedState.dE_dt(beta, alpha, N);
    k3[2] = TIME_STEP * updatedState.dI_dt(alpha, gamma);
    k3[3] = TIME_STEP * updatedState.dR_dt(gamma);

    // Update the state by the whole TIME_STEP
    updatedState.S = current_state.S + k3[0];
    updatedState.E = current_state.E + k3[1];
    updatedState.I = current_state.I + k3[2];
    updatedState.R = current_state.R + k3[3];

    // 4th order term
    // k4 = f(S+h ,E+h ,I+h ,R+h)
    std::array<double, 4> k4{};
    k4[0] = TIME_STEP * updatedState.dS_dt(beta, N);
    k4[1] = TIME_STEP * updatedState.dE_dt(beta, alpha, N);
    k4[2] = TIME_STEP * updatedState.dI_dt(alpha, gamma);
    k4[3] = TIME_STEP * updatedState.dR_dt(gamma);

    // Calculating the values of S,E,I,R for the new state
    // y_{n+1}= y_n + 1/6(k1+2k2+2k3+k4)
    newState.S = current_state.S + (k1[0] + 2.0 * k2[0] + 2.0 * k3[0] + k4[0]) / 6.0;
    newState.E = current_state.E + (k1[1] + 2.0 * k2[1] + 2.0 * k3[1] + k4[1]) / 6.0;
    newState.I = current_state.I + (k1[2] + 2.0 * k2[2] + 2.0 * k3[2] + k4[2]) / 6.0;
    newState.R = current_state.R + (k1[3] + 2.0 * k2[3] + 2.0 * k3[3] + k4[3]) / 6.0;

    // Handle case when one variable is negative
    if (newState.S < 0) newState.S = 0;
    if (newState.E < 0) newState.E = 0;
    if (newState.I < 0) newState.I = 0;
    if (newState.R < 0) newState.R = 0;

    return newState;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////           PUBLIC METHODS            /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////// EVOLVE /////////////////
void SEIR::evolve(std::vector<State>& states, bool method)
{
    assert(states.empty());
    // Allocate the needed space
    states.reserve(t);

    State current_state = S_0;
    State new_state;

    // Set the initial conditions as the first state
    states.push_back(current_state);

    ///////// Euler method to solve SEIR equations /////////
    if (method == 0)
    {
        for (int i = 0; i < t; ++i)
        {
            new_state = EulerSolver(current_state);
            states.push_back(new_state);
            current_state = new_state;
        }
        return;
    }

    ///////// Runge Kutta method to solve SEIR equations /////////
    if (method == 1)
    {
        for (int time = 0; time < t; time += TIME_STEP)
        {
            new_state = RungeKuttaSolver(current_state);
            states.push_back(new_state);
            current_state = new_state;
        }
    }
}
///////////////// RATE OF CHANGE OF SUSCEPTIBLES /////////////////
double State::dS_dt(double beta, int N) const
{
    return -(beta / N) * S * I;
}
///////////////// RATE OF CHANGE OF EXPOSED /////////////////
double State::dE_dt(double beta, double alpha, int N) const
{
    return +beta * S * I / N - alpha * E;
}
///////////////// RATE OF CHANGE OF INFECTED /////////////////
double State::dI_dt(double alpha, double gamma) const
{
    return alpha * E - gamma * I;
}
///////////////// RATE OF CHANGE OF RECOVERED /////////////////
double State::dR_dt(double gamma) const
{
    return +gamma * I;
}
} // namespace seir
