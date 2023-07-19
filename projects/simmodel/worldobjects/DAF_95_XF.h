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

#include "utils/threadpool.h"
#include "prescan/api/Experiment.hpp"
#include "prescan/sim/ISimulation.hpp"
#include "prescan/sim/ISimulationModel.hpp"
#include "prescan/sim/Simulation.hpp"
#include "prescan/api/Types.hpp"
#include "prescan/sim/SelfSensorUnit.hpp"
#include "prescan/sim/StateActuatorUnit.hpp"
#include "prescan/api/Air.hpp"
#include "prescan/sim/AirSensorUnit.hpp"
#include "prescan/api/Alms.hpp"
#include "prescan/sim/AlmsSensorUnit.hpp"
#include "prescan/api/Vehicledynamics.hpp"
#include "prescan/sim/AmesimPreconfiguredDynamicsUnit.hpp"
#include "sensorsdemux/demux_selfunit.h"
#include "sensorsdemux/demux_air.h"
#include "sensorsdemux/demux_alms.h"
#include "sensorsdemux/demux_amesim.h"

class DAF_95_XF{
public:
  explicit DAF_95_XF(){
    //Construct Air sensor properties
    airUnit_DAF_95_XF_AIR_1_Range = std::make_shared<std::vector<double>>();
    airUnit_DAF_95_XF_AIR_1_Theta = std::make_shared<std::vector<double>>();
    airUnit_DAF_95_XF_AIR_1_Phi = std::make_shared<std::vector<double>>();
    airUnit_DAF_95_XF_AIR_1_ID = std::make_shared<std::vector<uint32_t>>();
    airUnit_DAF_95_XF_AIR_1_Velocity = std::make_shared<std::vector<double>>();
    airUnit_DAF_95_XF_AIR_1_Heading = std::make_shared<std::vector<double>>();

    //Construct Alms sensor properties
    almsUnit_DAF_95_XF_ALMS_1_Polylines = std::make_shared<std::vector<const PRESCAN_ALMS_DATA*>>();

  }


  ~DAF_95_XF(){
//DECONSTRUCT//

  }
  
  void registerSimulationUnits(const prescan::api::experiment::Experiment& experiment, 
                               prescan::sim::ISimulation* simulation){
    //register SelfUnit
    auto obj_DAF_95_XF = experiment.getObjectByName<prescan::api::types::WorldObject>("DAF_95_XF");
    selfUnit_DAF_95_XF = prescan::sim::registerUnit<prescan::sim::SelfSensorUnit>(simulation, obj_DAF_95_XF);

    //register StateActuator units
    stateUnit_DAF_95_XF = prescan::sim::registerUnit<prescan::sim::StateActuatorUnit>(simulation, obj_DAF_95_XF);

    //register Air sensor units
    airSensor_DAF_95_XF_AIR_1 = experiment.getObjectByName<prescan::api::air::AirSensor>("AIR_1");
    airUnit_DAF_95_XF_AIR_1 = prescan::sim::registerUnit<prescan::sim::AirSensorUnit>(simulation, airSensor_DAF_95_XF_AIR_1);
    airUnit_DAF_95_XF_AIR_1_Range->reserve(airSensor_DAF_95_XF_AIR_1.maxDetectableObjects());
    airUnit_DAF_95_XF_AIR_1_Theta->reserve(airSensor_DAF_95_XF_AIR_1.maxDetectableObjects());
    airUnit_DAF_95_XF_AIR_1_Phi->reserve(airSensor_DAF_95_XF_AIR_1.maxDetectableObjects());
    airUnit_DAF_95_XF_AIR_1_ID->reserve(airSensor_DAF_95_XF_AIR_1.maxDetectableObjects());
    airUnit_DAF_95_XF_AIR_1_Velocity->reserve(airSensor_DAF_95_XF_AIR_1.maxDetectableObjects());
    airUnit_DAF_95_XF_AIR_1_Heading->reserve(airSensor_DAF_95_XF_AIR_1.maxDetectableObjects());

    //register Alms sensor units
    almsSensor_DAF_95_XF_ALMS_1 = experiment.getObjectByName<prescan::api::alms::AlmsSensor>("ALMS_1");
    almsUnit_DAF_95_XF_ALMS_1 = prescan::sim::registerUnit<prescan::sim::AlmsSensorUnit>(simulation, almsSensor_DAF_95_XF_ALMS_1);

    //register Amesim units
    auto dynamics_DAF_95_XF = prescan::api::vehicledynamics::getAttachedAmesimPreconfiguredDynamics(obj_DAF_95_XF);
    dynamicsUnit_DAF_95_XF = prescan::sim::registerUnit<prescan::sim::AmesimVehicleDynamicsUnit>(simulation, dynamics_DAF_95_XF, "./");


  }

  void updateState(){
    //update Amesim states
    stateUnit_DAF_95_XF->stateActuatorInput() = dynamicsUnit_DAF_95_XF->stateActuatorOutput();



  }

  void initialize(prescan::sim::ISimulation* simulation){
//INITIALIZE//

  }
  

  void step(prescan::sim::ISimulation* simulation, prescan::sim::ISimulationModel * simmodel){
    //For SelfUnit, please use 'selfUnit_DAF_95_XF' directly

    //demux air sensor outputs
    prescan::sensorDemux::demux_air(
      airUnit_DAF_95_XF_AIR_1,
      //Demux:
      airUnit_DAF_95_XF_AIR_1_Range, // ->Range (valid)
      airUnit_DAF_95_XF_AIR_1_Theta, // ->Theta (valid)
      airUnit_DAF_95_XF_AIR_1_Phi, // ->Phi (valid)
      airUnit_DAF_95_XF_AIR_1_ID, // ->ID (valid)
      airUnit_DAF_95_XF_AIR_1_Velocity, // ->Velocity (valid)
      airUnit_DAF_95_XF_AIR_1_Heading); // ->Heading (valid)

    //demux lms sensor outputs
    prescan::sensorDemux::demux_alms(
      almsUnit_DAF_95_XF_ALMS_1,
      //Demux:
      almsUnit_DAF_95_XF_ALMS_1_Polylines); // ->Lines (valid)

    //For Amesim dynamics, please use 'dynamicsUnit_DAF_95_XF' directly


  }

  void terminate(prescan::sim::ISimulation* simulation){
//TERMINATE//
  }

public:
    //Add SelfUnit properties
    const std::string name_DAF_95_XF{"DAF_95_XF"};
    prescan::sim::SelfSensorUnit* selfUnit_DAF_95_XF{nullptr};

    //Add StateActuator properties
    prescan::sim::StateActuatorUnit* stateUnit_DAF_95_XF{nullptr};

    //Add Air sensor properties
    prescan::api::air::AirSensor airSensor_DAF_95_XF_AIR_1;
    prescan::sim::AirSensorUnit* airUnit_DAF_95_XF_AIR_1{nullptr};
    std::shared_ptr<std::vector<double>> airUnit_DAF_95_XF_AIR_1_Range;
    std::shared_ptr<std::vector<double>> airUnit_DAF_95_XF_AIR_1_Theta;
    std::shared_ptr<std::vector<double>> airUnit_DAF_95_XF_AIR_1_Phi;
    std::shared_ptr<std::vector<uint32_t>> airUnit_DAF_95_XF_AIR_1_ID;
    std::shared_ptr<std::vector<double>> airUnit_DAF_95_XF_AIR_1_Velocity;
    std::shared_ptr<std::vector<double>> airUnit_DAF_95_XF_AIR_1_Heading;

    //Add Alms sensor properties
    prescan::api::alms::AlmsSensor almsSensor_DAF_95_XF_ALMS_1;
    prescan::sim::AlmsSensorUnit* almsUnit_DAF_95_XF_ALMS_1{nullptr};
    std::shared_ptr<std::vector<const PRESCAN_ALMS_DATA*>> almsUnit_DAF_95_XF_ALMS_1_Polylines;

    //Add Amesim properties
    prescan::sim::AmesimVehicleDynamicsUnit* dynamicsUnit_DAF_95_XF{nullptr};


};

