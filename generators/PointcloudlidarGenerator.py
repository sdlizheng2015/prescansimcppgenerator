#!/usr/bin
# -*- coding: utf-8 -*-
# @Time    : 7/17/2023 2:09 PM
# @Team    : Siemens Prescan SLS
# @Author  : Yi Yang
# @Support : prescansls.sisw@siemens.com

from sensors.Pointcloudlidar import Pointcloudlidar
from sensors.MetaSensor import Sensor
from ObjectsSensorsParser import ObjectSensors
from rename_api_namespace.simcpp_api import *
from generators.MetaGenerator import Generator, Include
import shutil

__load_module__ = ["PointcloudlidarGenerator"]


class PointcloudlidarGenerator(Generator):
    sensorName = Sensor.Pointcloudlidar

    class SensorInclude(Include):
        def __init__(self, dst: str):
            super().__init__(dst)
            self.include_pcl_dependency = False

        def generate_codes(self, _object: ObjectSensors):
            if len(_object.objectSensors[PointcloudlidarGenerator.sensorName]) > 0 and not self.include_pcl_dependency:
                self.includes += pcl_incl
                self.sensorDemux += f'''{sensorDemux_incl_prefix}_pcl.h"\n'''
                shutil.copy(f"{sensorDemux_file_prefix}_pcl.h", self.dst + "/simmodel/sensorsdemux")
                self.include_pcl_dependency = True

    def __init__(self):
        super().__init__()

    def generate_codes(self, _object: ObjectSensors):
        pclSensor_prefix = f"pclSensor_{_object.ps_object.name}"
        pclPointUnit_prefix = f"pclPointUnit_{_object.ps_object.name}"
        pclObjectIdUnit_prefix = f"pclObjectIdUnit_{_object.ps_object.name}"
        pclPowerUnit_prefix = f"pclPowerUnit_{_object.ps_object.name}"
        for pcl in _object.objectSensors[PointcloudlidarGenerator.sensorName]:  # type: Pointcloudlidar
            self.properties += f"{self.space4}//Add PCL sensor properties\n"
            self.properties += f"{self.space4}{PointCloudLidarSensor} {pclSensor_prefix}_{pcl.pcl.name};\n"
            self.properties += f"{self.space4}{PointCloudUnit}* {pclPointUnit_prefix}_{pcl.pcl.name}" + "{nullptr};\n"
            self.properties += f"{self.space4}{PointCloudObjectIdUnit}* {pclObjectIdUnit_prefix}_{pcl.pcl.name}" + "{nullptr};\n"
            if pcl.enablePowerCalculation:
                self.properties += f"{self.space4}{PointCloudPowerUnit}* {pclPowerUnit_prefix}_{pcl.pcl.name}" + "{nullptr};\n"

            # output properties
            self.properties += f"{self.space4}{float_vector_ptr} {pclPointUnit_prefix}_{pcl.pcl.name}_X;\n"
            self.properties += f"{self.space4}{float_vector_ptr} {pclPointUnit_prefix}_{pcl.pcl.name}_Y;\n"
            self.properties += f"{self.space4}{float_vector_ptr} {pclPointUnit_prefix}_{pcl.pcl.name}_Z;\n"
            if pcl.enablePowerCalculation:
                self.properties += f"{self.space4}{float_vector_ptr} {pclPowerUnit_prefix}_{pcl.pcl.name}_I;\n"
            self.properties += f"{self.space4}{int32_t_vector_ptr} {pclObjectIdUnit_prefix}_{pcl.pcl.name}_ID;\n"

            # output constructors
            self.constructor += f"{self.space4}//Construct PCL sensor properties\n"
            self.constructor += f"{self.space4}{pclPointUnit_prefix}_{pcl.pcl.name}_X = {float_vector_ptr_make};\n"
            self.constructor += f"{self.space4}{pclPointUnit_prefix}_{pcl.pcl.name}_Y = {float_vector_ptr_make};\n"
            self.constructor += f"{self.space4}{pclPointUnit_prefix}_{pcl.pcl.name}_Z = {float_vector_ptr_make};\n"
            if pcl.enablePowerCalculation:
                self.constructor += f"{self.space4}{pclPowerUnit_prefix}_{pcl.pcl.name}_I = {float_vector_ptr_make};\n"
            self.constructor += f"{self.space4}{pclObjectIdUnit_prefix}_{pcl.pcl.name}_ID = {int32_t_vector_ptr_make};\n"

            self.registerUnits += f"{self.space4}//register PCL sensor units\n"
            self.registerUnits += f"{self.space4}{pclSensor_prefix}_{pcl.pcl.name} = {getPointCloudLidarSensor}(\"{pcl.pcl.name}\");\n"
            self.registerUnits += f"{self.space4}{pclPointUnit_prefix}_{pcl.pcl.name} = " \
                                  f"{registerPointCloudUnit}(simulation, {pclSensor_prefix}_{pcl.pcl.name});\n"
            self.registerUnits += f"{self.space4}{pclObjectIdUnit_prefix}_{pcl.pcl.name} = " \
                                  f"{registerPointCloudObjectIdUnit}(simulation, {pclSensor_prefix}_{pcl.pcl.name});\n"
            if pcl.enablePowerCalculation:
                self.registerUnits += f"{self.space4}{pclPowerUnit_prefix}_{pcl.pcl.name} = " \
                                      f"{registerPointCloudPowerUnit}(simulation, {pclSensor_prefix}_{pcl.pcl.name});\n"

            self.steps += f"{self.space4}//demux PCL sensor outputs\n"
            func_space = " " * 6
            port_X = f"{pclPointUnit_prefix}_{pcl.pcl.name}_X" \
                if _object.enable_all_ports else Generator.Terminator
            port_Y = f"{pclPointUnit_prefix}_{pcl.pcl.name}_Y" \
                if _object.enable_all_ports else Generator.Terminator
            port_Z = f"{pclPointUnit_prefix}_{pcl.pcl.name}_Z" \
                if _object.enable_all_ports else Generator.Terminator
            port_ID = f"{pclObjectIdUnit_prefix}_{pcl.pcl.name}_ID" \
                if _object.enable_all_ports else Generator.Terminator
            port_I = f"{pclPowerUnit_prefix}_{pcl.pcl.name}_I" \
                if (_object.enable_all_ports and pcl.enablePowerCalculation) else Generator.Terminator
            pclPowerUnit = f"{pclPowerUnit_prefix}_{pcl.pcl.name}" if pcl.enablePowerCalculation else "nullptr"

            self.steps += f"{self.space4}{sensorDemux}::demux_pcl(\n" \
                          f"{func_space}simmodel,\n" \
                          f"{func_space}{pclSensor_prefix}_{pcl.pcl.name},\n" \
                          f"{func_space}{pclPointUnit_prefix}_{pcl.pcl.name},\n" \
                          f"{func_space}{pclObjectIdUnit_prefix}_{pcl.pcl.name},\n" \
                          f"{func_space}{pclPowerUnit}, // DON'T EDIT\n" \
                          f"{func_space}//Demux:\n" \
                          f"{func_space}{port_X}, // ->X (valid)\n" \
                          f"{func_space}{port_Y}, // ->Y (valid)\n" \
                          f"{func_space}{port_Z}, // ->Z (valid)\n" \
                          f"{func_space}{port_I}, // ->I {'(valid)' if pcl.enablePowerCalculation else '(invalid)'}\n" \
                          f"{func_space}{port_ID}); // ->ID (valid)\n"

            self.steps += "\n"
            self.properties += "\n"
            self.registerUnits += "\n"
            self.constructor += "\n"
