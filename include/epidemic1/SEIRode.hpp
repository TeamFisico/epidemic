#ifndef SEIRODE_HPP
#define SEIRODE_HPP

#include <iostream>
#include <vector>

namespace seir {

    struct State {
        double S;
        double E;
        double I;
        double R;

        double dS_dt (double beta, unsigned int N) const
        {     return -(beta/N)*S*I;       }
        double dE_dt (double beta,double alpha, unsigned int N) const
        {     return +beta*S*I/N-alpha*E; }
        double dI_dt (double alpha, double gamma, unsigned int N) const
        {     return alpha*E-gamma*I;     }
        double dR_dt (double gamma, unsigned int N) const
        {     return +gamma*I;            }
    };


//class to solve SEIR characteristic system of differential equations
    class ode {
    private:
        State S_0;
        double beta;
        double alpha;
        double gamma;
        unsigned int N; //population
        unsigned int t; //time of the simulation(days)
        double step;
    public:
        //constructor of the class
        ode(unsigned int time, unsigned int population, State S0, double beta, double alpha, double gamma,
            double step_time);
        ode(); //default constructor

        static bool is_valid (ode obj) ; //check whether the object is valid or not
        unsigned int population() const { return N; }
        unsigned int simulation_time() const { return t; }
        double time_step() const { return step; }
        State initial_state() const { return S_0; }

        State RungeKuttaSolver(const State& oldState); //returns the new state calculated with RungeKutta 4th order method

    };

    //The simulation output is a vector containing the States
    using Simulation = std::vector<State>;

    void simulation(ode sim,Simulation& result);

}


#endif





