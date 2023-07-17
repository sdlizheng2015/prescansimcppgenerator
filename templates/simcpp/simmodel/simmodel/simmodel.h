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

#include "utils/threadpool.h"
#include "utils/cnpy.h"

#include "prescan/api/Experiment.hpp"
#include "prescan/sim/ISimulation.hpp"
#include "prescan/sim/ISimulationModel.hpp"
#include "prescan/sim/Simulation.hpp"
//INCLUDES//


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
//REGISTER//
}

  void updateState(){
//UPDATE//
}
  
  void initialize(prescan::sim::ISimulation* simulation) override{
//INITIALIZE//
    updateState();
}

  void step(prescan::sim::ISimulation* simulation) override{
    //double simulation_time = prescan::utils::get_milliseconds_timestamp();
    double simulation_time = simulation->getSampleTime();
    std::cout<< std::fixed << std::setprecision(3) <<"Simulation time: " << simulation_time << std::endl;

//STEP//

    updateState();
}

  void terminate(prescan::sim::ISimulation* simulation) override{
//TERMINATE//
}

public:
//PROPERTIES//

};

