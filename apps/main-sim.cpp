#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "lyra/lyra.hpp" // LYRA HEADER
#include "simulation.hpp"

/////////  ROOT HEADERS  ///////
#include "TApplication.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TRootCanvas.h"

int main(int argc, char *argv[])
{
    using namespace smooth_sim;

    /*    auto start = std::chrono::high_resolution_clock::now();

        Simulation prova{25000 ,3, 25000, 4, 5, 500, 1000,0.5,0.3,0.2,0.5,20,10};

        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<float> duration = end - start;
        std::cout << "Time taken : " << duration.count() << " s " << std::endl;

        auto start2 = std::chrono::high_resolution_clock::now();

        prova.move();

        auto end2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration2 = end2 - start2;
        std::cout << "Time taken : " << duration2.count() << " s " << std::endl;

        auto start3 = std::chrono::high_resolution_clock::now();

        prova.move();

        auto end3 = std::chrono::high_resolution_clock::now();

        std::chrono::duration<float> duration3 = end3 - start3;
        std::cout << "Time taken : " << duration3.count() << " s " << std::endl;

        auto start4 = std::chrono::high_resolution_clock::now();

        prova.spread();

        auto end4 = std::chrono::high_resolution_clock::now();

        std::chrono::duration<float> duration4 = end4 - start4;
        std::cout << "Time taken : " << duration4.count() << " s " << std::endl;
    */

    auto start = std::chrono::high_resolution_clock::now();

    Simulation prova{25000, 3, 200, 4, 5, 1000, 800, 0.1, 0.02, 0.1, 1, 20, 10};
    for (int k = 0; k < 10; ++k)
    {
        prova.move();
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> duration = end - start;
    std::cout << "Generation and first movement : " << duration.count() << " s " << std::endl;

    auto start1 = std::chrono::high_resolution_clock::now();
    auto start2 = std::chrono::high_resolution_clock::now();

    auto end1 = std::chrono::high_resolution_clock::now();
    auto end2 = std::chrono::high_resolution_clock::now();

    double move_count;
    double spread_count;

    std::vector<Data> result{};
    std::vector<Position> positions{};
    std::vector<bool> at_home{};
    for (int i = 0; i < 50; ++i)
    {
        move_count = 0;
        spread_count = 0;
        start = std::chrono::high_resolution_clock::now();
        for (int j = 0; j < 10; ++j)
        {
            start1 = std::chrono::high_resolution_clock::now();
            prova.move();
            end1 = std::chrono::high_resolution_clock::now();
            start2 = std::chrono::high_resolution_clock::now();
            prova.spread();
            end2 = std::chrono::high_resolution_clock::now();
            prova.update_people_status();
            result.push_back(prova.get_data());
            positions.push_back(prova.person_pos(0, 0));
            at_home.push_back(prova.at_home(0, 0));

            std::chrono::duration<float> duration1 = end1 - start1;
            std::chrono::duration<float> duration2 = end2 - start2;
            move_count += duration1.count();
            spread_count += duration2.count();
        }
        prova.update_Colors();
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        std::cout << i + 1 << "-nth Cycle : " << duration.count() << " s "
                  << "   Move: " << move_count << " s "
                  << "   Spread: " << spread_count << " s " << std::endl;
    }

    std::ofstream out{"output.txt"};

    for (auto &a : result)
    {
        out << "S = " << a.S << " E = " << a.E << " I = " << a.I << " R = " << a.R << std::endl;
    }

    for (unsigned long i = 0; i < positions.size(); ++i)
    {
        out << "X = " << positions[i].get_x() << " Y = " << positions[i].get_y() << "   at home: " << at_home[i]
            << std::endl;
    }

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
    mg->SetTitle("Evolution; steps; number of people");

    int t2 = 0;
    for (auto &a : result)
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
    TRootCanvas *rc = (TRootCanvas *)c0->GetCanvasImp();
    rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
    app.Run();
}