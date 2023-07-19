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

#include "prescan/api/Objectlistprovider.hpp"
#include "prescan/sim/ObjectListProviderUnit.hpp"
#include "prescan/api/Experiment.hpp"
#include "prescan/sim/Simulation.hpp"
#include "prescan/api/Utils.hpp"


class ObjectsProvider{
public:
  explicit ObjectsProvider(){

  }


  ~ObjectsProvider(){
//DECONSTRUCT//

  }
  
  void registerSimulationUnits(const prescan::api::experiment::Experiment& experiment, 
                               prescan::sim::ISimulation* simulation){
    auto objectListProvier = prescan::api::objectlistprovider::getObjectListProvider(experiment);
    objProvideUnit = prescan::sim::registerUnit<prescan::sim::ObjectListProviderUnit>(simulation, objectListProvier);

  }

  void updateState(){
//UPDATE//

  }

  void initialize(prescan::sim::ISimulation* simulation){
//INITIALIZE//
    auto objects = objProvideUnit->objectListData();
    for (const auto & object : objects){
        objects_map[object->numericalID] = object;
    }
  }
  

  void step(prescan::sim::ISimulation* simulation, prescan::sim::ISimulationModel * simmodel){
//STEP//
  }

  void terminate(prescan::sim::ISimulation* simulation){
//TERMINATE//
  }

  const PRESCAN_OBJECTLISTPROVIDERDATA* get_object_from_ID(int NumericID){
    auto it = objects_map.find(NumericID);
    if (it != objects_map.end()){
        return objects_map[NumericID];
    }
    return &not_found;
  }

private:
    //Add SelfUnit properties
    prescan::sim::ObjectListProviderUnit* objProvideUnit{ nullptr };
    std::map<int, const PRESCAN_OBJECTLISTPROVIDERDATA*> objects_map{};
    PRESCAN_OBJECTLISTPROVIDERDATA not_found{};
};

