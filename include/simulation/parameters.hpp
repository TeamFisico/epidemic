#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

double constexpr PI = 3.14159265358979323846;

double constexpr TRANSMISSION_RANGE = 0.5;
double constexpr HOME_RADIUS = 0.5;
double constexpr MEAN_GROUP_SIZE = 0.125;     // fraction of available locations
double constexpr GROUP_SIZE_STDDEV = 0.03125; // fraction of available locations
double constexpr MEAN_FAMILY_COMPONENTS = 2.5;
double constexpr MEAN_FAMILY_STDDEV = 0.5;
double constexpr LOCATION_RADIUS_MEAN = 4;
double constexpr LOCATION_RADIUS_DEV = 1;
double constexpr MINIMUM_LOCATION_RADIUS = 0.5;
double constexpr MAXIMUM_ANGLE_VARIATION = PI / 36; // random variation in Position::move_toward()
double constexpr HOME_PROBABILITY = 0.98;
double constexpr GREEN_PATH_MEAN = 4; // mean number of locations when filling path in a green cluster
double constexpr GREEN_PATH_STDDEV = 1;
double constexpr YELLOW_PATH_MEAN = 2; // mean number of locations when filling path in a yellow cluster
double constexpr YELLOW_PATH_STDDEV = 0.5;
double constexpr RED_PATH_MEAN = 0; // mean number of locations when filling path in a red cluster
double constexpr RED_PATH_STDDEV = 0.2;
double constexpr YELLOW_ZONE_CONDITION = 0.04;
double constexpr RED_ZONE_CONDITION = 0.12;
double constexpr WHITE_ZONE_LATP_ALPHA = 5;
double constexpr YELLOW_ZONE_LATP_ALPHA = 15;
double constexpr RED_ZONE_LATP_ALPHA = 30;
double constexpr PAUSE_EXPONENT = 0.5;
int constexpr UPDATE_ZONES_INTERVAL = 10;
double constexpr MEAN_SPEED = 3.5;
double constexpr SPEED_STDDEV = 1.0;
double constexpr MAX_PAUSE = 15.0;
double constexpr MIN_PAUSE = 5.0;
double constexpr SPREAD_RADIUS = 1.0;
////////////////////////////  DEFAULT SEIR MODEL PARAMETERS ////////////////////////////
int constexpr DEF_SEIR_PEOPLE = 1000000;
double constexpr DEF_SEIR_S = 0.99;
double constexpr DEF_SEIR_E = 0.003;
double constexpr DEF_SEIR_I = 0.005;
double constexpr DEF_SEIR_R = 0.002;
double constexpr DEF_SEIR_ALPHA = 0.035;
double constexpr DEF_SEIR_BETA = 0.15;
double constexpr DEF_SEIR_GAMMA = 0.015;
double constexpr DEF_SEIR_DURATION = 300;
////////////////////////////  DEFAULT SIMULATION PARAMETERS ////////////////////////////
int constexpr DEF_PEOPLE = 25000;
int constexpr DEF_CLUSTERS = 10;
int constexpr DEF_LOCATIONS = 1500;
double constexpr DEF_S = 0.95;
double constexpr DEF_E = 0.03;
double constexpr DEF_I = 0.015;
double constexpr DEF_R = 0.005;
int constexpr DEF_SIDE = 1000;
double constexpr DEF_ALPHA = 0.05;
double constexpr DEF_BETA = 0.04;
double constexpr DEF_GAMMA = 0.015;
double constexpr DEF_DURATION = 125;

#endif