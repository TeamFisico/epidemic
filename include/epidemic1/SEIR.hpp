#include <vector>

struct State {
    unsigned int S;
    unsigned int E;
    unsigned int I;
    unsigned int R;
};

class sim {
private:
    State state0;
    double alpha;
    double beta;
    double gamma;
    int time_in_days;
public:
    sim(State state0, double alpha, double beta, double gamma, int time_in_days);
    State generate_next_point();
    std::vector<State> generate_all_points();
};



