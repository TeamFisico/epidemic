//////////////////// PARAMETERS OF THE SIMULATION ////////////////

#include "configuration.h" //get the CMAKE VARIABLES parameters

int constexpr CLUSTERS_SIZE = CLUSTERS;
int constexpr WAYPOINTS_SIZE = WAYPOINTS;
int constexpr POPULATION_SIZE = POPULATION;
double constexpr TRANSMISSION_RANGE = 2.0;
double constexpr MAXIMUM_GROUP_PROBABILITY = 0.5;
double constexpr HOME_RADIUS = 0.25;
// double constexpr INTENSIVE_CARE_UNIT_SIZE = 0.025;   //capacity of ICU of a cluster with respecto to its size
double constexpr WHITE_LATP_PARAMETER = 0.5;
double constexpr YELLOW_LATP_PARAMETER = 10.0;
double constexpr ORANGE_LATP_PARAMETER = 50.0;
double constexpr RED_LATP_PARAMETER = 100.0;
double constexpr WHITE_CLUSTER_RATIO = 0.2;  // ratio infected-susceptible for a cluster to be white
double constexpr YELLOW_CLUSTER_RATIO = 0.3; // ratio infected-susceptible for a cluster to be yellow
double constexpr ORANGE_CLUSTER_RATIO = 0.4; // ratio infected-susceptible for a cluster to be orange
// double constexpr RED_CLUSTER_RATIO = 0.6;    //ratio infected-susceptible for a cluster to be red
double constexpr TIME_STEP = 0.5;
int constexpr CHECK_ZONES_INTERVAL = 5;
double constexpr VISITING_PERCENTAGE_WHITE = 0.4;
double constexpr VISITING_PERCENTAGE_YELLOW = 0.3;
double constexpr VISITING_PERCENTAGE_ORANGE = 0.2;
double constexpr VISITING_PERCENTAGE_RED = 0.1;
double constexpr AVERAGE_FAMILY_SIZE = 2.5;
double constexpr PAUSE_EXPONENT = 0.5;
double constexpr MAX_PAUSE = 60.0;               // maximum pause time(steps)
double constexpr MIN_PAUSE = 5.0;                // miminim pause time(steps)
double constexpr STARTING_VELOCITY[2]{0.5, 0.5}; // initial velocity vector assigned to all peoople
double constexpr MEAN_SPEED = 2.0;
double constexpr MAXIMUM_VELOCITY_STDDEV = 1.0;
double constexpr OTHER_CLUSTERS_PROBABILITY =
    0.30; // probability to choose a location in other white clusters(when available)