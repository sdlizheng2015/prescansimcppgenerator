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
#include "prescan/api/Pcs.hpp"
#include "prescan/sim/PcsSensorUnit.hpp"
#include "prescan/api/Trajectory.hpp"
#include "prescan/sim/StateActuatorUnit.hpp"
#include "prescan/sim/SpeedProfileUnit.hpp"
#include "prescan/sim/PathUnit.hpp"
#include "sensorsdemux/demux_selfunit.h"
#include "sensorsdemux/demux_pcsRange.h"
#include "sensorsdemux/demux_pcsPosition.h"

class Skywell_1{
public:
  explicit Skywell_1(){
    //Add PCS constructors
    pcsUnit_Skywell_1_PointCloudSensor_1_Range = std::make_shared<std::vector<float>>();
    pcsUnit_Skywell_1_PointCloudSensor_1_Intensity = std::make_shared<std::vector<float>>();
    pcsUnit_Skywell_1_PointCloudSensor_1_ID = std::make_shared<std::vector<int32_t>>();
    pcsUnit_Skywell_1_PointCloudSensor_1_Angle_Theta = std::make_shared<std::vector<float>>();
    pcsUnit_Skywell_1_PointCloudSensor_1_Angle_Phi = std::make_shared<std::vector<float>>();

    //Add PCS constructors
    pcsUnit_Skywell_1_PointCloudSensor_2_X = std::make_shared<std::vector<float>>();
    pcsUnit_Skywell_1_PointCloudSensor_2_Y = std::make_shared<std::vector<float>>();
    pcsUnit_Skywell_1_PointCloudSensor_2_Z = std::make_shared<std::vector<float>>();
    pcsUnit_Skywell_1_PointCloudSensor_2_ID = std::make_shared<std::vector<int32_t>>();
    pcsUnit_Skywell_1_PointCloudSensor_2_I = std::make_shared<std::vector<float>>();

  }


  ~Skywell_1(){
//DECONSTRUCT//

  }
  
  void registerSimulationUnits(const prescan::api::experiment::Experiment& experiment, 
                               prescan::sim::ISimulation* simulation){
    //register SelfUnit
    auto obj_Skywell_1 = experiment.getObjectByName<prescan::api::types::WorldObject>("Skywell_1");
    selfUnit_Skywell_1 = prescan::sim::registerUnit<prescan::sim::SelfSensorUnit>(simulation, obj_Skywell_1);

    //register StateActuator units
    stateUnit_Skywell_1 = prescan::sim::registerUnit<prescan::sim::StateActuatorUnit>(simulation, obj_Skywell_1);

    //register PCS sensor units
    pcsSensor_Skywell_1_PointCloudSensor_1 = experiment.getObjectByName<prescan::api::pcs::PcsSensor>("PointCloudSensor_1");
    pcsUnit_Skywell_1_PointCloudSensor_1 = prescan::sim::registerUnit<prescan::sim::PcsSensorUnit>(simulation, pcsSensor_Skywell_1_PointCloudSensor_1);

    //register PCS sensor units
    pcsSensor_Skywell_1_PointCloudSensor_2 = experiment.getObjectByName<prescan::api::pcs::PcsSensor>("PointCloudSensor_2");
    pcsUnit_Skywell_1_PointCloudSensor_2 = prescan::sim::registerUnit<prescan::sim::PcsSensorUnit>(simulation, pcsSensor_Skywell_1_PointCloudSensor_2);

    //register Trajectory units
    auto trj_Skywell_1 = prescan::api::trajectory::getActiveTrajectory(obj_Skywell_1);
    pathUnit_Skywell_1_Trajectory_3 = prescan::sim::registerUnit<prescan::sim::PathUnit>(simulation, trj_Skywell_1.path(), obj_Skywell_1);
    speedUnit_Skywell_1_Trajectory_3 = prescan::sim::registerUnit<prescan::sim::SpeedProfileUnit>(simulation, trj_Skywell_1.speedProfile());


  }

  void updateState(){
    //update Trajectory states
    pathUnit_Skywell_1_Trajectory_3->motionInput() = speedUnit_Skywell_1_Trajectory_3->motionOutput();
    stateUnit_Skywell_1->stateActuatorInput() = pathUnit_Skywell_1_Trajectory_3->stateActuatorOutput();

  }

  void initialize(prescan::sim::ISimulation* simulation){
//INITIALIZE//

  }
  

  void step(prescan::sim::ISimulation* simulation, prescan::sim::ISimulationModel * simmodel){
    //For SelfUnit, please use 'selfUnit_Skywell_1' directly

    //demux PCS sensor outputs
    //If the output is disabled, DON'T/CAN'T replace its Terminator
    prescan::sensorDemux::demux_pcsRange(
      pcsUnit_Skywell_1_PointCloudSensor_1,
      pcsSensor_Skywell_1_PointCloudSensor_1,
      0, // outputDopplerVelocity?, DON'T EDIT
      1, // outputIntensity?, DON'T EDIT
      1, // outputObjectID?, DON'T EDIT
      1, // outputAngles?, DON'T EDIT
      //Demux:
      Terminator, // ->Range (valid)
      Terminator, // ->Velocity (invalid)
      Terminator, // ->Intensity (valid)
      Terminator, // ->ID (valid)
      Terminator, // ->Angle_Theta (valid)
      Terminator); // ->Angle_Phi (valid)

    //demux PCS sensor outputs
    //If the output is disabled, DON'T/CAN'T replace its Terminator
    prescan::sensorDemux::demux_pcsPosition(
      pcsUnit_Skywell_1_PointCloudSensor_2,
      pcsSensor_Skywell_1_PointCloudSensor_2,
      1, // outputIntensity?, DON'T EDIT
      1, // outputObjectID?, DON'T EDIT
      //Demux:
      Terminator, // ->X (valid)
      Terminator, // ->Y (valid)
      Terminator, // ->Z (valid)
      Terminator, // ->I (valid)
      Terminator); // ->ID (valid)


  }

  void terminate(prescan::sim::ISimulation* simulation){
//TERMINATE//
  }

public:
    //Add SelfUnit properties
    const std::string name_Skywell_1{"Skywell_1"};
    prescan::sim::SelfSensorUnit* selfUnit_Skywell_1{nullptr};

    //Add StateActuator properties
    prescan::sim::StateActuatorUnit* stateUnit_Skywell_1{nullptr};

    //Add PCS sensor properties
    prescan::api::pcs::PcsSensor pcsSensor_Skywell_1_PointCloudSensor_1;
    prescan::sim::PcsSensorUnit* pcsUnit_Skywell_1_PointCloudSensor_1{nullptr};
    std::shared_ptr<std::vector<float>> pcsUnit_Skywell_1_PointCloudSensor_1_Range;
    std::shared_ptr<std::vector<float>> pcsUnit_Skywell_1_PointCloudSensor_1_Intensity;
    std::shared_ptr<std::vector<int32_t>> pcsUnit_Skywell_1_PointCloudSensor_1_ID;
    std::shared_ptr<std::vector<float>> pcsUnit_Skywell_1_PointCloudSensor_1_Angle_Theta;
    std::shared_ptr<std::vector<float>> pcsUnit_Skywell_1_PointCloudSensor_1_Angle_Phi;

    //Add PCS sensor properties
    prescan::api::pcs::PcsSensor pcsSensor_Skywell_1_PointCloudSensor_2;
    prescan::sim::PcsSensorUnit* pcsUnit_Skywell_1_PointCloudSensor_2{nullptr};
    std::shared_ptr<std::vector<float>> pcsUnit_Skywell_1_PointCloudSensor_2_X;
    std::shared_ptr<std::vector<float>> pcsUnit_Skywell_1_PointCloudSensor_2_Y;
    std::shared_ptr<std::vector<float>> pcsUnit_Skywell_1_PointCloudSensor_2_Z;
    std::shared_ptr<std::vector<int32_t>> pcsUnit_Skywell_1_PointCloudSensor_2_ID;
    std::shared_ptr<std::vector<float>> pcsUnit_Skywell_1_PointCloudSensor_2_I;

    //Add Trajectory properties
    prescan::sim::PathUnit* pathUnit_Skywell_1_Trajectory_3{nullptr};
    prescan::sim::SpeedProfileUnit* speedUnit_Skywell_1_Trajectory_3{nullptr};


};

