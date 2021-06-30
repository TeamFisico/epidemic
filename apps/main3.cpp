#include "simulation.hpp"
#include <fstream>
#include <iostream>
#include <string>
////////// ROOT ////////
#include "TApplication.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TRootCanvas.h"

using namespace smooth_simulation;

std::string st(Person p)
{
    switch (p.current_status())
    {
    case Status::Exposed:
        return {"Exposed"};
    case Status::Infected:
        return{"Infected"};
    case Status::Susceptible:
        return {"Susceptible"};
    case Status::Dead:
         return {"Dead"};
    }
}

int main(int argc,char* argv[])
{
    using namespace smooth_simulation;
    using namespace std::literals::chrono_literals;

    unsigned int nE = 40;
    unsigned int nI = 100;
    unsigned int nR = 2;
    unsigned int nS = POPULATION_SIZE - nE - nI - nR;

    Simulation sim{800, 1.0, 0.025, 0.15, 0.02, 0.0005, {nS,nE,nI,nR}};

    std::ofstream o {"prova.txt"};

   std::vector<Data> result;
    double move_duration = 0;
    double spread_duration = 0;
    sim.update_data();
    result.push_back(sim.get_simulation_data());
    print_data(sim.get_simulation_data(),o);
    for (int j = 1; j < 50; ++j)
    {
        move_duration = 0;
        spread_duration = 0;
        for (int i = 0; i < UPDATE_ZONES_INTERVAL; ++i)
        {
            auto start1 = std::chrono::high_resolution_clock::now();
            sim.move();
            auto end1 = std::chrono::high_resolution_clock::now();
            auto start2 = std::chrono::high_resolution_clock::now();
            sim.spread();
            auto end2 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> mov = end1 - start1;
            std::chrono::duration<float> spr = end2 - start2;
            move_duration += mov.count();
            spread_duration += spr.count();
        }
        sim.update_data();
        sim.update_zones();
        print_data(sim.get_simulation_data(),o);
        result.push_back(sim.get_simulation_data());
        std::cout << "Block["<<j<<"]"<<std::endl;
        std::cout<<"Move -->"<<move_duration<<std::endl;
        std::cout << "Spread -- > "<<spread_duration<<std::endl;

    }

    // ROOT CODE
    TApplication app("app", &argc, argv);

    auto c0 = new TCanvas("c0", "Evoluzione");
    auto mg = new TMultiGraph();
    auto gS = new TGraph();
    auto gE = new TGraph();
    auto gI = new TGraph();
    auto gR = new TGraph();
    auto gD = new TGraph();

    gS->SetLineColor(kBlue);
    gE->SetLineColor(kOrange);
    gI->SetLineColor(kGreen);
    gR->SetLineColor(kRed);
    gD->SetLineColor(kBlack);
    mg->SetTitle("Evolution; steps; number of people");

    int t2 = 0;
    for (auto &a : result)
    {
        gS->SetPoint(t2, t2, a.S);
        gE->SetPoint(t2, t2, a.E);
        gI->SetPoint(t2, t2, a.I);
        gR->SetPoint(t2, t2, a.R);
        gD->SetPoint(t2,t2,a.D);
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
    mg->Add(gD);
    gD->SetTitle("D");

    mg->Draw("AL");
    c0->BuildLegend();

    c0->Modified();
    c0->Update();
    TRootCanvas *rc = (TRootCanvas *)c0->GetCanvasImp();
    rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
    app.Run();



}