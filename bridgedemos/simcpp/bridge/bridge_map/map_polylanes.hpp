#pragma once

#include "prescan/sim/AlmsSensorUnit.hpp"
#include "bridge_base/bridge_shm.hpp"
#include "bridge_msgs/perception_msgs/obstacle.pb.h"

using namespace std;
using prescan::sim::AlmsSensorUnit;
using namespace prescan::perception;


namespace prescan{
namespace mapping{
  void map_polylanes(PolyLanes & polyLanes, 
                     std::shared_ptr<std::vector<const PRESCAN_ALMS_DATA*>> almsLanes,
                     prescan::bridge::ShmHandler & polyLanesHandler,
                     double timestamp,
                     double timeout, 
                     bool blocking){

  auto header = polyLanes.mutable_header();
  header->set_module_name("PolyLanes");
  header->set_timestamp_sec(timestamp);

  polyLanes.clear_lane();
  for(int i=0; i< (*almsLanes).size();i++){
    auto polyLane = polyLanes.mutable_lane()->Add();
    polyLane->set_quality(1);
    polyLane->set_model_degree(3);

    auto X = polyLane->mutable_x();
    X->set_n0((*almsLanes)[i]->X.N0);
    X->set_n1((*almsLanes)[i]->X.N1);
    X->set_n2((*almsLanes)[i]->X.N2);
    X->set_n3((*almsLanes)[i]->X.N3);

    auto Y = polyLane->mutable_y();
    Y->set_n0((*almsLanes)[i]->Y.N0);
    Y->set_n1((*almsLanes)[i]->Y.N1);
    Y->set_n2((*almsLanes)[i]->Y.N2);
    Y->set_n3((*almsLanes)[i]->Y.N3);
  
    auto Z = polyLane->mutable_z();
    Z->set_n0((*almsLanes)[i]->Z.N0);
    Z->set_n1((*almsLanes)[i]->Z.N1);
    Z->set_n2((*almsLanes)[i]->Z.N2);
    Z->set_n3((*almsLanes)[i]->Z.N3);

    polyLane->set_length((*almsLanes)[i]->Length);
    polyLane->set_lane_label((*almsLanes)[i]->LineLabel);
    polyLane->set_groupid((*almsLanes)[i]->GroupID);
  }
  // std::cout << "polyLanes: " << polyLanes.ByteSizeLong() << std::endl;
  polyLanesHandler.write_shm(prescan::bridge::p_header, &polyLanes, timeout, blocking);
}
}
}

