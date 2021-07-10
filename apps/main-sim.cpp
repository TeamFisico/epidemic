//////  STL //////
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
//////  LYRA (CMD LINE PARSER) //////
#include <lyra/lyra.hpp>
//////  ROOT HEADERS  //////
#include "TApplication.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TRootCanvas.h"
////// PROJECT HEADERS //////
#include "simulation.hpp"

int main(int argc, char** argv)
{

    /* clang-format off */

    ////////////////////////////////////////////////  INPUT COLLECTION  ////////////////////////////////////////////////

    bool get_help = false;               // evaluates true if there's any lyra-detected input error
    bool default_sim {false};                //should the simulation be performed with default parameters?
    bool default_seir {true};            // are ratios of S,E,I,R individuals among the population(which is specified) default chosen?
    bool clusters_and_locations {false}; // are both clusters and locations values get specified?
    bool default_params {true};
    int people{};
    static int susceptibles{};
    static int exposed{};
    static int infected{};
    static int recovered{};
    static int locations{};
    static int clusters{};
    double alpha{};
    double beta{};
    double gamma{};
    int side{};
    int time{};

    lyra::cli cli;  //Lyra object: command line input

    cli.add_argument(lyra::help(get_help))
            .add_argument(lyra::opt(default_sim, "default")
                          ["--def"]["--default"]
                                  .help("Perform the simulation with default chosen values"))
            .add_argument(lyra::opt(people, "people")
                          ["-N"]["--people"]
                                  .choices([](int value){ return value > 0;})
                                  .help("How many people should there be in the simulation?"))
            .add_argument(lyra::group([&](const lyra::group &) {
                default_seir = false;
            })
                                  .add_argument(lyra::opt(susceptibles, "Susceptibles")
                                                ["-S"]
                                                        .required()
                                                        .choices([](int value){ return value > 0;})
                                                        .help("Susceptible individuals in the simulation."))
                                  .add_argument(lyra::opt(exposed, "Exposed")
                                                ["-E"]
                                                        .required()
                                                        .choices([](int value){ return value >= 0;})
                                                        .help("Exposed individuals in the simulation."))
                                  .add_argument(lyra::opt(infected, "Infected")
                                                ["-I"]
                                                        .required()
                                                        .choices([](int value){ return value >=0;})
                                                        .help("Infected individuals in the simulation."))
                                  .add_argument(lyra::opt(recovered, "Recovered")
                                                ["-R"]
                                                        .required()
                                                        .choices([](int value){ return value >= 0;})
                                                        .help("Recovered individuals in the simulation."))) //end group
            .add_argument(lyra::opt(locations, "locations")
                          ["-l"]["--loc"]
                                  .choices([](int value){ return value >= 10;})
                                  .help("How many locations should there be on the map?"))
            .add_argument(lyra::opt(clusters, "clusters")
                          ["-c"]["--clust"]
                                  .choices([](int value){ return value > 0 && value < 40;})
                                  .help("How many cluster should the area be divided into?"))
            .add_argument(lyra::group([&](const lyra::group &) {
                clusters_and_locations = true;
            })
                                  .add_argument(lyra::opt(locations, "locations")
                                                ["-l"]["--loc"]
                                                        .required()
                                                        .choices([](int value){ return value > 0; })
                                                        .help("How many locations should there be on the map?"))
                                  .add_argument(lyra::opt(clusters, "clusters")
                                                ["-c"]["--clust"]
                                                        .required()
                                                        .choices([](int value){ return value > 0; })
                                                        .help("How many cluster should the area be divided into?")))//end group
            .add_argument(lyra::group([&](const lyra::group &) {
                default_params = false;
            })
                                  .add_argument(lyra::opt(alpha, "alpha")
                                                ["-a"]["--alpha"]
                                                        .choices([](double value){ return value >= 0.0 && value <= 1.0;})
                                                        .help("Parameter: governs the lag between infectious contact and showing symptoms."))
                                  .add_argument(lyra::opt(beta, "beta")
                                                ["-b"]["--beta"]
                                                        .choices([](double value){ return value >= 0.0 && value <= 1.0;})
                                                        .help("Parameter: number of people an infective person infects each day."))
                                  .add_argument(lyra::opt(gamma, "gamma")
                                                ["-g"]["--gamma"]
                                                        .choices([](double value){ return value >= 0.0 && value <= 1.0;})
                                                        .help("Parameter: cumulative probability for a person to recover or die."))) //end group
            .add_argument(lyra::opt(side, "side")
                          ["--sd"]["--side"]
                                  .choices([](int value){ return value >= locations / 2;})
                                  .help("How big should the simulation area side be"))
            .add_argument(lyra::opt(time, "time")
                          ["-t"]["--time"]
                                  .choices([](int value){ return value > 0;})
                                  .help("How many cycles should the simulation last for?"));;

    /* clang-format on */

    // Parse the program arguments:
    auto result = cli.parse({argc, argv});

    // Check that the arguments where valid:
    if (!result)
    {
        std::cerr << "Error in command line: " << result.errorMessage() << std::endl;
        std::cerr << cli << "\n";
        return EXIT_FAILURE;
    }

    // Show help indications if needed
    if (get_help)
    {
        std::cout << cli << "\n";
        return 0;
    }

    // Terminate program in case the user chooses to perform the default simulation but also sets some parameters
    if (default_sim &&
        (people != 0 || !default_seir || locations != 0 || clusters != 0 || !default_params || side != 0))
    {
        std::cerr << "The simulation mode has been setted as default mode, but some parameters have been specified by "
                     "the user.\n";
        std::cerr << "Use --def 1 (or --default 1) without any other argument to simulate with default values."
                  << std::endl;
        return EXIT_FAILURE;
    }
    //////// Perform simulation with default chosen parameters ////////
    if (default_sim)
    {
        people = DEF_PEOPLE;
        susceptibles = people * DEF_S;
        exposed = people * DEF_E;
        infected = people * DEF_I;
        recovered = people * DEF_R;
        locations = DEF_LOCATIONS;
        clusters = DEF_CLUSTERS;
        alpha = DEF_ALPHA;
        beta = DEF_BETA;
        gamma = DEF_GAMMA;
        side = DEF_SIDE;
        time = DEF_DURATION;
    }
    //////// The user has chosen to set simulation parameters himself ////////
    else
    {
        // handle missing input cases
        if (default_seir && people == 0)
        {
            std::cerr << "At least one parameter among 'people' and the group 'S,E,I,R' must be specified."
                      << std::endl;
            return EXIT_FAILURE;
        }
        if (!clusters_and_locations && locations == 0 && clusters == 0)
        {
            std::cerr << "At least one parameter among 'clusters' and 'locations' must be specified." << std::endl;
            return EXIT_FAILURE;
        }
        if (side == 0)
        {
            std::cerr << "Side must be specified." << std::endl;
            return EXIT_FAILURE;
        }
        // only simulation people number have been specified: set S,E,I,R default ratio among people
        if (default_seir)
        {
            susceptibles = people * DEF_S;
            exposed = people * DEF_E;
            infected = people * DEF_I;
            recovered = people * DEF_R;
        }
        // only clusters have been specified leaving locations out
        if (!clusters_and_locations && locations == 0 && clusters > 0) { locations = clusters * 150; }
        // only locations have been specified leaving clusters out
        else if (!clusters_and_locations && locations > 0 && clusters == 0)
        {
            clusters = locations / 150 + 1;
        }
        // Not accepted values
        if (susceptibles + exposed + infected + recovered < locations)
        {
            std::cerr << "The total population has to be higher tha the number of locations." << std::endl;
            return EXIT_FAILURE;
        }
        if (susceptibles + exposed + infected + recovered < 100 * clusters)
        {
            std::cerr << "The total population has to be at least 100 times the number of clusters." << std::endl;
            return EXIT_FAILURE;
        }
        if (clusters * 10 > locations)
        {
            std::cerr << "the minimum number of locations per cluster has to be 10." << std::endl;
            return EXIT_FAILURE;
        }
        // automatically set the chosen epidemic parameters if not specified in the command line
        if (default_params)
        {
            alpha = DEF_ALPHA;
            beta = DEF_BETA;
            gamma = DEF_GAMMA;
        }
    }
    // Output the simulation parameters
    std::cout << "Clusters == " << clusters << "\t Locations == " << locations << std::endl;
    std::cout << "People == " << people << std::endl;
    std::cout << "S == " << susceptibles << "\nE == " << exposed << "\nI == " << infected << "\nR == " << recovered
              << std::endl;
    std::cout << "Simulation side == " << side << "\t Spread radius == " << SPREAD_RADIUS << std::endl;

    ////////////////////////////  Simulation ////////////////////////////
    using namespace smooth_sim;
    auto start = std::chrono::high_resolution_clock::now();
    // Initialize the simulation
    Simulation sim{susceptibles, exposed, infected, recovered, clusters,     locations,
                   side,         alpha,   beta,     gamma,     SPREAD_RADIUS};

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> duration = end - start;
    std::cout << "Generation: " << duration.count() << " s " << std::endl;
    // Initialize objects used to track how much it takes to move and spread for every cycle
    auto start1 = std::chrono::high_resolution_clock::now();
    auto start2 = std::chrono::high_resolution_clock::now();
    auto end1 = std::chrono::high_resolution_clock::now();
    auto end2 = std::chrono::high_resolution_clock::now();
    double move_count;
    double spread_count;
    // Initialize Result vector
    std::vector<Data> Result{};
    // Cycle trough every cycle
    for (int i = 0; i < time; ++i)
    {
        move_count = 0;
        spread_count = 0;
        start = std::chrono::high_resolution_clock::now();
        // Cycle through every step
        for (int j = 0; j < UPDATE_ZONES_INTERVAL; ++j)
        {
            // Call Move and spread functions, keeping track of their time
            start1 = std::chrono::high_resolution_clock::now();
            sim.move();
            end1 = std::chrono::high_resolution_clock::now();
            start2 = std::chrono::high_resolution_clock::now();
            sim.spread();
            end2 = std::chrono::high_resolution_clock::now();
            // Calculate how much it took
            std::chrono::duration<float> duration1 = end1 - start1;
            std::chrono::duration<float> duration2 = end2 - start2;
            move_count += duration1.count();
            spread_count += duration2.count();
            // Fill the Result vector
            Result.push_back(sim.get_data());
        }
        // Update the color of the zones and eventually clean the paths
        sim.update_zones();
        // Calculate the time it took for every cycle
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        // Output the times on the terminal
        std::cout << i + 1 << "-nth Cycle : " << duration.count() << " s "
                  << "   Move: " << move_count << " s "
                  << "   Spread: " << spread_count << " s " << std::endl;
    }

    ///////////////// TXT OUTPUT /////////////////

    std::ofstream out{"sim.txt"};

    int step = 0;
    for (auto& a : Result)
    {
        out << "Step = " << step << " S = " << a.S << " E = " << a.E << " I = " << a.I << " R = " << a.R << std::endl;
        ++step;
    }

    ///////////////// ROOT CODE /////////////////
    // Create the app
    TApplication app("app", &argc, argv);
    // Create the canvas
    auto c0 = new TCanvas("c0", "Epidemic simulation");
    // Create the multigraph
    auto mg = new TMultiGraph();
    // Create the various Graph
    auto gS = new TGraph();
    auto gE = new TGraph();
    auto gI = new TGraph();
    auto gR = new TGraph();
    // Set the Colors
    gS->SetLineColor(kBlue);
    gE->SetLineColor(kOrange);
    gI->SetLineColor(kGreen);
    gR->SetLineColor(kRed);
    mg->SetTitle("Simulation; steps; number of people");
    // Set the points in the graphs
    int t = 0;
    for (auto a : Result)
    {
        gS->SetPoint(t, t, a.S);
        gE->SetPoint(t, t, a.E);
        gI->SetPoint(t, t, a.I);
        gR->SetPoint(t, t, a.R);
        t++;
    }
    // Add the graphs in the multigraph, giving each of them the corresponding name
    mg->Add(gS);
    gS->SetTitle("S");
    mg->Add(gE);
    gE->SetTitle("E");
    mg->Add(gI);
    gI->SetTitle("I");
    mg->Add(gR);
    gR->SetTitle("R");
    // Create a Root File for the multigraph
    auto file = new TFile("sim.root", "RECREATE");
    mg->Write();
    file->Close();
    // Draw the multigraph with a legend
    mg->Draw("AL");
    c0->BuildLegend();

    c0->Modified();
    c0->Update();
    // Close the application when thw canvas is closed
    TRootCanvas* rc = (TRootCanvas*)c0->GetCanvasImp();
    rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
    app.Run();

    return 0;
}