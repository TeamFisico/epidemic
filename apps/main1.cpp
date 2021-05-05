#include <fstream>
#include <iostream>
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

    for (auto& a: result) {

        out << "S = " << a.S << " E = " << a.E << " I = " << a.I << " R = " << a.R << std::endl;

    }
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

    int t = 0;
    for(auto a: result){
        gS->AddPoint(t, a.S);
        gE->AddPoint(t, a.E);
        gI->AddPoint(t, a.I);
        gR->AddPoint(t, a.R);
        t++;
    }

    mg->Add(gS); gS->SetTitle("S");
    mg->Add(gE); gE->SetTitle("E");
    mg->Add(gI); gI->SetTitle("I");
    mg->Add(gR); gR->SetTitle("R");

    mg->Draw();

    c0->Modified();
    c0->Update();
    TRootCanvas *rc = (TRootCanvas *)c0->GetCanvasImp();
    rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
    app.Run();

    return 0;
}