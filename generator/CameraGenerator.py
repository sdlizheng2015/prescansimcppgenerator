from sensors.Camera import Camera
from sensors.MetaSensor import Sensor
from ObjectParser import GeneratorObject
from rename_api_namespace.simcpp_api import *
from generator.MetaGenerator import Generator, Include
from rename_api_namespace.prescan_python_dmapi import *
import shutil
from logger.UserLog import uniLog
from prescan.internal import PrescanException

__load_module__ = ["CameraGenerator"]


class CameraGenerator(Generator):
    sensorName = Sensor.Camera

    class SensorInclude(Include):
        def __init__(self, dst: str):
            super().__init__(dst)
            self.include_camera_SimulinkU8 = False
            self.include_camera_dependency = False
            self.include_camera_BGRU8 = False

        def generate_codes(self, _object: GeneratorObject):
            if len(_object.objectSensors[CameraGenerator.sensorName]) > 0 and not self.include_camera_dependency:
                self.includes += camera_incl
                self.include_camera_dependency = True

            if not (self.include_camera_BGRU8 and self.include_camera_SimulinkU8):
                for _camera in _object.objectSensors[CameraGenerator.sensorName]:  # type: Camera
                    if _camera.format == prescan_api_camera.ImageFormat.SimulinkColorbuffersU8 and not self.include_camera_SimulinkU8:
                        self.sensorDemux += f'''{sensorDemux_incl_prefix}_cameraSimulinkU8.h"\n'''
                        shutil.copy(f"{sensorDemux_file_prefix}_cameraSimulinkU8.h",
                                    self.dst + "/simmodel/sensorsdemux")
                        self.include_camera_SimulinkU8 = True
                    elif _camera.format == prescan_api_camera.ImageFormat.BGRU8 and not self.include_camera_BGRU8:
                        self.sensorDemux += f'''{sensorDemux_incl_prefix}_cameraBGRU8.h"\n'''
                        shutil.copy(f"{sensorDemux_file_prefix}_cameraBGRU8.h",
                                    self.dst + "/simmodel/sensorsdemux")
                        self.include_camera_BGRU8 = True
                    elif _camera.format == prescan_api_camera.ImageFormat.MonochromeU8:
                        uniLog.logger.error("Unsupported MonochromeU8 camera format for #inlcude")
                        raise PrescanException("Unsupported MonochromeU8 camera format for #inlcude")
                    else:
                        pass

    def __init__(self):
        super().__init__()

    def generate_codes(self, _object: GeneratorObject):
        for camera in _object.objectSensors[CameraGenerator.sensorName]:  # type: Camera
            cameraSensor_prefix = f"cameraSensor_{_object.ps_object.name}"
            cameraUnit_prefix = f"cameraUnit_{_object.ps_object.name}"
            if camera.outputCameraImage:
                self.properties += f"{self.space4}//Add Camera sensor properties\n"
                self.properties += f"{self.space4}{CameraSensor} {cameraSensor_prefix}_{camera.camera.name};\n"
                self.properties += f"{self.space4}{CameraSensorUnit}* {cameraUnit_prefix}_{camera.camera.name}" + "{nullptr};\n"

                self.constructor += f"{self.space4}//Construct Camera sensor properties\n"

                self.registerUnits += f"{self.space4}//register Camera sensor units\n"
                self.registerUnits += f"{self.space4}{cameraSensor_prefix}_{camera.camera.name} = " \
                                      f"{getCameraSensor}(\"{camera.camera.name}\");\n"
                self.registerUnits += f"{self.space4}{cameraUnit_prefix}_{camera.camera.name} = " \
                                      f"{registerCameraSensorUnit}(simulation, {cameraSensor_prefix}_{camera.camera.name});\n"

                self.steps += f"{self.space4}//demux camera sensor outputs\n"
                if camera.format == prescan_api_camera.ImageFormat.SimulinkColorbuffersU8:
                    # output properties and constructors
                    self.properties += f"{self.space4}{uint8_t_vector_ptr} {cameraUnit_prefix}_{camera.camera.name}_R;\n"
                    self.properties += f"{self.space4}{uint8_t_vector_ptr} {cameraUnit_prefix}_{camera.camera.name}_G;\n"
                    self.properties += f"{self.space4}{uint8_t_vector_ptr} {cameraUnit_prefix}_{camera.camera.name}_B;\n"
                    self.constructor += f"{self.space4}{cameraUnit_prefix}_{camera.camera.name}_R = {uint8_t_vector_ptr_make};\n"
                    self.constructor += f"{self.space4}{cameraUnit_prefix}_{camera.camera.name}_G = {uint8_t_vector_ptr_make};\n"
                    self.constructor += f"{self.space4}{cameraUnit_prefix}_{camera.camera.name}_B = {uint8_t_vector_ptr_make};\n"
                    func_len = len(f"{self.space4}{sensorDemux}::demux_cameraSimulinkU8(")
                    func_space = " " * 6
                    self.steps += f"{self.space4}{sensorDemux}::demux_cameraSimulinkU8(\n" \
                                  f"{func_space}{cameraUnit_prefix}_{camera.camera.name},\n" \
                                  f"{func_space}//Demux:\n" \
                                  f"{func_space}Terminator, // ->R\n" \
                                  f"{func_space}Terminator, // ->G\n" \
                                  f"{func_space}Terminator); // ->B\n"
                elif camera.format == prescan_api_camera.ImageFormat.BGRU8:
                    # output properties and constructors
                    self.properties += f"{self.space4}{uchar_vector_ptr} {cameraUnit_prefix}_{camera.camera.name}_BGRU8;\n"
                    self.constructor += f"{self.space4}{cameraUnit_prefix}_{camera.camera.name}_BGRU8 = {uchar_vector_ptr_make};\n"
                    func_len = len(f"{self.space4}{sensorDemux}::demux_cameraBGRU8(")
                    func_space = " " * 6
                    self.steps += f"{self.space4}{sensorDemux}::demux_cameraBGRU8(\n" \
                                  f"{func_space}{cameraUnit_prefix}_{camera.camera.name},\n" \
                                  f"{func_space}//Demux:\n" \
                                  f"{func_space}Terminator); // -> BGRU8\n"
                else:
                    pass
                self.constructor += "\n"
                self.registerUnits += "\n"
                self.properties += "\n"
                self.steps += "\n"
