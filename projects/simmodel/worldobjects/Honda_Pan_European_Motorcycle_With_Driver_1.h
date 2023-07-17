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
#include "prescan/api/Pointcloudlidar.hpp"
#include "prescan/sim/PointCloudLidarSensorUnit.hpp"
#include "prescan/sim/SignalView.hpp"
#include "prescan/sim/ISimulationModel.hpp"
#include "prescan/api/Trajectory.hpp"
#include "prescan/sim/StateActuatorUnit.hpp"
#include "prescan/sim/SpeedProfileUnit.hpp"
#include "prescan/sim/PathUnit.hpp"
#include "sensorsdemux/demux_selfunit.h"
#include "sensorsdemux/demux_pcl.h"

class Honda_Pan_European_Motorcycle_With_Driver_1{
public:
  explicit Honda_Pan_European_Motorcycle_With_Driver_1(){
    //Construct PCL sensor properties
    pclPointUnit_Honda_Pan_European_Motorcycle_With_Driver_1_PointCloudLidar_1_X = std::make_shared<std::vector<float>>();
    pclPointUnit_Honda_Pan_European_Motorcycle_With_Driver_1_PointCloudLidar_1_Y = std::make_shared<std::vector<float>>();
    pclPointUnit_Honda_Pan_European_Motorcycle_With_Driver_1_PointCloudLidar_1_Z = std::make_shared<std::vector<float>>();
    pclPowerUnit_Honda_Pan_European_Motorcycle_With_Driver_1_PointCloudLidar_1_I = std::make_shared<std::vector<float>>();
    pclObjectIdUnit_Honda_Pan_European_Motorcycle_With_Driver_1_PointCloudLidar_1_ID = std::make_shared<std::vector<int32_t>>();

  }


  ~Honda_Pan_European_Motorcycle_With_Driver_1(){
//DECONSTRUCT//

  }
  
  void registerSimulationUnits(const prescan::api::experiment::Experiment& experiment, 
                               prescan::sim::ISimulation* simulation){
    //register SelfUnit
    auto obj_Honda_Pan_European_Motorcycle_With_Driver_1 = experiment.getObjectByName<prescan::api::types::WorldObject>("Honda_Pan_European_Motorcycle_With_Driver_1");
    selfUnit_Honda_Pan_European_Motorcycle_With_Driver_1 = prescan::sim::registerUnit<prescan::sim::SelfSensorUnit>(simulation, obj_Honda_Pan_European_Motorcycle_With_Driver_1);

    //register StateActuator units
    stateUnit_Honda_Pan_European_Motorcycle_With_Driver_1 = prescan::sim::registerUnit<prescan::sim::StateActuatorUnit>(simulation, obj_Honda_Pan_European_Motorcycle_With_Driver_1);

    //register PCL sensor units
    pclSensor_Honda_Pan_European_Motorcycle_With_Driver_1_PointCloudLidar_1 = experiment.getObjectByName<prescan::api::pointcloudlidar::PointCloudLidarSensor>("PointCloudLidar_1");
    pclPointUnit_Honda_Pan_European_Motorcycle_With_Driver_1_PointCloudLidar_1 = prescan::sim::registerUnit<prescan::sim::pointcloudlidar::PointCloudUnit>(simulation, pclSensor_Honda_Pan_European_Motorcycle_With_Driver_1_PointCloudLidar_1);
    pclObjectIdUnit_Honda_Pan_European_Motorcycle_With_Driver_1_PointCloudLidar_1 = prescan::sim::registerUnit<prescan::sim::pointcloudlidar::ObjectIdUnit>(simulation, pclSensor_Honda_Pan_European_Motorcycle_With_Driver_1_PointCloudLidar_1);
    pclPowerUnit_Honda_Pan_European_Motorcycle_With_Driver_1_PointCloudLidar_1 = prescan::sim::registerUnit<prescan::sim::pointcloudlidar::PowerUnit>(simulation, pclSensor_Honda_Pan_European_Motorcycle_With_Driver_1_PointCloudLidar_1);

    //register Trajectory units
    auto trj_Honda_Pan_European_Motorcycle_With_Driver_1 = prescan::api::trajectory::getActiveTrajectory(obj_Honda_Pan_European_Motorcycle_With_Driver_1);
    pathUnit_Honda_Pan_European_Motorcycle_With_Driver_1_Trajectory_2 = prescan::sim::registerUnit<prescan::sim::PathUnit>(simulation, trj_Honda_Pan_European_Motorcycle_With_Driver_1.path(), obj_Honda_Pan_European_Motorcycle_With_Driver_1);
    speedUnit_Honda_Pan_European_Motorcycle_With_Driver_1_Trajectory_2 = prescan::sim::registerUnit<prescan::sim::SpeedProfileUnit>(simulation, trj_Honda_Pan_European_Motorcycle_With_Driver_1.speedProfile());


  }

  void updateState(){
    //update Trajectory states
    pathUnit_Honda_Pan_European_Motorcycle_With_Driver_1_Trajectory_2->motionInput() = speedUnit_Honda_Pan_European_Motorcycle_With_Driver_1_Trajectory_2->motionOutput();
    stateUnit_Honda_Pan_European_Motorcycle_With_Driver_1->stateActuatorInput() = pathUnit_Honda_Pan_European_Motorcycle_With_Driver_1_Trajectory_2->stateActuatorOutput();

  }

  void initialize(prescan::sim::ISimulation* simulation){
//INITIALIZE//

  }
  

  void step(prescan::sim::ISimulation* simulation, prescan::sim::ISimulationModel * simmodel){
    //For SelfUnit, please use 'selfUnit_Honda_Pan_European_Motorcycle_With_Driver_1' directly

    //demux PCL sensor outputs
    prescan::sensorDemux::demux_pcl(
      simmodel,
      pclSensor_Honda_Pan_European_Motorcycle_With_Driver_1_PointCloudLidar_1,
      pclPointUnit_Honda_Pan_European_Motorcycle_With_Driver_1_PointCloudLidar_1,
      pclObjectIdUnit_Honda_Pan_European_Motorcycle_With_Driver_1_PointCloudLidar_1,
      pclPowerUnit_Honda_Pan_European_Motorcycle_With_Driver_1_PointCloudLidar_1,
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
    const std::string name_Honda_Pan_European_Motorcycle_With_Driver_1{"Honda_Pan_European_Motorcycle_With_Driver_1"};
    prescan::sim::SelfSensorUnit* selfUnit_Honda_Pan_European_Motorcycle_With_Driver_1{nullptr};

    //Add StateActuator properties
    prescan::sim::StateActuatorUnit* stateUnit_Honda_Pan_European_Motorcycle_With_Driver_1{nullptr};

    //Add PCL sensor properties
    prescan::api::pointcloudlidar::PointCloudLidarSensor pclSensor_Honda_Pan_European_Motorcycle_With_Driver_1_PointCloudLidar_1;
    prescan::sim::pointcloudlidar::PointCloudUnit* pclPointUnit_Honda_Pan_European_Motorcycle_With_Driver_1_PointCloudLidar_1{nullptr};
    prescan::sim::pointcloudlidar::ObjectIdUnit* pclObjectIdUnit_Honda_Pan_European_Motorcycle_With_Driver_1_PointCloudLidar_1{nullptr};
    prescan::sim::pointcloudlidar::PowerUnit* pclPowerUnit_Honda_Pan_European_Motorcycle_With_Driver_1_PointCloudLidar_1{nullptr};
    std::shared_ptr<std::vector<float>> pclPointUnit_Honda_Pan_European_Motorcycle_With_Driver_1_PointCloudLidar_1_X;
    std::shared_ptr<std::vector<float>> pclPointUnit_Honda_Pan_European_Motorcycle_With_Driver_1_PointCloudLidar_1_Y;
    std::shared_ptr<std::vector<float>> pclPointUnit_Honda_Pan_European_Motorcycle_With_Driver_1_PointCloudLidar_1_Z;
    std::shared_ptr<std::vector<float>> pclPowerUnit_Honda_Pan_European_Motorcycle_With_Driver_1_PointCloudLidar_1_I;
    std::shared_ptr<std::vector<int32_t>> pclObjectIdUnit_Honda_Pan_European_Motorcycle_With_Driver_1_PointCloudLidar_1_ID;

    //Add Trajectory properties
    prescan::sim::PathUnit* pathUnit_Honda_Pan_European_Motorcycle_With_Driver_1_Trajectory_2{nullptr};
    prescan::sim::SpeedProfileUnit* speedUnit_Honda_Pan_European_Motorcycle_With_Driver_1_Trajectory_2{nullptr};


};

