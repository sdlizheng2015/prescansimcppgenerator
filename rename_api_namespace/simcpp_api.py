type_incl = '''#include "prescan/api/Types.hpp"\n#include "prescan/sim/SelfSensorUnit.hpp"\n'''
camera_incl = '''#include "prescan/api/Camera.hpp"\n#include "prescan/sim/CameraSensorUnit.hpp"\n'''
pcs_incl = '''#include "prescan/api/Pcs.hpp"\n#include "prescan/sim/PcsSensorUnit.hpp"\n'''
pcl_incl = '''#include "prescan/api/Pointcloudlidar.hpp"\n#include "prescan/sim/PointCloudLidarSensorUnit.hpp"\n#include "prescan/sim/SignalView.hpp"\n#include "prescan/sim/ISimulationModel.hpp"\n'''
trajectory_incl = '''#include "prescan/api/Trajectory.hpp"\n#include "prescan/sim/StateActuatorUnit.hpp"\n#include "prescan/sim/SpeedProfileUnit.hpp"\n#include "prescan/sim/PathUnit.hpp"\n'''
air_incl = '''#include "prescan/api/Air.hpp"\n#include "prescan/sim/AirSensorUnit.hpp"\n'''
pbc_incl = '''#include "prescan/api/Physicsbasedcameraunreal.hpp"\n#include "prescan/sim/PhysicsBasedCameraUnrealUnit.hpp"\n#include "prescan/sim/PhysicsBasedCameraUnrealControlUnit.hpp"\n#include "prescan/sim/PhysicsBasedCameraUnrealExposureUnit.hpp"\n#include "prescan/sim/PhysicsBasedCameraUnrealRainOverlayUnit.hpp"\n'''
fwl_incl = '''#include "prescan/api/Fullwaveformlidar.hpp"\n#include "prescan/sim/FullWaveformLidarSensorUnit.hpp"\n#include <prescan/sim/SignalView.hpp>\n#include "prescan/sim/ISimulationModel.hpp"\n'''
amesim_incl = '''#include "prescan/api/Vehicledynamics.hpp"\n#include "prescan/sim/AmesimPreconfiguredDynamicsUnit.hpp"\n'''
state_incl = '''#include "prescan/sim/StateActuatorUnit.hpp"\n'''

SelfSensorUnit = "prescan::sim::SelfSensorUnit"
AirSensor = "prescan::api::air::AirSensor"
AirSensorUnit = "prescan::sim::AirSensorUnit"
PcsSensor = "prescan::api::pcs::PcsSensor"
PcsSensorUnit = "prescan::sim::PcsSensorUnit"
CameraSensor = "prescan::api::camera::CameraSensor"
CameraSensorUnit = "prescan::sim::CameraSensorUnit"
PointCloudLidarSensor = "prescan::api::pointcloudlidar::PointCloudLidarSensor"
PointCloudUnit = "prescan::sim::pointcloudlidar::PointCloudUnit"
PointCloudObjectIdUnit = "prescan::sim::pointcloudlidar::ObjectIdUnit"
PointCloudPowerUnit = "prescan::sim::pointcloudlidar::PowerUnit"
PhysicsBasedCameraUnrealSensor = "prescan::api::physicsbasedcameraunreal::PhysicsBasedCameraUnrealSensor"
PhysicsBasedCameraUnrealRGBUnit = "prescan::sim::PhysicsBasedCameraUnrealRGBUnit"
FullWaveformLidarSensor = "prescan::api::fullwaveformlidar::FullWaveformLidarSensor"
FwlPointCloudUnit = "prescan::sim::fullwaveformlidar::PointCloudUnit"
FwlBeamUnit = "prescan::sim::fullwaveformlidar::BeamUnit"
FwlOutputInfoUnit = "prescan::sim::fullwaveformlidar::OutputInfoUnit"
FwlPowerUnit = "prescan::sim::fullwaveformlidar::PowerUnit"
PathUnit = "prescan::sim::PathUnit"
SpeedProfileUnit = "prescan::sim::SpeedProfileUnit"
AmesimVehicleDynamicsUnit = "prescan::sim::AmesimVehicleDynamicsUnit"
StateActuatorUnit = "prescan::sim::StateActuatorUnit"

double_vector_ptr = "std::shared_ptr<std::vector<double>>"
double_vector_ptr_make = "std::make_shared<std::vector<double>>()"

float_vector_ptr = "std::shared_ptr<std::vector<float>>"
float_vector_ptr_make = "std::make_shared<std::vector<float>>()"

uint32_t_vector_ptr = "std::shared_ptr<std::vector<uint32_t>>"
uint32_t_vector_ptr_make = "std::make_shared<std::vector<uint32_t>>()"

int32_t_vector_ptr = "std::shared_ptr<std::vector<int32_t>>"
int32_t_vector_ptr_make = "std::make_shared<std::vector<int32_t>>()"

uint8_t_vector_ptr = "std::shared_ptr<std::vector<uint8_t>>"
uint8_t_vector_ptr_make = "std::make_shared<std::vector<uint8_t>>()"

uchar_vector_ptr = "std::shared_ptr<std::vector<unsigned char>>"
uchar_vector_ptr_make = "std::make_shared<std::vector<unsigned char>>()"

fwl_beam_vector_ptr = "std::shared_ptr<std::vector<std::vector<PRESCAN_FULLWAVEFORMLIDAR_BEAM_DATA>>>"
fwl_beam_vector_ptr_make = "std::make_shared<std::vector<std::vector<PRESCAN_FULLWAVEFORMLIDAR_BEAM_DATA>>>()"

fwl_info_vector_ptr = "std::shared_ptr<std::vector<PRESCAN_FULLWAVEFORMLIDAR_OUTPUT_INFO_DATA>>"
fwl_info_vector_ptr_make = "std::make_shared<std::vector<PRESCAN_FULLWAVEFORMLIDAR_OUTPUT_INFO_DATA>>()"

sensorDemux = "prescan::sensorDemux"
sensorDemux_incl_prefix = '''#include "sensorsdemux/demux'''
sensorDemux_file_prefix = "./templates/sensorsdemux/demux"

getWorldObject = "experiment.getObjectByName<prescan::api::types::WorldObject>"
registerSelfSensorUnit = "prescan::sim::registerUnit<prescan::sim::SelfSensorUnit>"
getAirSensor = "experiment.getObjectByName<prescan::api::air::AirSensor>"
registerAirSensorUnit = "prescan::sim::registerUnit<prescan::sim::AirSensorUnit>"
getPcsSensor = "experiment.getObjectByName<prescan::api::pcs::PcsSensor>"
registerPcsSensorUnit = "prescan::sim::registerUnit<prescan::sim::PcsSensorUnit>"
getCameraSensor = "experiment.getObjectByName<prescan::api::camera::CameraSensor>"
registerCameraSensorUnit = "prescan::sim::registerUnit<prescan::sim::CameraSensorUnit>"
getPointCloudLidarSensor = "experiment.getObjectByName<prescan::api::pointcloudlidar::PointCloudLidarSensor>"
registerPointCloudUnit = "prescan::sim::registerUnit<prescan::sim::pointcloudlidar::PointCloudUnit>"
registerPointCloudObjectIdUnit = "prescan::sim::registerUnit<prescan::sim::pointcloudlidar::ObjectIdUnit>"
registerPointCloudPowerUnit = "prescan::sim::registerUnit<prescan::sim::pointcloudlidar::PowerUnit>"
getPhysicsBasedCameraUnrealSensor = "experiment.getObjectByName<prescan::api::physicsbasedcameraunreal::PhysicsBasedCameraUnrealSensor>"
registerPhysicsBasedCameraUnrealRGBUnit = "prescan::sim::registerUnit<prescan::sim::PhysicsBasedCameraUnrealRGBUnit>"
getFullWaveformLidarSensor = "experiment.getObjectByName<prescan::api::fullwaveformlidar::FullWaveformLidarSensor>"
registerFwlPointCloudUnit = "prescan::sim::registerUnit<prescan::sim::fullwaveformlidar::PointCloudUnit>"
registerFwlBeamUnit = "prescan::sim::registerUnit<prescan::sim::fullwaveformlidar::BeamUnit>"
registerFwlOutputInfoUnit = "prescan::sim::registerUnit<prescan::sim::fullwaveformlidar::OutputInfoUnit>"
registerFwlPowerUnit = "prescan::sim::registerUnit<prescan::sim::fullwaveformlidar::PowerUnit>"
getActiveTrajectory = "prescan::api::trajectory::getActiveTrajectory"
registerPathUnit = "prescan::sim::registerUnit<prescan::sim::PathUnit>"
registerSpeedProfileUnit = "prescan::sim::registerUnit<prescan::sim::SpeedProfileUnit>"
getAttachedAmesimPreconfiguredDynamics = "prescan::api::vehicledynamics::getAttachedAmesimPreconfiguredDynamics"
registerAmesimVehicleDynamicsUnit = "prescan::sim::registerUnit<prescan::sim::AmesimVehicleDynamicsUnit>"
registerStateActuatorUnit = "prescan::sim::registerUnit<prescan::sim::StateActuatorUnit>"
