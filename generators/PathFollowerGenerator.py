#!/usr/bin
# -*- coding: utf-8 -*-
# @Time    : 10/11/2023 2:09 PM
# @Team    : Siemens Prescan SLS
# @Author  : Yi Yang
# @Support : prescansls.sisw@siemens.com

from sensors.PathFollower import PathFollower, DriverSensor
from sensors.MetaSensor import Sensor
from ObjectsSensorsParser import ObjectSensors
from rename_api_namespace.simcpp_api import *
from generators.MetaGenerator import Generator, Include
from rename_api_namespace.prescan_python_dmapi import *

__load_module__ = ["PathFollowerGenerator"]


class PathFollowerGenerator(Generator):
    sensorName = Sensor.PathFollower

    class SensorInclude(Include):
        def __init__(self, dst: str):
            super().__init__(dst)
            self.include_driver_dependency = False

        def generate_codes(self, _object: ObjectSensors):
            if len(_object.objectSensors[PathFollowerGenerator.sensorName]) > 0 and \
                    not self.include_driver_dependency and _object.has_follower \
                    and prescan_api_trajectory.hasActiveTrajectory(_object.ps_object) \
                    and prescan_api_vehicledynamics.hasAttachedAmesimPreconfiguredDynamics(_object.ps_object):
                self.includes += driver_incl
                self.include_driver_dependency = True

    def __init__(self):
        super().__init__()

    def generate_codes(self, _object: ObjectSensors):
        """Check if this object has follower and Amesim and Trajectory"""
        if _object.has_follower \
                and prescan_api_trajectory.hasActiveTrajectory(_object.ps_object) \
                and prescan_api_vehicledynamics.hasAttachedAmesimPreconfiguredDynamics(_object.ps_object):
            for _ in _object.objectSensors[PathFollowerGenerator.sensorName]:  # type: DriverSensor
                self.properties += f"{self.space4}//Define PathFollower\n"
                self.properties += f"{self.space4}prescan::sim::AmesimDynamicsPathFollowerUnit pathFollower;\n"
                self.properties += f"{self.space4}prescan::api::Preview::types::Coordinate position;\n"
                self.properties += f"{self.space4}prescan::api::Preview::types::VehicleControl driverOut;\n"

                self.registerUnits += f"{self.space4}//register follower\n"
                self.registerUnits += f'''{self.space4}pathFollower.registerUnit(experiment, simulation->getSimulationPath() + "/simcpp/PathFollower/conf/", "{_object.ps_object.name}");\n'''

                self.steps += f'''{self.space4}//run path follower\n'''
                self.steps += f'''{self.space4}position.X = dynamicsUnit_{_object.ps_object.name}->stateActuatorOutput().PositionX;\n'''
                self.steps += f'''{self.space4}position.Y = dynamicsUnit_{_object.ps_object.name}->stateActuatorOutput().PositionY;\n'''
                self.steps += f'''{self.space4}position.HD = dynamicsUnit_{_object.ps_object.name}->stateActuatorOutput().OrientationYaw;\n'''

                self.steps += f'''{self.space4}auto vel_x = dynamicsUnit_{_object.ps_object.name}->stateActuatorOutput().VelocityX;\n'''
                self.steps += f'''{self.space4}auto vel_y = dynamicsUnit_{_object.ps_object.name}->stateActuatorOutput().VelocityY;\n'''
                self.steps += f'''{self.space4}auto vel = sqrt(pow(vel_x, 2) + pow(vel_y, 2));\n'''

                self.steps += f'''{self.space4}pathFollower.Preview(position, simulation->getSampleTime(), vel, driverOut);\n'''
                self.steps += f'''{self.space4}dynamicsUnit_{_object.ps_object.name}->vehicleControlInput().Gear = driverOut.Gear;\n'''
                self.steps += f'''{self.space4}dynamicsUnit_{_object.ps_object.name}->vehicleControlInput().Throttle = driverOut.Throttle;\n'''
                self.steps += f'''{self.space4}dynamicsUnit_{_object.ps_object.name}->vehicleControlInput().Brake = driverOut.Brake;\n'''
                self.steps += f'''{self.space4}dynamicsUnit_{_object.ps_object.name}->vehicleControlInput().SteeringWheelAngle = driverOut.SteeringWheelAngle;\n'''

                self.steps += "\n"
                self.properties += "\n"
                self.constructor += "\n"
                self.registerUnits += "\n"
