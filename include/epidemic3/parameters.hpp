//////////////////// PARAMETERS OF THE SIMULATION ////////////////

#include "configuration.h" //get the CMAKE VARIABLES parameters

int constexpr CLUSTERS_SIZE = CLUSTERS;
int constexpr WAYPOINTS_SIZE = WAYPOINTS;
int constexpr POPULATION_SIZE = POPULATION;
double constexpr TRANSMISSION_RANGE = 5.0;
double constexpr MAXIMUM_GROUP_PROBABILITY = 0.5;
double constexpr HOME_RADIUS = 0.25;
double constexpr MAXIMUM_ANGLE_VARIATION = 0.0872664626; // 5 degrees variation
// double constexpr INTENSIVE_CARE_UNIT_SIZE = 0.025;   //capacity of ICU of a cluster with respecto to its size
double constexpr WHITE_LATP_PARAMETER = 0.5;
double constexpr YELLOW_LATP_PARAMETER = 10.0;
double constexpr ORANGE_LATP_PARAMETER = 50.0;
double constexpr RED_LATP_PARAMETER = 100.0;
double constexpr WHITE_CLUSTER_RATIO = 0.2;  // ratio infected-susceptible for a cluster to be white
double constexpr YELLOW_CLUSTER_RATIO = 0.3; // ratio infected-susceptible for a cluster to be yellow
double constexpr ORANGE_CLUSTER_RATIO = 0.4; // ratio infected-susceptible for a cluster to be orange
double constexpr TIME_STEP = 10;
int constexpr UPDATE_ZONES_INTERVAL = 10; // steps
double constexpr VISITING_PERCENTAGE_WHITE = 0.4;
double constexpr VISITING_PERCENTAGE_YELLOW = 0.3;
double constexpr VISITING_PERCENTAGE_ORANGE = 0.2;
double constexpr VISITING_PERCENTAGE_RED = 0.1;
double constexpr AVERAGE_FAMILY_SIZE = 2.5;
double constexpr PAUSE_EXPONENT = 0.5;
double constexpr MAX_PAUSE = 60.0; // maximum pause time(steps)
double constexpr MIN_PAUSE = 5.0;  // miminim pause time(steps)
double constexpr MEAN_SPEED = 2.0;
double constexpr MAXIMUM_VELOCITY_STDDEV = 1.0;
double constexpr OTHER_CLUSTERS_PROBABILITY = 0.30; // prob to choose a location in other white clusters(when available)