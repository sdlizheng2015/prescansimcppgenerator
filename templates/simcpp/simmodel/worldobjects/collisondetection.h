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

#include "prescan/api/Collisiondetection.hpp"
#include "prescan/sim/CollisionDetectionUnit.hpp"
#include "prescan/api/Experiment.hpp"
#include "prescan/sim/Simulation.hpp"
#include "prescan/api/Utils.hpp"
#include <vector>


class CollisionDetection{
public:
  explicit CollisionDetection(){
    collisonOutput = std::make_shared<std::vector<const PRESCAN_COLLISIONDETECTION_DATA*>>();
  }


  ~CollisionDetection(){
//DECONSTRUCT//

  }
  
  void registerSimulationUnits(const prescan::api::experiment::Experiment& experiment, 
                               prescan::sim::ISimulation* simulation){
    collisonDetectionSensor = prescan::api::collisiondetection::getCollisionDetection(experiment);
    collisionDetectionUnit = prescan::sim::registerUnit<prescan::sim::CollisionDetectionUnit>(simulation, collisonDetectionSensor);

  }

  void updateState(){
//UPDATE//

  }

  void initialize(prescan::sim::ISimulation* simulation){
//INITIALIZE//

  }
  

  void step(prescan::sim::ISimulation* simulation, prescan::sim::ISimulationModel * simmodel){
//STEP//
    auto Output = collisionDetectionUnit->collisionDetectionOutput();
    collisonOutput->assign(Output.data(), Output.data() + Output.size()); 
  }

  void terminate(prescan::sim::ISimulation* simulation){
//TERMINATE//
  }

private:
    //Add properties
    prescan::api::collisiondetection::CollisionDetection collisonDetectionSensor;
    prescan::sim::CollisionDetectionUnit* collisionDetectionUnit;

public:
    std::shared_ptr<std::vector<const PRESCAN_COLLISIONDETECTION_DATA*>> collisonOutput;
};

