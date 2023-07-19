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
#include "prescan/api/Lms.hpp"
#include "prescan/sim/LmsSensorUnit.hpp"
#include "prescan/api/Trajectory.hpp"
#include "prescan/sim/StateActuatorUnit.hpp"
#include "prescan/sim/SpeedProfileUnit.hpp"
#include "prescan/sim/PathUnit.hpp"
#include "sensorsdemux/demux_selfunit.h"
#include "sensorsdemux/demux_lms.h"

class MercedesBenz{
public:
  explicit MercedesBenz(){
    //Construct Lms sensor properties
    lmsUnit_MercedesBenz_LMSSensor_1_Lines = std::make_shared<std::vector<std::vector<PRESCAN_LMS_DATA>>>();

  }


  ~MercedesBenz(){
//DECONSTRUCT//

  }
  
  void registerSimulationUnits(const prescan::api::experiment::Experiment& experiment, 
                               prescan::sim::ISimulation* simulation){
    //register SelfUnit
    auto obj_MercedesBenz = experiment.getObjectByName<prescan::api::types::WorldObject>("MercedesBenz");
    selfUnit_MercedesBenz = prescan::sim::registerUnit<prescan::sim::SelfSensorUnit>(simulation, obj_MercedesBenz);

    //register StateActuator units
    stateUnit_MercedesBenz = prescan::sim::registerUnit<prescan::sim::StateActuatorUnit>(simulation, obj_MercedesBenz);

    //register Lms sensor units
    lmsSensor_MercedesBenz_LMSSensor_1 = experiment.getObjectByName<prescan::api::lms::LmsSensor>("LMSSensor_1");
    lmsUnit_MercedesBenz_LMSSensor_1 = prescan::sim::registerUnit<prescan::sim::LmsSensorUnit>(simulation, lmsSensor_MercedesBenz_LMSSensor_1, simulation->getSimulationPath());

    //register Trajectory units
    auto trj_MercedesBenz = prescan::api::trajectory::getActiveTrajectory(obj_MercedesBenz);
    pathUnit_MercedesBenz_Trajectory_1 = prescan::sim::registerUnit<prescan::sim::PathUnit>(simulation, trj_MercedesBenz.path(), obj_MercedesBenz);
    speedUnit_MercedesBenz_Trajectory_1 = prescan::sim::registerUnit<prescan::sim::SpeedProfileUnit>(simulation, trj_MercedesBenz.speedProfile());


  }

  void updateState(){
    //update Trajectory states
    pathUnit_MercedesBenz_Trajectory_1->motionInput() = speedUnit_MercedesBenz_Trajectory_1->motionOutput();
    stateUnit_MercedesBenz->stateActuatorInput() = pathUnit_MercedesBenz_Trajectory_1->stateActuatorOutput();

  }

  void initialize(prescan::sim::ISimulation* simulation){
//INITIALIZE//

  }
  

  void step(prescan::sim::ISimulation* simulation, prescan::sim::ISimulationModel * simmodel){
    //For SelfUnit, please use 'selfUnit_MercedesBenz' directly

    //demux lms sensor outputs
    prescan::sensorDemux::demux_lms(
      lmsUnit_MercedesBenz_LMSSensor_1,
      //Demux:
      lmsUnit_MercedesBenz_LMSSensor_1_Lines); // ->Lines (valid)


  }

  void terminate(prescan::sim::ISimulation* simulation){
//TERMINATE//
  }

public:
    //Add SelfUnit properties
    const std::string name_MercedesBenz{"MercedesBenz"};
    prescan::sim::SelfSensorUnit* selfUnit_MercedesBenz{nullptr};

    //Add StateActuator properties
    prescan::sim::StateActuatorUnit* stateUnit_MercedesBenz{nullptr};

    //Add Lms sensor properties
    prescan::api::lms::LmsSensor lmsSensor_MercedesBenz_LMSSensor_1;
    prescan::sim::LmsSensorUnit* lmsUnit_MercedesBenz_LMSSensor_1{nullptr};
    std::shared_ptr<std::vector<std::vector<PRESCAN_LMS_DATA>>> lmsUnit_MercedesBenz_LMSSensor_1_Lines;

    //Add Trajectory properties
    prescan::sim::PathUnit* pathUnit_MercedesBenz_Trajectory_1{nullptr};
    prescan::sim::SpeedProfileUnit* speedUnit_MercedesBenz_Trajectory_1{nullptr};


};

