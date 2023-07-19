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



class StepTimeInfo{
public:
  explicit StepTimeInfo(){
//CONSTRUCT//
}
  virtual ~StepTimeInfo(){
//DECONSTRUCT//
}

  void initialize() {
    start_time = prescan::utils::get_milliseconds_timestamp();
    last_time = start_time;
}

  void step_start_time(prescan::sim::ISimulation* simulation){
    start_time = prescan::utils::get_milliseconds_timestamp();
    double simulation_time = simulation->getSampleTime();
    std::cout<< std::fixed << std::setprecision(3) << " =============== Simulation time: "
             << simulation_time << "[s] ===============" <<std::endl;
}

  void step_end_time(){
    end_time = prescan::utils::get_milliseconds_timestamp();
    std::cout <<"Step time: ";
    std::cout << std::fixed << std::setprecision(3) << std::setw(7) << std::setfill('0') << std::left << end_time - start_time;
    std::cout << "[s], Cilic time: ";
    std::cout << std::fixed << std::setprecision(3) << std::setw(7) << std::setfill('0') << std::left << start_time - last_time;
    std::cout << "[s]" << std::endl << std::endl;
    last_time = start_time;
}


public:
    double start_time, last_time, end_time;
};

