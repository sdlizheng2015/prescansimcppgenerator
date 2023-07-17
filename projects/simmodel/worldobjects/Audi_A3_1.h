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
#include "prescan/api/Vehicledynamics.hpp"
#include "prescan/sim/AmesimPreconfiguredDynamicsUnit.hpp"
#include "prescan/api/Camera.hpp"
#include "prescan/sim/CameraSensorUnit.hpp"
#include "prescan/api/Trajectory.hpp"
#include "prescan/sim/StateActuatorUnit.hpp"
#include "prescan/sim/SpeedProfileUnit.hpp"
#include "prescan/sim/PathUnit.hpp"
#include "sensorsdemux/demux_selfunit.h"
#include "sensorsdemux/demux_air.h"
#include "sensorsdemux/demux_amesim.h"
#include "sensorsdemux/demux_cameraSimulinkU8.h"

class Audi_A3_1{
public:
  explicit Audi_A3_1(){
    //Construct Air sensor properties
    airUnit_Audi_A3_1_AIR_1_Range = std::make_shared<std::vector<double>>();
    airUnit_Audi_A3_1_AIR_1_Theta = std::make_shared<std::vector<double>>();
    airUnit_Audi_A3_1_AIR_1_Phi = std::make_shared<std::vector<double>>();
    airUnit_Audi_A3_1_AIR_1_ID = std::make_shared<std::vector<uint32_t>>();
    airUnit_Audi_A3_1_AIR_1_Velocity = std::make_shared<std::vector<double>>();
    airUnit_Audi_A3_1_AIR_1_Heading = std::make_shared<std::vector<double>>();

    //Construct Camera sensor properties
    cameraUnit_Audi_A3_1_CameraSensor_1_R = std::make_shared<std::vector<uint8_t>>();
    cameraUnit_Audi_A3_1_CameraSensor_1_G = std::make_shared<std::vector<uint8_t>>();
    cameraUnit_Audi_A3_1_CameraSensor_1_B = std::make_shared<std::vector<uint8_t>>();

  }


  ~Audi_A3_1(){
//DECONSTRUCT//

  }
  
  void registerSimulationUnits(const prescan::api::experiment::Experiment& experiment, 
                               prescan::sim::ISimulation* simulation){
    //register SelfUnit
    auto obj_Audi_A3_1 = experiment.getObjectByName<prescan::api::types::WorldObject>("Audi_A3_1");
    selfUnit_Audi_A3_1 = prescan::sim::registerUnit<prescan::sim::SelfSensorUnit>(simulation, obj_Audi_A3_1);

    //register StateActuator units
    stateUnit_Audi_A3_1 = prescan::sim::registerUnit<prescan::sim::StateActuatorUnit>(simulation, obj_Audi_A3_1);

    //register Air sensor units
    airSensor_Audi_A3_1_AIR_1 = experiment.getObjectByName<prescan::api::air::AirSensor>("AIR_1");
    airUnit_Audi_A3_1_AIR_1 = prescan::sim::registerUnit<prescan::sim::AirSensorUnit>(simulation, airSensor_Audi_A3_1_AIR_1);
    airUnit_Audi_A3_1_AIR_1_Range->reserve(airSensor_Audi_A3_1_AIR_1.maxDetectableObjects());
    airUnit_Audi_A3_1_AIR_1_Theta->reserve(airSensor_Audi_A3_1_AIR_1.maxDetectableObjects());
    airUnit_Audi_A3_1_AIR_1_Phi->reserve(airSensor_Audi_A3_1_AIR_1.maxDetectableObjects());
    airUnit_Audi_A3_1_AIR_1_ID->reserve(airSensor_Audi_A3_1_AIR_1.maxDetectableObjects());
    airUnit_Audi_A3_1_AIR_1_Velocity->reserve(airSensor_Audi_A3_1_AIR_1.maxDetectableObjects());
    airUnit_Audi_A3_1_AIR_1_Heading->reserve(airSensor_Audi_A3_1_AIR_1.maxDetectableObjects());

    //register Amesim units
    auto dynamics_Audi_A3_1 = prescan::api::vehicledynamics::getAttachedAmesimPreconfiguredDynamics(obj_Audi_A3_1);
    dynamicsUnit_Audi_A3_1 = prescan::sim::registerUnit<prescan::sim::AmesimVehicleDynamicsUnit>(simulation, dynamics_Audi_A3_1, "./");

    //register Camera sensor units
    cameraSensor_Audi_A3_1_CameraSensor_1 = experiment.getObjectByName<prescan::api::camera::CameraSensor>("CameraSensor_1");
    cameraUnit_Audi_A3_1_CameraSensor_1 = prescan::sim::registerUnit<prescan::sim::CameraSensorUnit>(simulation, cameraSensor_Audi_A3_1_CameraSensor_1);

    //register Trajectory units
    auto trj_Audi_A3_1 = prescan::api::trajectory::getActiveTrajectory(obj_Audi_A3_1);
    pathUnit_Audi_A3_1_Trajectory_1 = prescan::sim::registerUnit<prescan::sim::PathUnit>(simulation, trj_Audi_A3_1.path(), obj_Audi_A3_1);
    speedUnit_Audi_A3_1_Trajectory_1 = prescan::sim::registerUnit<prescan::sim::SpeedProfileUnit>(simulation, trj_Audi_A3_1.speedProfile());


  }

  void updateState(){
    //update Amesim states
    stateUnit_Audi_A3_1->stateActuatorInput() = dynamicsUnit_Audi_A3_1->stateActuatorOutput();



  }

  void initialize(prescan::sim::ISimulation* simulation){
//INITIALIZE//

  }
  

  void step(prescan::sim::ISimulation* simulation, prescan::sim::ISimulationModel * simmodel){
    //For SelfUnit, please use 'selfUnit_Audi_A3_1' directly

    //demux air sensor outputs
    prescan::sensorDemux::demux_air(
      airUnit_Audi_A3_1_AIR_1,
      //Demux:
      Terminator, // ->Range (valid)
      Terminator, // ->Theta (valid)
      Terminator, // ->Phi (valid)
      Terminator, // ->ID (valid)
      Terminator, // ->Velocity (valid)
      Terminator); // ->Heading (valid)

    //For Amesim dynamics, please use 'dynamicsUnit_Audi_A3_1' directly

    //demux camera sensor outputs
    prescan::sensorDemux::demux_cameraSimulinkU8(
      cameraUnit_Audi_A3_1_CameraSensor_1,
      //Demux:
      Terminator, // ->R (valid)
      Terminator, // ->G (valid)
      Terminator); // ->B (valid)


  }

  void terminate(prescan::sim::ISimulation* simulation){
//TERMINATE//
  }

public:
    //Add SelfUnit properties
    const std::string name_Audi_A3_1{"Audi_A3_1"};
    prescan::sim::SelfSensorUnit* selfUnit_Audi_A3_1{nullptr};

    //Add StateActuator properties
    prescan::sim::StateActuatorUnit* stateUnit_Audi_A3_1{nullptr};

    //Add Air sensor properties
    prescan::api::air::AirSensor airSensor_Audi_A3_1_AIR_1;
    prescan::sim::AirSensorUnit* airUnit_Audi_A3_1_AIR_1{nullptr};
    std::shared_ptr<std::vector<double>> airUnit_Audi_A3_1_AIR_1_Range;
    std::shared_ptr<std::vector<double>> airUnit_Audi_A3_1_AIR_1_Theta;
    std::shared_ptr<std::vector<double>> airUnit_Audi_A3_1_AIR_1_Phi;
    std::shared_ptr<std::vector<uint32_t>> airUnit_Audi_A3_1_AIR_1_ID;
    std::shared_ptr<std::vector<double>> airUnit_Audi_A3_1_AIR_1_Velocity;
    std::shared_ptr<std::vector<double>> airUnit_Audi_A3_1_AIR_1_Heading;

    //Add Amesim properties
    prescan::sim::AmesimVehicleDynamicsUnit* dynamicsUnit_Audi_A3_1{nullptr};

    //Add Camera sensor properties
    prescan::api::camera::CameraSensor cameraSensor_Audi_A3_1_CameraSensor_1;
    prescan::sim::CameraSensorUnit* cameraUnit_Audi_A3_1_CameraSensor_1{nullptr};
    std::shared_ptr<std::vector<uint8_t>> cameraUnit_Audi_A3_1_CameraSensor_1_R;
    std::shared_ptr<std::vector<uint8_t>> cameraUnit_Audi_A3_1_CameraSensor_1_G;
    std::shared_ptr<std::vector<uint8_t>> cameraUnit_Audi_A3_1_CameraSensor_1_B;

    //Add Trajectory properties
    prescan::sim::PathUnit* pathUnit_Audi_A3_1_Trajectory_1{nullptr};
    prescan::sim::SpeedProfileUnit* speedUnit_Audi_A3_1_Trajectory_1{nullptr};


};

