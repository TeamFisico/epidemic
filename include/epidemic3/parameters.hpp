//////////////////// PARAMETERS OF THE SIMULATION ////////////////

#include "configuration.h" //get the CMAKE VARIABLES parameters

int constexpr CLUSTERS_SIZE = CLUSTERS;
int constexpr WAYPOINTS_SIZE = WAYPOINTS;
int constexpr POPULATION_SIZE = POPULATION;
double constexpr TRANSMISSION_RANGE = 2.0;
double constexpr HOME_RADIUS = 0.5;
double constexpr TIME_STEP = 0.5; // minutes
double constexpr VISITING_PERCENTAGE = 0.4;
double constexpr PAUSE_EXPONENT = 0.5;
double constexpr MAX_PAUSE = 60.0;
double constexpr MIN_PAUSE = 5.0;