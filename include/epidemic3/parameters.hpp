/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////                        SIMULATION PARAMETERS                            ///////////////
/////////////                      section III B of the paper                         ///////////////
//  parameters[0] == NODES                total number of nodes in the network                     //
//  parameters[1] == AREA                 size of simulation area                                  //
//  parameters[2] == WAYPOINTS(N)         total number in the network                              //
//  parameters[3] == RANGE(R)             transmission range                                       //
//  parameters[4] == CLUSTERS(C)          total number of communities                              //
//  parameters[5] == PERCENT_WAYPOINT(y)  percentage of wpts visited by a node                     //
//  parameters[6] == ALPHA                controls the selection of the next waypoint in LATP algo //
//  parameters[7] == BETA                 Lévy exponent in pause-time distribution                 //
//  parameters[8] == MIN_PAUSE            minimum allowed pause-time(seconds) for a node           //
//  parameters[9] == MAX_PAUSE            maximum allowed pause-time(seconds) for a node           //
/////////////////////////////////////////////////////////////////////////////////////////////////////

#include <vector>

namespace SMOOTH {
    std::vector<double> parameters(10);
}


























