#pragma once

#include "bridge_base/bridge_shm.hpp"
#include "bridge_base/bridge_base.hpp"

#include "bridge_msgs/basic_msgs/header.pb.h"
#include "bridge_msgs/basic_msgs/header.pb.h"
#include "bridge_msgs/chassis_msgs/chassis.pb.h"
#include "bridge_msgs/control_msgs/control_cmd.pb.h"
#include "bridge_msgs/localiztion_msgs/localization.pb.h"
#include "bridge_msgs/perception_msgs/obstacle.pb.h"

#include "bridge_map/map_control.hpp"
#include "bridge_map/map_chassis.hpp"
#include "bridge_map/map_obstacles.hpp"
#include "bridge_map/map_localization.hpp"
#include "bridge_map/map_syncheader.hpp"
#include "bridge_map/map_polylanes.hpp"
#include "bridge_map/map_pointlanes.hpp"

using namespace std;
using namespace prescan::perception;
using namespace prescan::common;
using namespace prescan::localization;
using namespace prescan::control;
using namespace prescan::chassis;
using namespace prescan::mapping;

//#ADDOBJECTS#//

#include "worldobjects/DAF_95_XF.h"
#include "worldobjects/MercedesBenz.h"
#include "worldobjects/objectsprovider.h"

class PrescanGeneralBridgeModel{

public:
  explicit PrescanGeneralBridgeModel(int poolNum, double timeout, bool block): 
                                    thread_pool(poolNum), _timeout(timeout), 
                                    _block(block){

  }

  void initialize(){
    // // *************************initial shmhandler************************
    thread_pool.start();
    hander_manager.add_handler(&syncHandler);
    hander_manager.add_handler(&localizationHandler);
    hander_manager.add_handler(&obstaclesHandler);
    hander_manager.add_handler(&polyLanesHandler);
    hander_manager.add_handler(&pointLanesHandler);
    hander_manager.add_handler(&chassisHandler);
    hander_manager.add_handler(&controlHandler);
    hander_manager.initial_shmhandlers(false);
    //************************************************************************

  }

  void step(ObjectsProvider &objectsProvider, 
            DAF_95_XF &ps_DAF_95_XF, 
            MercedesBenz &ps_MercedesBenz){
    auto timestamp = get_microseconds_timestamp();

    /* ============================ Send ================================= */
    start_sync(syncheader, timestamp, syncHandler, _timeout, _block);

    thread_pool.appendTask(std::bind(map_chassis, 
                                     std::ref(chassis), 
                                     ps_DAF_95_XF.dynamicsUnit_DAF_95_XF,
                                     std::ref(chassisHandler),
                                     timestamp, 
                                     _timeout, 
                                     _block));
    thread_pool.appendTask(std::bind(map_localization, 
                                     std::ref(localization), 
                                     ps_DAF_95_XF.dynamicsUnit_DAF_95_XF,
                                     ps_DAF_95_XF.selfUnit_DAF_95_XF,
                                     std::ref(localizationHandler),
                                     timestamp, 
                                     _timeout, 
                                     _block));
    thread_pool.appendTask(std::bind(map_obstacles, 
                                  std::ref(obstacles),
                                  ps_DAF_95_XF.selfUnit_DAF_95_XF,
                                  ps_DAF_95_XF.airUnit_DAF_95_XF_AIR_1_Range,
                                  ps_DAF_95_XF.airUnit_DAF_95_XF_AIR_1_Theta,
                                  ps_DAF_95_XF.airUnit_DAF_95_XF_AIR_1_Phi,
                                  ps_DAF_95_XF.airUnit_DAF_95_XF_AIR_1_ID,
                                  ps_DAF_95_XF.airUnit_DAF_95_XF_AIR_1_Velocity,
                                  ps_DAF_95_XF.airUnit_DAF_95_XF_AIR_1_Heading,
                                  std::ref(objectsProvider),
                                  std::ref(obstaclesHandler),
                                  timestamp, 
                                  _timeout, 
                                  _block));
    thread_pool.appendTask(std::bind(map_polylanes, 
                                  std::ref(polyLanes),
                                  ps_DAF_95_XF.almsUnit_DAF_95_XF_ALMS_1_Polylines,
                                  std::ref(polyLanesHandler),
                                  timestamp, 
                                  _timeout, 
                                  _block));
    thread_pool.appendTask(std::bind(map_pointlanes, 
                                  std::ref(pointLanes),
                                  ps_DAF_95_XF.lmsUnit_DAF_95_XF_LMSSensor_1_Lines,
                                  std::ref(pointLanesHandler),
                                  timestamp, 
                                  _timeout, 
                                  _block));  
    thread_pool.wait();
    /* ============================ Read ================================= */
    thread_pool.appendTask(std::bind(map_control, 
                                  std::ref(control),
                                  ps_DAF_95_XF.dynamicsUnit_DAF_95_XF,
                                  std::ref(controlHandler),
                                  timestamp, 
                                  _timeout, 
                                  _block));
    thread_pool.wait();
  }

  void terminate(){
    //********************** Stop sham and free shmhandler *******************
    terminate_sync(syncheader, 0, syncHandler, _timeout, _block);
    hander_manager.free_shmhandlers();
    thread_pool.stop();
    //*************************************************************************
   }

private:

  ThreadPool thread_pool;
  double _timeout;
  bool _block;
  prescan::bridge::HandlerManager hander_manager;

  /*
  Here must run the simulation first to determine the buffer size
  for each shared memory. If the actual data size exceeds the buffer, 
  the shm read and protobuf parse will have error.
  */

  prescan::bridge::ShmHandler syncHandler{"Sync", 1024};
  Header syncheader;

  prescan::bridge::ShmHandler chassisHandler{"Chassis", 1024};
  Chassis chassis;

  prescan::bridge::ShmHandler localizationHandler{"Localization", 1024};
  LocalizationStatus localization;

  prescan::bridge::ShmHandler obstaclesHandler{"Obstacles", 1024};
  Obstacles obstacles;

  prescan::bridge::ShmHandler polyLanesHandler{"PolyLanes", 1024 * 5};
  PolyLanes polyLanes;

  prescan::bridge::ShmHandler pointLanesHandler{"PointLanes", 1024 * 10};
  PointLanes pointLanes;
  
  prescan::bridge::ShmHandler controlHandler{"Control", 1024};
  ControlCommand control;

};
