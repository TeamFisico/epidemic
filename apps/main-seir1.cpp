#include <TApplication.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TRootCanvas.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>

#include "seir1.hpp"

using namespace Euler;

int main(int argc, char* argv[])
{

    int pop{0};
    int sim_time{100};
    double param1{0.0}; // beta
    double param2{0.0}; // alpha
    double param3{0.0}; // gamma
    State state0{};
    sim ode;

    std::string answer;
    std::cout << "Do you want to set the simulation parameters yourself?" << std::endl;
    std::cout << "Type y or n." << std::endl;

    try
    {
        while (std::cin >> answer)
        {
            if (answer == "y" || answer == "yes" || answer == "Y")
            {
                std::cout << std::setw(15) << "Population : ";
                std::cin >> pop;
                std::cout << std::setw(15) << "Simulation time(days) : ";
                std::cin >> sim_time;
                std::cout << std::setw(15) << "Parameter beta : ";
                std::cin >> param1;
                std::cout << std::setw(15) << "Parameter alpha : ";
                std::cin >> param2;
                std::cout << std::setw(15) << "Parameter gamma : ";
                std::cin >> param3;
                std::cout << std::setw(15) << "Susceptible individuals : ";
                std::cin >> state0.S;
                std::cout << std::setw(15) << "Latent individuals : ";
                std::cin >> state0.E;
                std::cout << std::setw(15) << "Infected individuals : ";
                std::cin >> state0.I;
                std::cout << std::setw(15) << "Removed individuals : ";
                std::cin >> state0.R;
                ode = {state0, param2, param1, param3};
                // ode.isValid();
                if (pop != static_cast<int>(state0.S + state0.E + state0.I + state0.R))
                {
                    throw std::runtime_error{"ERROR: The sum of individuals is different from the popolation"};
                }
                else
                {
                    break;
                }
            }
            else if (answer == "n" || answer == "no" || answer == "N")
            {
                break;
            }
            else
            {
                std::cout << "ERROR: input not supported, Type y or n." << std::endl;
            }
        }
    }
    catch (std::runtime_error const& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    std::vector<State> result = ode.generate_all_points(sim_time);

    std::ofstream out{"output.txt"};
    std::cout << "┌─────┬───────────────┬───────────────┬───────────────┬────────"
                 "───────┐"
              << std::endl;
    std::cout << "│ Day │       S       │       E       │       I       │       "
                 "R       │"
              << std::endl;
    std::cout << "├─────├───────────────├───────────────├───────────────├────────"
                 "───────├"
              << std::endl;
    int t1 = 1;
    for (auto& a : result)
    {
        out << "S = " << a.S << " E = " << a.E << " I = " << a.I << " R = " << a.R << std::endl;

        std::cout << std::setprecision(9) << std::left << "│" << std::setw(5) << t1 << "│" << std::setw(15) << a.S
                  << "│" << std::setw(15) << a.E << "│" << std::setw(15) << a.I << "│" << std::setw(15) << a.R << "│"
                  << std::endl;
        t1++;
    }
    std::cout << "└─────┴───────────────┴───────────────┴───────────────┴────────"
                 "───────┘"
              << std::endl;

    // ROOT CODE

    TApplication app("app", &argc, argv);

    auto c0 = new TCanvas("c0", "Evoluzione");
    auto mg = new TMultiGraph();
    auto gS = new TGraph();
    auto gE = new TGraph();
    auto gI = new TGraph();
    auto gR = new TGraph();
    gS->SetLineColor(kBlue);
    gE->SetLineColor(kOrange);
    gI->SetLineColor(kGreen);
    gR->SetLineColor(kRed);
    mg->SetTitle("Evolution; time (days); number of people");

    int t2 = 0;
    for (auto& a : result)
    {
        gS->SetPoint(t2, t2, a.S);
        gE->SetPoint(t2, t2, a.E);
        gI->SetPoint(t2, t2, a.I);
        gR->SetPoint(t2, t2, a.R);
        t2++;
    }

    mg->Add(gS);
    gS->SetTitle("S");
    mg->Add(gE);
    gE->SetTitle("E");
    mg->Add(gI);
    gI->SetTitle("I");
    mg->Add(gR);
    gR->SetTitle("R");

    mg->Draw("AL");
    c0->BuildLegend();

    c0->Modified();
    c0->Update();
    TRootCanvas* rc = (TRootCanvas*)c0->GetCanvasImp();
    rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
    app.Run();

    return 0;
}
