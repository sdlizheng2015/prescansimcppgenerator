#pragma once

#include "bridge_base/bridge_base.hpp"
#include "bridge_base/bridge_shm.hpp"
#include "prescan/api/Vehicledynamics.hpp"
#include "prescan/sim/AmesimVehicleDynamicsUnit.hpp"


using namespace std;
using prescan::sim::AmesimVehicleDynamicsUnit;
using namespace prescan::chassis;

namespace prescan{
namespace mapping{
  void map_chassis(Chassis & chassis, 
                   AmesimVehicleDynamicsUnit * dynamics, 
                   prescan::bridge::ShmHandler & chassisHandler,
                   double timestamp,
                   double timeout, 
                   bool blocking){

    std::unique_lock<std::mutex> lk_dynamics(prescan::bridge::dymamicsUnit);
    chassis.set_speed_mps(dynamics->stateActuatorOutput().VelocityX);
    lk_dynamics.unlock();

    chassis.set_gear_location(prescan::chassis::Chassis_GearPosition::Chassis_GearPosition_GEAR_DRIVE);
    auto header = chassis.mutable_header();
    header->set_module_name("Chassis");
    header->set_timestamp_sec(timestamp);

    // std::cout << "chassis: " <<chassis.ByteSizeLong() << std::endl;
    chassisHandler.write_shm(prescan::bridge::p_header, &chassis, timeout, blocking);
  }
}
}


