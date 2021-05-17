#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include "group.hpp"

namespace SMOOTH{

    enum class Zone{
        white=0,
        yellow,
        orange,
        red
    };

    class Cluster{
      private:
          int sz;         //num of waypoints
          int lbl;        //corresponding index into Cluster array 0 <= lbl <= clusters_size-1
          double weight;  //weight to be chosen by a person
          Zone zone;
      public:
          std::vector<Group> Groups;  //groups of waypoints in cluster

          explicit Cluster(int size); //constructor
          Cluster(); //default constructor

          //non-modifying members
          int size() const { return sz; }
          Zone zone_type() const { return zone; }
          int label() const { return lbl; }

          void set_weight(double w) { weight = w; };
          void set_label(int n) { lbl = n; }
          void set_zone(Zone newZone) { zone = newZone; }

          double& cluster_weight() { return weight; }
          int& size () { return sz; }

          bool is_partitioned() const { return (Groups.size() > 0); } //return true if groups have been created for this cluster

          void determine_groups_sizes(); //determine the n. of waypoints associated to every group

         //setarea?

    };


} //namespace SMOOTH


#endif  //CLUSTER_HPP