#!/usr/bin
# -*- coding: utf-8 -*-
# @Time    : 7/17/2023 2:09 PM
# @Team    : Siemens Prescan SLS
# @Author  : Yi Yang
# @Support : prescansls.sisw@siemens.com

from sensors.AmesimDynamics import AmesimDynamics
from sensors.MetaSensor import Sensor
from ObjectsSensorsParser import ObjectSensors
from rename_api_namespace.simcpp_api import *
from generator.MetaGenerator import Generator, Include
import shutil

__load_module__ = ["AmesimDynamicsGenerator"]


class AmesimDynamicsGenerator(Generator):
    sensorName = Sensor.AmesimDynamics

    class SensorInclude(Include):
        def __init__(self, dst: str):
            super().__init__(dst)
            self.include_amesim_dependency = False

        def generate_codes(self, _object: ObjectSensors):
            if len(_object.objectSensors[
                       AmesimDynamicsGenerator.sensorName]) > 0 and not self.include_amesim_dependency:
                self.includes += amesim_incl
                self.sensorDemux += f'''{sensorDemux_incl_prefix}_amesim.h"\n'''
                shutil.copy(f"{sensorDemux_file_prefix}_amesim.h", self.dst + "/simmodel/sensorsdemux")
                self.include_amesim_dependency = True

    def __init__(self):
        super().__init__()

    def generate_codes(self, _object: ObjectSensors):
        dynamicsUnit_prefix = f"dynamicsUnit_{_object.ps_object.name}"
        object_var = f"obj_{_object.ps_object.name}"
        for _ in _object.objectSensors[AmesimDynamicsGenerator.sensorName]:  # type: AmesimDynamics
            self.properties += f"{self.space4}//Add Amesim properties\n"
            self.properties += f"{self.space4}{AmesimVehicleDynamicsUnit}* {dynamicsUnit_prefix}" + "{nullptr};\n"

            dynamics = f"dynamics_{_object.ps_object.name}"
            self.registerUnits += f"{self.space4}//register Amesim units\n"
            self.registerUnits += f"{self.space4}auto {dynamics} = {getAttachedAmesimPreconfiguredDynamics}({object_var});\n"
            self.registerUnits += f"{self.space4}{dynamicsUnit_prefix} = {registerAmesimVehicleDynamicsUnit}(simulation, {dynamics}, \"./\");\n"

            self.updates += f"{self.space4}//update Amesim states\n"
            self.updates += f"{self.space4}stateUnit_{_object.ps_object.name}->stateActuatorInput() = " \
                            f"dynamicsUnit_{_object.ps_object.name}->stateActuatorOutput();\n"

            self.steps += f"{self.space4}//For Amesim dynamics, please use '{dynamicsUnit_prefix}' directly\n"
            # self.steps += f"{self.space4}{sensorDemux}::demux_amesim({dynamicsUnit_prefix});\n"

            self.has_amesim = True

            self.properties += "\n"
            self.updates += "\n"
            self.registerUnits += "\n"
            self.steps += "\n"
