#pragma once

#include "prescan/sim/AmesimVehicleDynamicsUnit.hpp"
#include "bridge_base/bridge_shm.hpp"
#include "bridge_msgs/chassis_msgs/chassis.pb.h"
#include "bridge_msgs/control_msgs/control_cmd.pb.h"

using namespace prescan::control;
using prescan::chassis::Chassis_GearPosition;
using namespace std;
using prescan::sim::AmesimVehicleDynamicsUnit;

namespace prescan{
namespace mapping{
  void map_control(ControlCommand & control, 
                   AmesimVehicleDynamicsUnit * dynamicsUnit,
                   prescan::bridge::ShmHandler & controlHandler,
                   double timestamp,
                   double timeout, 
                   bool blocking){
  bool get_cmd = controlHandler.read_shm(prescan::bridge::p_header, &control, timeout, blocking);
  if (get_cmd){
    std::unique_lock<std::mutex> lk_dynamics(prescan::bridge::dymamicsUnit);
    auto gear = 0;
    if(control.has_brake() && control.has_throttle() && control.has_steering()){
      if (control.has_gear_location()){
          if(control.gear_location() == Chassis_GearPosition::Chassis_GearPosition_GEAR_DRIVE){
            gear = 1;
          }else if(control.gear_location() == Chassis_GearPosition::Chassis_GearPosition_GEAR_REVERSE){
            gear = -1;
          }
      }
      dynamicsUnit->vehicleControlInput().Brake = control.brake() * 1.2;
      dynamicsUnit->vehicleControlInput().Throttle = control.throttle();
      dynamicsUnit->vehicleControlInput().Gear = gear; // NOTE: I did not change here
      dynamicsUnit->vehicleControlInput().SteeringWheelAngle = control.steering() * 360 * 3.1415 / 180;
    }else{
      dynamicsUnit->vehicleControlInput().Brake = 0;
      dynamicsUnit->vehicleControlInput().Throttle = 0;
      dynamicsUnit->vehicleControlInput().Gear = gear; // NOTE: I did not change here
      dynamicsUnit->vehicleControlInput().SteeringWheelAngle = 0;
    }
    lk_dynamics.unlock();
  }

}
}
}


