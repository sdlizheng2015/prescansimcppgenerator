#pragma once


#include "prescan/sim/SelfSensorUnit.hpp"
#include "prescan/sim/AmesimVehicleDynamicsUnit.hpp"

using prescan::sim::SelfSensorUnit;
using prescan::sim::AmesimVehicleDynamicsUnit;


namespace prescan{
namespace sensorDemux{
  void demux_amesim(AmesimVehicleDynamicsUnit* ego_dynamicsUnit){
	  ego_dynamicsUnit->vehicleControlInput().Brake = 0;
      ego_dynamicsUnit->vehicleControlInput().Throttle = 0.3;
      ego_dynamicsUnit->vehicleControlInput().Gear = 1; // NOTE: I did not change here
      ego_dynamicsUnit->vehicleControlInput().SteeringWheelAngle = 0;

	  double head = ego_dynamicsUnit->stateActuatorOutput().OrientationYaw;
      double vel_X = ego_dynamicsUnit->stateActuatorOutput().VelocityX;
      double vel_Y = ego_dynamicsUnit->stateActuatorOutput().VelocityY;
      double vel_Z = ego_dynamicsUnit->stateActuatorOutput().VelocityZ;
      double ACC_X = ego_dynamicsUnit->stateActuatorOutput().AccelerationX;
      double ACC_Y = ego_dynamicsUnit->stateActuatorOutput().AccelerationY;
      double ACC_Z = ego_dynamicsUnit->stateActuatorOutput().AccelerationZ;
  }
}
}
