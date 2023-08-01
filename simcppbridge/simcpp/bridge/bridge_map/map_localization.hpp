#pragma once


#include "prescan/sim/SelfSensorUnit.hpp"
#include "bridge_base/bridge_shm.hpp"
#include "prescan/sim/AmesimVehicleDynamicsUnit.hpp"
#include "prescan/sim/SelfSensorUnit.hpp"
#include "bridge_base/transfer.hpp"
#include "bridge_base/wgs2utm.hpp"
#include "bridge_msgs/localiztion_msgs/localization.pb.h"

using prescan::sim::SelfSensorUnit;
using prescan::sim::AmesimVehicleDynamicsUnit;
using namespace std;
using namespace prescan::localization;


namespace prescan{
namespace mapping{
  void map_localization(LocalizationStatus & location, 
                        AmesimVehicleDynamicsUnit* dynamics, 
                        SelfSensorUnit* selfUnit,
                        prescan::bridge::ShmHandler & localizationHandler,
                        double timestamp,
                        double timeout, 
                        bool blocking){
  auto header = location.mutable_header();
  header->set_module_name("Location");
  header->set_timestamp_sec(timestamp);

  std::unique_lock<std::mutex> lk_selfUnit(prescan::bridge::selfUnit);
  auto gps = location.mutable_gps();
  auto llh = gps->mutable_llh();
  llh->set_lat(selfUnit->selfSensorOutput().Lat);
  llh->set_lon(selfUnit->selfSensorOutput().Long);
  llh->set_alt(selfUnit->selfSensorOutput().Alt);
  auto pose = location.mutable_pose();
  auto xyz = pose->mutable_xyz();
  xyz->set_x(selfUnit->selfSensorOutput().PositionX);
  xyz->set_y(selfUnit->selfSensorOutput().PositionY);
  xyz->set_z(selfUnit->selfSensorOutput().PositionZ);
  lk_selfUnit.unlock();

  std::unique_lock<std::mutex> lk_dynamics(prescan::bridge::dymamicsUnit);
  auto imu = location.mutable_imu();
  auto acceleration = imu->mutable_acceleration();
  acceleration->set_x(dynamics->stateActuatorOutput().AccelerationX);
  acceleration->set_y(dynamics->stateActuatorOutput().AccelerationY);
  acceleration->set_z(dynamics->stateActuatorOutput().AccelerationZ);
  auto orientation = imu->mutable_orientation();
  orientation->set_pitch(dynamics->stateActuatorOutput().OrientationPitch);
  orientation->set_roll(dynamics->stateActuatorOutput().OrientationRoll);
  orientation->set_yaw(dynamics->stateActuatorOutput().OrientationYaw);
  lk_dynamics.unlock();

  // std::cout << "location: " <<location.ByteSizeLong() << std::endl;
  localizationHandler.write_shm(prescan::bridge::p_header, &location, timeout, blocking);

}
}
}
