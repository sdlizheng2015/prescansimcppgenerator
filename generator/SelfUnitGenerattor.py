#!/usr/bin
# -*- coding: utf-8 -*-
# @Time    : 7/17/2023 2:09 PM
# @Team    : Siemens Prescan SLS
# @Author  : Yi Yang
# @Support : prescansls.sisw@siemens.com

from ObjectParser import GeneratorObject
from sensors.MetaSensor import Sensor
from rename_api_namespace.simcpp_api import *
from generator.MetaGenerator import Generator, Include
import shutil

__load_module__ = ["SelfUnitGenerator"]


class SelfUnitGenerator(Generator):
    sensorName = Sensor.SelfUnit

    class SensorInclude(Include):
        def __init__(self, dst: str):
            super().__init__(dst)
            self.include_type_dependency = False

        def generate_codes(self, _object: GeneratorObject):
            if not self.include_type_dependency:
                self.includes += type_incl
                self.sensorDemux += f'''{sensorDemux_incl_prefix}_selfunit.h"\n'''
                shutil.copy(f"{sensorDemux_file_prefix}_selfunit.h", self.dst + "/simmodel/sensorsdemux")
                self.include_type_dependency = True

    def __init__(self):
        super().__init__()

    def generate_codes(self, _object: GeneratorObject):
        object_name_var = f"{_object.ps_object.name}"
        self_unit_var = f"selfUnit_{_object.ps_object.name}"

        self.properties += f"{self.space4}//Add SelfUnit properties\n"
        self.properties += f"{self.space4}const std::string name_{object_name_var}" + "{\"" + f"{_object.ps_object.name}" + "\"};\n"
        self.properties += f"{self.space4}{SelfSensorUnit}* {self_unit_var}" + "{nullptr};\n"

        self.registerUnits += f"{self.space4}//register SelfUnit\n"
        object_var = f"obj_{_object.ps_object.name}"
        self.registerUnits += f"{self.space4}auto {object_var} = {getWorldObject}(\"{object_name_var}\");\n"
        self.registerUnits += f"{self.space4}selfUnit_{_object.ps_object.name} = {registerSelfSensorUnit}(simulation, {object_var});\n"

        self.steps += f"{self.space4}//For SelfUnit, please use '{self_unit_var}' directly\n"
        # self.steps += f"{self.space4}{sensorDemux}::demux_selfState({self_unit_var});\n"

        self.steps += "\n"
        self.properties += "\n"
        self.registerUnits += "\n"
