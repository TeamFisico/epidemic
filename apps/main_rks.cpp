
#include "SEIRode.hpp"

////////STANDARD ///////
#include <fstream>
#include <iomanip>
#include <iostream>


/////////  ROOT  ///////
#include "TF1.h"
#include "TApplication.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TMultiGraph.h"

int main (int argc, char* argv[])
{
    using namespace seir;

    State start{100000,1,1,0};
    ode conditions{100000,start,0.9,0.5,0.2,1,100};

    Simulation mysimulation;

    simulation(conditions,mysimulation);
     std::ofstream out{"output.txt"};

  std::cout << "┌─────┬───────────────┬───────────────┬───────────────┬───────────────┐" << std::endl;
  std::cout << "│ Day │       S       │       E       │       I       │       R       │" << std::endl;
  std::cout << "├─────├───────────────├───────────────├───────────────├───────────────├" << std::endl;
  int t1 = 0;
  for (auto& a: mysimulation) {
    out << "S = " << a.S << " E = " << a.E << " I = " << a.I << " R = " << a.R << std::endl;

    std::cout << std::setprecision(9) << std::left << "│" << std::setw(5) << t1 << "│" << std::setw(15) << a.S << "│" << std::setw(15) << a.E << "│" << std::setw(15) << a.I << "│" << std::setw(15) << a.R << "│" << std::endl;
    t1++;
  }
  std::cout << "└─────┴───────────────┴───────────────┴───────────────┴───────────────┘" << std::endl;

    //ROOT CODE
    TApplication app("app", &argc, argv);


    auto c0 = new TCanvas("c0", "Epidemic evolution");
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

    int t = 0;
    for(auto a: mysimulation){
        gS->SetPoint(t,t, a.S);
        gE->SetPoint(t,t, a.E);
        gI->SetPoint(t,t, a.I);
        gR->SetPoint(t,t, a.R);
        t++;
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