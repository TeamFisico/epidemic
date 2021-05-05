#include <fstream>
#include <iostream>
#include <iomanip>
#include "epidemic1/SEIR.hpp"

#include <TApplication.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TCanvas.h>
#include <TRootCanvas.h>

int main (int argc, char* argv[])
{

    State state0{10000,10,2,0,10012};
    sim ode{state0,0.5,0.9,0.2};
    std::vector<State> result = ode.generate_all_points(150);


    std::ofstream out {"output.txt"};
    std::cout << "┌─────┬───────────────┬───────────────┬───────────────┬───────────────┐" << std::endl;
    std::cout << "│ Day │       S       │       E       │       I       │       R       │" << std::endl;
    std::cout << "├─────├───────────────├───────────────├───────────────├───────────────├" << std::endl;
    int t1 = 0;
    for (auto& a: result) {
      out << "S = " << a.S << " E = " << a.E << " I = " << a.I << " R = " << a.R << std::endl;

      std::cout << std::setprecision(9) << std::left << "│" << std::setw(5) << t1 << "│" << std::setw(15) << a.S << "│" << std::setw(15) << a.E << "│" << std::setw(15) << a.I << "│" << std::setw(15) << a.R << "│" << std::endl;
      t1++;
    }
    std::cout << "└─────┴───────────────┴───────────────┴───────────────┴───────────────┘" << std::endl;

    //ROOT CODE

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
    for(auto& a: result){
        gS->SetPoint(t2,t2, a.S);
        gE->SetPoint(t2,t2, a.E);
        gI->SetPoint(t2,t2, a.I);
        gR->SetPoint(t2,t2, a.R);
        t2++;
    }

    mg->Add(gS); gS->SetTitle("S");
    mg->Add(gE); gE->SetTitle("E");
    mg->Add(gI); gI->SetTitle("I");
    mg->Add(gR); gR->SetTitle("R");

    mg->Draw("AL");
    c0->BuildLegend();

    c0->Modified();
    c0->Update();
//    TRootCanvas *rc = (TRootCanvas *)c0->GetCanvasImp();
//    rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
    app.Run();

    return 0;
}