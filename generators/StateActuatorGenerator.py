#!/usr/bin
# -*- coding: utf-8 -*-
# @Time    : 7/17/2023 2:09 PM
# @Team    : Siemens Prescan SLS
# @Author  : Yi Yang
# @Support : prescansls.sisw@siemens.com

from ObjectsSensorsParser import ObjectSensors
from sensors.MetaSensor import Sensor
from rename_api_namespace.simcpp_api import *
from generators.MetaGenerator import Generator, Include

__load_module__ = ["StateActuatorGenerator"]


class StateActuatorGenerator(Generator):
    sensorName = Sensor.StateActuator

    class SensorInclude(Include):
        def __init__(self, dst: str):
            super().__init__(dst)
            self.include_state_actuator_dependency = False

        def generate_codes(self, _object: ObjectSensors):
            if not self.include_state_actuator_dependency:
                self.includes += state_incl
                self.include_state_actuator_dependency = True

    def __init__(self):
        super().__init__()

    def generate_codes(self, _object: ObjectSensors):
        stateUnit_prefix = f"stateUnit_{_object.ps_object.name}"
        object_var = f"obj_{_object.ps_object.name}"

        self.properties += f"{self.space4}//Add StateActuator properties\n"
        self.properties += f"{self.space4}{StateActuatorUnit}* {stateUnit_prefix}" + "{nullptr};\n"
        self.properties += "\n"

        self.registerUnits += f"{self.space4}//register StateActuator units\n"
        self.registerUnits += f"{self.space4}{stateUnit_prefix} = {registerStateActuatorUnit}(simulation, {object_var});\n"
        self.registerUnits += "\n"
