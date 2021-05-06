#include <epidemic1/SEIRode.hpp>
#include <stdexcept>
#include <iostream>

namespace seir {

///////////////////////////////////////////////////////////////
//                     CONSTRUCTOR                           //
///////////////////////////////////////////////////////////////
    ode::ode(int population, int time, double step_time,State initial_state,double beta,
             double alpha, double gamma)
            : N{population},t{time}, step{step_time},S_0{initial_state},
              beta{beta}, alpha{alpha}, gamma{gamma}
    {
        if (!is_valid(*this)) throw std::runtime_error("The constructed object is invalid");
    }
///////////////////////////////////////////////////////////////
//  DEFAULT CONDITIONS : POPULATION: 100000,TIME: 80 DAYS, 1 //
//   INFECTED, BETA = 0.7, ALPHA = 0.5 , GAMMA = 0.2       //
///////////////////////////////////////////////////////////////
   const ode& default_ode()
   {
        State df {99999,0,1,0};
        static ode def{100000,80,1,df,0.7,0.5,0.2};
        return def;
   }
///////////////////////////////////////////////////////////////
//                   DEFAULT CONSTRUCTOR                     //
///////////////////////////////////////////////////////////////
   ode::ode()
   //default constructor:sample of 1000000 running for 150 days ... CHECK IF IT MAKES SENSE
           : t{default_ode().t}, N{default_ode().N}, S_0{default_ode().S_0},
          beta{default_ode().beta}, alpha{default_ode().alpha}, gamma{default_ode().gamma}, step{default_ode().step}
   {
   }



    //argument checking in object construction
    bool ode::is_valid(ode obj) {
        if (obj.t <= 0) {
            std::cerr << "The simulation duration have a non-positive value!\n";
            return false;
        }
        else if (obj.t < 10) {
            std::cerr << "The simulation period is too small for the simulation to be accurate!\n";
            return false;
        }
        if (obj.N <= 0) {
            std::cerr << "The population have a non-positive value!\n";
            return false;
        }
        else if (obj.N < 100) {
            std::cerr << "The population sample is too small for the simulation to be accurate!\n";
            return false;
        }
        if (obj.S_0.E == 0 && obj.S_0.I == 0) { // 0 people with, at least, virus in latent phase
            std::cerr << "The disease will not spread if nobody has taken the virus in the initial state!\n";
            return false;
        }
        if (obj.beta <= 0.0 || obj.gamma <= 0.0 || obj.alpha <= 0.0){
            std::cerr << "The parameters can't be negative or 0!\n";
            return false;
            //add some more constraints
        }
        if (obj.step <= 0){
            std::cerr << "In order for the method to be efficient, the time step must not be too high(>1)!\n";
            return false;
        } else if (obj.step >1) {
            std::cerr << "The time step can't be negative(or 0)";
            return false;
        }

        return true;
    }
////////////////////////////////
//     RUNGE KUTTA METHOD     //
////////////////////////////////
// 4TH order method with an error of O(h^5)
    State ode::RungeKuttaSolver(const State &oldState)
    //method solving solving SEIR system of differential equations
    // using Runge Kutta 4th order method
    {

        State newState;
        State updatedState;


        //1st order term in formula for S,E,I,R
        //k1 = f(S,E,I,R)
        std::vector<double>k1 (4);
        k1[0] = step*oldState.dS_dt(beta,N);
        k1[1] = step*oldState.dE_dt(beta,alpha,N);
        k1[2] = step*oldState.dI_dt(alpha,gamma);
        k1[3] = step*oldState.dR_dt(gamma);

        //update the state by half a step
        updatedState.S = oldState.S + k1[0]/2.0;
        updatedState.E = oldState.E + k1[1]/2.0;
        updatedState.I = oldState.I + k1[2]/2.0;
        updatedState.R = oldState.R + k1[3]/2.0;

        //2nd order term in formula for S,E,I,R
        //k2 = f(S+h/2*k1 ,E+h/2*k1 ,I+h/2*k1 ,R+h/2*k1)
        std::vector<double>k2 (4);
        k2[0] = step*updatedState.dS_dt(beta,N);
        k2[1] = step*updatedState.dE_dt(beta,alpha,N);
        k2[2] = step*updatedState.dI_dt(alpha,gamma);
        k2[3] = step*updatedState.dR_dt(gamma);

        //update the state by half a step
        updatedState.S = oldState.S + k2[0]/2.0;
        updatedState.E = oldState.E + k2[1]/2.0;
        updatedState.I = oldState.I + k2[2]/2.0;
        updatedState.R = oldState.R + k2[3]/2.0;

        //3rd order term in formula for S,E,I,R
        //k3 = f(S+h/2*k ,E+h/2*k2 ,I+h/2*k2 ,R+h/2*k2)
        std::vector<double>k3 (4);
        k3[0] = step*updatedState.dS_dt(beta,N);
        k3[1] = step*updatedState.dE_dt(beta,alpha,N);
        k3[2] = step*updatedState.dI_dt(alpha,gamma);
        k3[3] = step*updatedState.dR_dt(gamma);

        //update the state by the whole step
        updatedState.S = oldState.S + k3[0];
        updatedState.E = oldState.E + k3[1];
        updatedState.I = oldState.I + k3[2];
        updatedState.R = oldState.R + k3[3];

        //4th order term in formula for S,E,I,R
        //k4 = f(S+h ,E+h ,I+h ,R+h)
        std::vector<double>k4 (4);
        k4[0] = step*updatedState.dS_dt(beta,N);
        k4[1] = step*updatedState.dE_dt(beta,alpha,N);
        k4[2] = step*updatedState.dI_dt(alpha,gamma);
        k4[3] = step*updatedState.dR_dt(gamma);

        //calculating the values of S,E,I,R for the new state
        // yn+1= yn + 1/6(k1+2k2+2k3+k4)
        newState.S = oldState.S + (k1[0] + 2.0*k2[0] + 2.0*k3[0] + k4[0])/6.0;
        newState.E = oldState.E + (k1[1] + 2.0*k2[1] + 2.0*k3[1] + k4[1])/6.0;
        newState.I = oldState.I + (k1[2] + 2.0*k2[2] + 2.0*k3[2] + k4[2])/6.0;
        newState.R = oldState.R + (k1[3] + 2.0*k2[3] + 2.0*k3[3] + k4[3])/6.0;

        //if one of SEIR is <0 set it to 0
        return newState;

    }


////////////////////////////////
//     SIMULATION FUNCTION    //
////////////////////////////////
    void simulation(ode sim,Simulation& result)
    {

        result.clear(); //empty the vector
        State current_state = sim.initial_state();
        State new_state;

        result.push_back(current_state);
        //sistema stop controlla
        for (double t = 0.0; t < sim.simulation_time()+1; t+= sim.time_step())
        {
            new_state = sim.RungeKuttaSolver(current_state); //calculate new state with Runge Kutta
            result.push_back(new_state);   //push into the Simulation
            current_state = new_state;
        }

        //cast to int to calculaate the bins
        int  bins =(int)(sim.simulation_time()/sim.time_step());

        std::cout << "bins : " << bins << std::endl << " Simulation vector size: " << result.size() << std::endl;
//        if (result.size() != bins) throw std::runtime_error(
//                    "Number of time bins and number of actual simulation bins do not match!");
    }


}











