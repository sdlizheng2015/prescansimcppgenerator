#!/usr/bin
# -*- coding: utf-8 -*-
# @Time    : 7/17/2023 2:09 PM
# @Team    : Siemens Prescan SLS
# @Author  : Yi Yang
# @Support : prescansls.sisw@siemens.com

from sensors.Alms import Alms
from sensors.MetaSensor import Sensor
from ObjectsSensorsParser import ObjectSensors
from rename_api_namespace.simcpp_api import *
from generator.MetaGenerator import Generator, Include
import shutil

__load_module__ = ["AlmsGenerator"]


class AlmsGenerator(Generator):
    sensorName = Sensor.Alms

    class SensorInclude(Include):
        def __init__(self, dst: str):
            super().__init__(dst)
            self.include_alms_dependency = False

        def generate_codes(self, _object: ObjectSensors):
            if len(_object.objectSensors[AlmsGenerator.sensorName]) > 0 and not self.include_alms_dependency:
                self.includes += alms_incl
                self.sensorDemux += f'''{sensorDemux_incl_prefix}_alms.h"\n'''
                shutil.copy(f"{sensorDemux_file_prefix}_alms.h", self.dst + "/simmodel/sensorsdemux")
                self.include_alms_dependency = True

    def __init__(self):
        super().__init__()

    def generate_codes(self, _object: ObjectSensors):
        almsSensor_prefix = f"almsSensor_{_object.ps_object.name}"
        almsUnit_prefix = f"almsUnit_{_object.ps_object.name}"

        for alms in _object.objectSensors[AlmsGenerator.sensorName]:  # type: Alms
            self.properties += f"{self.space4}//Add Alms sensor properties\n"
            self.properties += f"{self.space4}{AlmsSensor} {almsSensor_prefix}_{alms.alms.name};\n"
            self.properties += f"{self.space4}{AlmsSensorUnit}* {almsUnit_prefix}_{alms.alms.name}" + "{nullptr};\n"

            # output properties
            self.properties += f"{self.space4}{alms_line_vector_ptr} {almsUnit_prefix}_{alms.alms.name}_Polylines;\n"

            self.constructor += f"{self.space4}//Construct Alms sensor properties\n"
            self.constructor += f"{self.space4}{almsUnit_prefix}_{alms.alms.name}_Polylines = {alms_line_vector_ptr_make};\n"

            self.registerUnits += f"{self.space4}//register Alms sensor units\n"
            self.registerUnits += f"{self.space4}{almsSensor_prefix}_{alms.alms.name} = " \
                                  f"{getAlmsSensor}(\"{alms.alms.name}\");\n"
            self.registerUnits += f"{self.space4}{almsUnit_prefix}_{alms.alms.name} = " \
                                  f"{registerAlmsSensorUnit}(simulation, {almsSensor_prefix}_{alms.alms.name});\n"

            self.steps += f"{self.space4}//demux lms sensor outputs\n"
            func_space = " " * 6

            port_Polylines = f"{almsUnit_prefix}_{alms.alms.name}_Polylines" \
                if _object.enable_all_ports else Generator.Terminator

            self.steps += f"{self.space4}{sensorDemux}::demux_alms(\n" \
                          f"{func_space}{almsUnit_prefix}_{alms.alms.name},\n" \
                          f"{func_space}//Demux:\n" \
                          f"{func_space}{port_Polylines}); // ->Lines (valid)\n"

            self.steps += "\n"
            self.properties += "\n"
            self.constructor += "\n"
            self.registerUnits += "\n"
