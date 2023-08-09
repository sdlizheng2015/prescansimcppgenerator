//
// (c)Copyright 2023 by Siemens Industry Software and Services B.V.
// All rights reserved.
//
// Simcenter Prescan(r) has been developed at Siemens Industry Software and
// Services B.V.
//
// This document contains proprietary and confidential information of Siemens.
// The contents of this document may not be disclosed to third parties, copied
// or duplicated in any form, in whole or in part, without prior written
// permission of Siemens.
//
// The terms and conditions governing the license of Simcenter Prescan(r)
// software consist solely of those set forth in the written contracts between
// Siemens or Siemens authorized third parties and its customers. The software
// may only be used or copied in accordance with the terms of these contracts.
//


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
