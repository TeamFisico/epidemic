#include <fstream>
#include "../src/SEIR.cpp"

int main ()
{
    State state0{10000,10,2,0,10012};
    sim ode{state0,0.5,0.9,0.2};
    std::vector<State> result = ode.generate_all_points(150);

    std::ofstream out {"output.txt"};

    for (auto& a: result) {

        out << "S = " << a.S << " E = " << a.E << " I = " << a.I << " R = " << a.R << std::endl;

    }
    return 0;
}