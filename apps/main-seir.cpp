////////  STL ///////
#include <fstream>
#include <iomanip>
#include <iostream>
//////  LYRA (CMD LINE PARSER) //////
#include <lyra/lyra.hpp>
/////////  ROOT  ///////
#include "TApplication.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TRootCanvas.h"
////// PROJECT HEADERS //////
#include "parameters.hpp"
#include "seir.hpp"

int main(int argc, char* argv[])
{
    ////////////////////////////////////////////////  INPUT COLLECTION  ////////////////////////////////////////////////

    /* clang-format off */

    bool get_help = false;               // evaluates true if there's any lyra-detected input error
    bool default_sim {false};            //should the simulation be performed with default parameters?
    bool default_seir {true};            // are ratios of S,E,I,R individuals among the population(which is specified) default chosen?
    bool default_params {true};          // should default values for alpha,beta and gamma be used rather than user defined?
    bool numerical_method {0};          //method use to solve SEIR ode system(Euler method by default)
    int people{};
    static int susceptibles{};
    static int exposed{};
    static int infected{};
    static int recovered{};
    double alpha{};
    double beta{};
    double gamma{};
    int time{};


    lyra::cli cli;  //Lyra object: command line input

    cli.add_argument(lyra::help(get_help))
            .add_argument(lyra::opt(default_sim, "default")
                          ["--def"]["--default"]
                                  .help("Perform the simulation with default chosen values"))
            .add_argument(lyra::opt(numerical_method, "numerical method")
                          ["-m"]["--method"]
                                  .help("Numerical method used to solve SEIR odes: \n'0'(false)-->Euler Method\n'1'(true)-->Runge Kutta Method\n"))
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
            .add_argument(lyra::opt(time, "time")
                          ["-t"]["--time"]
                                  .choices([](int value){ return value > 0;})
                                  .help("How many days should the simulation last for?"));

    /* clang-format on */

    // Parse the program arguments:
    auto result = cli.parse({argc, argv});

    // Check that the arguments validity
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
    if (default_sim && (people != 0 || !default_seir || !default_params || numerical_method != 0 || time != 0))
    {
        std::cerr << "The simulation mode has been setted as default mode, but some parameters have been specified by "
                     "the user\n";
        std::cerr << "Use --def 1 (or --default 1) without any other argument to simulate with default values"
                  << std::endl;
        return EXIT_FAILURE;
    }

    /////////// The user has chosen to use default chosen parameters ///////////

    if (default_sim)
    {
        people = DEF_SEIR_PEOPLE;
        susceptibles = people * DEF_SEIR_S;
        exposed = people * DEF_SEIR_E;
        infected = people * DEF_SEIR_I;
        recovered = people * DEF_SEIR_R;
        alpha = DEF_SEIR_ALPHA;
        beta = DEF_SEIR_BETA;
        gamma = DEF_SEIR_GAMMA;
        time = DEF_SEIR_DURATION;
    }

    /////////// The user has chosen to set simulation parameters himself ///////////

    else
    {
        // handle missing input cases
        if (default_seir && people == 0)
        {
            std::cerr << "At least one parameter among 'people' and the group 'S,E,I,R' must be specified."
                      << std::endl;
            return EXIT_FAILURE;
        }
        // only simulation people number have been specified: set S,E,I,R default ratio among people
        if (default_seir)
        {
            susceptibles = people * DEF_SEIR_S;
            exposed = people * DEF_SEIR_E;
            infected = people * DEF_SEIR_I;
            recovered = people * DEF_SEIR_R;
        }

        // automatically set the chosen epidemic parameters if not specified in the command line
        if (default_params)
        {
            alpha = DEF_SEIR_ALPHA;
            beta = DEF_SEIR_BETA;
            gamma = DEF_SEIR_GAMMA;
        }
        if (time == 0) { time = DEF_SEIR_DURATION; }
    }

    // Temporary checking for correct variables assignment
    std::cout << "People == " << people << std::endl;
    std::cout << "S == " << susceptibles << "\nE == " << exposed << "\nI == " << infected << "\nR == " << recovered
              << "\n";
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    using namespace seir;

    State initial_state{(double)susceptibles, (double)exposed, (double)infected, (double)recovered};
    SEIR simulation{people, time, initial_state, alpha, beta, gamma};

    // Vector containing all the states over the simulation time
    std::vector<State> states{};
    // Fill the states vector with the results
    simulation.evolve(states, numerical_method);

    // Output the table on command line

    std::cout << "┌─────┬───────────────┬───────────────┬───────────────┬────────"
                 "───────┐"
              << std::endl;
    std::cout << "│ Day │       S       │       E       │       I       │       "
                 "R       │"
              << std::endl;
    std::cout << "├─────├───────────────├───────────────├───────────────├────────"
                 "───────├"
              << std::endl;
    int t1 = 0;
    for (auto& a : states)
    {
        std::cout << std::setprecision(9) << std::left << "│" << std::setw(5) << t1 << "│" << std::setw(15) << a.S
                  << "│" << std::setw(15) << a.E << "│" << std::setw(15) << a.I << "│" << std::setw(15) << a.R << "│"
                  << std::endl;
        t1++;
    }
    std::cout << "└─────┴───────────────┴───────────────┴───────────────┴────────"
                 "───────┘"
              << std::endl;

    ///////////////// TXT OUTPUT /////////////////

    std::ofstream out{"output.txt"};

    int day = 0;
    for (auto& a : states)
    {
        out << "Day = " << day << " S = " << a.S << " E = " << a.E << " I = " << a.I << " R = " << a.R << std::endl;
        ++day;
    }

    ///////////////// ROOT CODE /////////////////
    // Create the app
    TApplication app("app", &argc, argv);
    // Create the canvas
    auto c0 = new TCanvas("c0", "Epidemic evolution");
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
    mg->SetTitle("Evolution; time (days); number of people");
    // Set the points in the graphs
    int t = 0;
    for (auto a : states)
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
    auto file = new TFile("seir.root", "RECREATE");
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
