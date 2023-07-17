#!/usr/bin
# -*- coding: utf-8 -*-
# @Time    : 7/17/2023 2:09 PM
# @Team    : Siemens Prescan SLS
# @Author  : Yi Yang
# @Support : prescansls.sisw@siemens.com

from sensors.Trajectory import Trajectory
from sensors.MetaSensor import Sensor
from ObjectsSensorsParser import ObjectSensors
from rename_api_namespace.simcpp_api import *
from generator.MetaGenerator import Generator, Include

__load_module__ = ["TrajectoryGenerator"]


class TrajectoryGenerator(Generator):
    sensorName = Sensor.Trajectory

    class SensorInclude(Include):
        def __init__(self, dst: str):
            super().__init__(dst)
            self.include_trajectory_dependency = False

        def generate_codes(self, _object: ObjectSensors):
            if len(_object.objectSensors[TrajectoryGenerator.sensorName]) > 0 and not self.include_trajectory_dependency:
                self.includes += trajectory_incl
                self.include_trajectory_dependency = True

    def __init__(self):
        super().__init__()

    def generate_codes(self, _object: ObjectSensors):
        pathUnit_prefix = f"pathUnit_{_object.ps_object.name}"
        speedUnit_prefix = f"speedUnit_{_object.ps_object.name}"
        object_var = f"obj_{_object.ps_object.name}"
        for _trj in _object.objectSensors[TrajectoryGenerator.sensorName]:  # type: Trajectory
            self.properties += f"{self.space4}//Add Trajectory properties\n"
            self.properties += f"{self.space4}{PathUnit}* {pathUnit_prefix}_{_trj.trj.name}" + "{nullptr};\n"
            self.properties += f"{self.space4}{SpeedProfileUnit}* {speedUnit_prefix}_{_trj.trj.name}" + "{nullptr};\n"
            self.properties += "\n"

            self.registerUnits += f"{self.space4}//register Trajectory units\n"
            trj = f"trj_{_object.ps_object.name}"
            self.registerUnits += f"{self.space4}auto {trj} = {getActiveTrajectory}({object_var});\n"
            self.registerUnits += f"{self.space4}{pathUnit_prefix}_{_trj.trj.name} = " \
                                  f"{registerPathUnit}(simulation, {trj}.path(), {object_var});\n"
            self.registerUnits += f"{self.space4}{speedUnit_prefix}_{_trj.trj.name} = " \
                                  f"{registerSpeedProfileUnit}(simulation, {trj}.speedProfile());\n"
            self.registerUnits += "\n"

            self.updates += f"{self.space4}//update Trajectory states\n"
            self.updates += f"{self.space4}pathUnit_{_object.ps_object.name}_{_trj.trj.name}->motionInput() = " \
                            f"speedUnit_{_object.ps_object.name}_{_trj.trj.name}->motionOutput();\n"
            self.updates += f"{self.space4}stateUnit_{_object.ps_object.name}->stateActuatorInput() = " \
                            f"pathUnit_{_object.ps_object.name}_{_trj.trj.name}->stateActuatorOutput();"
            self.has_trajectory = True
