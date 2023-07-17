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

#include "prescan/api/Physicsbasedcameraunreal.hpp"
#include "prescan/api/Experiment.hpp"
#include "prescan/sim/Simulation.hpp"
#include "prescan/api/Utils.hpp"
#include "simmodel/simmodel.h"

#include <iostream>
#include <string>

PRESCAN_MAIN() {
  if (argc < 3) {
    std::cerr << "Usage:\n"
              << argv[0] << " <path_to_pb_file> <simulation_duration>" << std::endl
              << "where" << std::endl
              << "    <path_to_pb_file> - full path to experiment pb file" << std::endl
              << "    <simulation_duration> - simulation time in seconds" << std::endl;

    return EXIT_FAILURE;
  }

  const std::string pathToPb = argv[1];
  const double simulationDuration = std::stof(argv[2]); // [seconds]
  
  try{
    prescan::api::experiment::Experiment experiment = prescan::api::experiment::loadExperimentFromFile(pathToPb);
    auto physicsBasedCameraSensors = prescan::api::physicsbasedcameraunreal::getPhysicsBasedCameraUnrealSensors(experiment);
    for (auto & pbc: physicsBasedCameraSensors.asVector()){
      pbc.setSeparateChannelsAndTranspose(false);
    }
    experiment.saveToFile(pathToPb);

    SimModel code;
    prescan::sim::Simulation sim(&code);
    sim.setSimulationPath("./");
    sim.setLogLevel(LogLevelWarning);
    sim.run(experiment, simulationDuration);
  }
  catch (const prescan::internal::PrescanException& e){
		std::cout << e.what() << std::endl;
  }  

  std::cout << "Simulation finished" << std::endl;
  return EXIT_SUCCESS;
}
