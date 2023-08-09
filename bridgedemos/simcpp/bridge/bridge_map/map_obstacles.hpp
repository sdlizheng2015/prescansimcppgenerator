#pragma once

#include "prescan//api/Air.hpp"
#include "prescan/sim/AirSensorUnit.hpp"
#include "prescan/sim/SelfSensorUnit.hpp"
#include "prescan/api/Vehicledynamics.hpp"
#include "prescan/sim/AmesimVehicleDynamicsUnit.hpp"
#include "bridge_base/bridge_shm.hpp"
#include "bridge_msgs/perception_msgs/obstacle.pb.h"
#include "worldobjects/objectsprovider.h"

using namespace std;
using prescan::sim::SelfSensorUnit;
using prescan::sim::AmesimVehicleDynamicsUnit;
using prescan::sim::AirSensorUnit;
using namespace prescan::perception;


namespace prescan{
namespace mapping{
  void map_obstacles(Obstacles & obstacles, 
                     SelfSensorUnit* selfUnit,
                     std::shared_ptr<std::vector<double>> Range,
                     std::shared_ptr<std::vector<double>> Theta,
                     std::shared_ptr<std::vector<double>> Phi,
                     std::shared_ptr<std::vector<uint32_t>> ID,
                     std::shared_ptr<std::vector<double>> Velocity,
                     std::shared_ptr<std::vector<double>> Heading,
                     ObjectsProvider & provider,
                     prescan::bridge::ShmHandler & obstaclesHandler,
                     double timestamp,
                     double timeout, 
                     bool blocking){
  auto header = obstacles.mutable_header();
  header->set_timestamp_sec(timestamp);
  header->set_module_name("Obstacles");

  obstacles.clear_perception_obstacle();
  
  std::unique_lock<std::mutex> lk_selfUnit(prescan::bridge::selfUnit);
  auto selfHeading = selfUnit->selfSensorOutput().OrientationYaw;
  lk_selfUnit.unlock();

  for(int i=0; i< Range->size();i++){
    auto onjectinfo = provider.get_object_from_ID((*ID)[i]);
    auto obstacle = obstacles.mutable_perception_obstacle()->Add();

    obstacle->set_id((*ID)[i]);

    auto box = obstacle->mutable_box();
    box->set_size_x(onjectinfo->BoundingBoxSize.X);
    box->set_size_y(onjectinfo->BoundingBoxSize.Y);
    box->set_size_z(onjectinfo->BoundingBoxSize.Z);

    auto pose = obstacle->mutable_position();
    pose->set_x(((*Range)[i] * cos(-(*Theta)[i] * (M_PI/180))) * cos((*Phi)[i] * (M_PI/180)));
    pose->set_y(((*Range)[i] * sin(-(*Theta)[i] * (M_PI/180))) * cos((*Phi)[i] * (M_PI/180)));
    pose->set_z((*Range)[i] * sin((*Phi)[i] * (M_PI/180)));

    auto selfHeading = selfUnit->selfSensorOutput().OrientationYaw;
    obstacle->set_heading(90 - (*Heading)[i] - selfHeading * 180 / M_PI);

    auto velocity = obstacle->mutable_velocity();
    velocity->set_x((*Velocity)[i]);

  }

  // std::cout << "obstacles: " << obstacles.ByteSizeLong() << std::endl;
  obstaclesHandler.write_shm(prescan::bridge::p_header, &obstacles, timeout, blocking);
}
}
}

