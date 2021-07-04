#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

double constexpr PI = 3.14159265358979323846;

double constexpr TRANSMISSION_RANGE = 0.5;
double constexpr HOME_RADIUS = 0.5;
double constexpr MEAN_GROUP_SIZE = 0.125;
double constexpr GROUP_SIZE_STDDEV = 0.03125;
double constexpr MEAN_FAMILY_COMPONENTS = 2.5;
double constexpr MEAN_FAMILY_STDDEV = 0.5;
double constexpr LOCATION_RADIUS_MEAN = 4;
double constexpr LOCATION_RADIUS_DEV = 1;
double constexpr MAXIMUM_ANGLE_VARIATION = PI / 36;
double constexpr HOME_PROBABILITY = 0.98;
double constexpr WHITE_PATH_MEAN = 4;
double constexpr WHITE_PATH_STDDEV = 1;
double constexpr YELLOW_PATH_MEAN = 2;
double constexpr YELLOW_PATH_STDDEV = 0.5;
double constexpr RED_PATH_MEAN = 0;
double constexpr RED_PATH_STDDEV = 0.2;
// double constexpr WHITE_ZONE_CONDITION = 0.25;
double constexpr YELLOW_ZONE_CONDITION = 0.03;
double constexpr RED_ZONE_CONDITION = 0.10;
double constexpr WHITE_ZONE_LATP_ALPHA = 5;
double constexpr YELLOW_ZONE_LATP_ALPHA = 15;
double constexpr RED_ZONE_LATP_ALPHA = 30;
double constexpr PAUSE_EXPONENT = 0.5;
int constexpr UPDATE_ZONES_INTERVAL = 10;
double constexpr STEP_TIME = 10; // minutes
double constexpr MIN_SPEED = 2.0;
double constexpr MAX_SPEED = 5.0;
double constexpr MAX_PAUSE = 15.0;
double constexpr MIN_PAUSE = 5.0;

#endif