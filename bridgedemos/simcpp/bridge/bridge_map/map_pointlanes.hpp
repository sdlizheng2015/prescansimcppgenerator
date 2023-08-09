#pragma once

#include "prescan/sim/LmsSensorUnit.hpp"
#include "bridge_base/bridge_shm.hpp"
#include "bridge_msgs/perception_msgs/obstacle.pb.h"

using namespace std;
using prescan::sim::AlmsSensorUnit;
using namespace prescan::perception;


namespace prescan{
namespace mapping{
  void map_pointlanes(PointLanes & pointLanes, 
                     std::shared_ptr<std::vector<std::vector<PRESCAN_LMS_DATA>>> lmsLanes,
                     prescan::bridge::ShmHandler & polyLanesHandler,
                     double timestamp,
                     double timeout, 
                     bool blocking){

  auto header = pointLanes.mutable_header();
  header->set_module_name("PointLanes");
  header->set_timestamp_sec(timestamp);

  pointLanes.clear_lane();
  for(int i=0; i< (*lmsLanes).size();i++){
    auto pointLane = pointLanes.mutable_lane()->Add();
    for (int j = 0;j< (*lmsLanes)[i].size();j++){
      auto point = pointLane->mutable_point()->Add();
      point->set_x((*lmsLanes)[i][j].X);
      point->set_y((*lmsLanes)[i][j].Y);
      point->set_z((*lmsLanes)[i][j].Z);
    }
  }
  std::cout << "pointLanes: " << pointLanes.ByteSizeLong() << std::endl;
  polyLanesHandler.write_shm(prescan::bridge::p_header, &pointLanes, timeout, blocking);
}
}
}

