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
#include "utils/cnpy.h"

#include "prescan/api/Experiment.hpp"
#include "prescan/sim/ISimulation.hpp"
#include "prescan/sim/ISimulationModel.hpp"
#include "prescan/sim/Simulation.hpp"
#include "utils/printsteptime.h"
#include "worldobjects/objectsprovider.h"
#include "worldobjects/DAF_95_XF.h"
#include "worldobjects/MercedesBenz.h"

#include "bridge_base/bridge_model.hpp"

class SimModel : public prescan::sim::ISimulationModel {
public:
  explicit SimModel(){
//CONSTRUCT//
}
  virtual ~SimModel(){
//DECONSTRUCT//
}

  void registerSimulationUnits(const prescan::api::experiment::Experiment& experiment,
                               prescan::sim::ISimulation* simulation) override{
    //Register ObjectsProvider
    objectsProvider.registerSimulationUnits(experiment, simulation);
    //Register units for DAF_95_XF
    ps_DAF_95_XF.registerSimulationUnits(experiment, simulation);
    //Register units for MercedesBenz
    ps_MercedesBenz.registerSimulationUnits(experiment, simulation);
}

  void updateState(){
    //Update states for DAF_95_XF
    ps_DAF_95_XF.updateState();
    //Update states for MercedesBenz
    ps_MercedesBenz.updateState();
}
  
  void initialize(prescan::sim::ISimulation* simulation) override{
    updateState();
    stepTimeInfo.initialize();
    //Initialize ObjectsProvider
    objectsProvider.initialize(simulation);
    //use 'objectsProvider.get_object_from_ID(NumericID)'
	
	bridge.initialize();
}

  void step(prescan::sim::ISimulation* simulation) override{
    stepTimeInfo.step_start_time(simulation);

    //Demux sensors for DAF_95_XF
    ps_DAF_95_XF.step(simulation, this);
    //Demux sensors for MercedesBenz
    ps_MercedesBenz.step(simulation, this);
	
	bridge.step(objectsProvider, ps_DAF_95_XF, ps_MercedesBenz);

    updateState();

    stepTimeInfo.step_end_time();
}

  void terminate(prescan::sim::ISimulation* simulation) override{
//TERMINATE//
bridge.terminate();
}

public:
    //To print simulation step time info;
    StepTimeInfo stepTimeInfo;
    //Create ObjectListProvider
    ObjectsProvider objectsProvider;
    //Define properties for DAF_95_XF
    DAF_95_XF ps_DAF_95_XF;
    //Define properties for MercedesBenz
    MercedesBenz ps_MercedesBenz;
//#ADDBRIDGEMODE#//
    PrescanGeneralBridgeModel bridge{10, 0.01, true};
};

