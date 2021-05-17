/////////////////////////////////////////////////////////////////////////////////////////////////////////
///  This file is used to pass the parameters(CMake variables) given by the user at build time with   ///
///  -DParametername=Value from command line for the second version of the program so to allow to     ///
///  use stack arrays and maximize the performance                                                    ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "configuration.h"


//TODO try-catch for checking
//getting some program inputs from Cmake Variables and doing type check
//so that the program won't compile in case of wrong input
int constexpr population_size = POPULATION;
int constexpr clusters_size = CLUSTERS;
int constexpr waypoints_size = WAYPOINTS;

