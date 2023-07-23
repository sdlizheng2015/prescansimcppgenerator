#!/usr/bin
# -*- coding: utf-8 -*-
# @Time    : 7/17/2023 2:09 PM
# @Team    : Siemens Prescan SLS
# @Author  : Yi Yang
# @Support : prescansls.sisw@siemens.com

import shutil
import os
import sys

from rename_api_namespace.prescan_python_dmapi import *
from prescan.internal import PrescanException
from generator import *
from typing import Dict, List, Tuple
from sensors.MetaSensor import Sensor
from ObjectsSensorsParser import ObjectsSensorsParser, ObjectSensors
from utils.load_modules import get_cls
from generator import SelfUnitGenerator, StateActuatorGenerator
import yaml
from logger.UserLog import uniLog


generator_names = [os.path.splitext(module)[0] for module in os.listdir("./generator") if
                   os.path.splitext(module)[1] == ".py"]
generator_cls, incl_cls = get_cls(generator_names, "generator", sub_cls_name="SensorInclude")
generator_cls.insert(0, generator_cls.pop(generator_cls.index(StateActuatorGenerator)))
generator_cls.insert(0, generator_cls.pop(generator_cls.index(SelfUnitGenerator)))
incl_cls.insert(0, incl_cls.pop(incl_cls.index(StateActuatorGenerator.SensorInclude)))
incl_cls.insert(0, incl_cls.pop(incl_cls.index(SelfUnitGenerator.SensorInclude)))


class SimcppGenerator:
    def __init__(self, pb: str, pb_yaml: str, ps_dir: str,
                 load_yaml: bool = True, enable_all_port: bool = False,
                 enable_sim_time: bool = True):
        self.xp_yaml = {}
        self.ps_dir = ps_dir
        self.dst = ""
        self.includes = ""
        self.properties = ""
        self.registerUnits = ""
        self.updates = ""
        self.steps = ""
        self.terminate = ""
        self.constructor = ""
        self.initialize = ""
        self.step_start = ""
        self.step_end = ""
        self.enable_sim_time = enable_sim_time
        try:
            self.xp: prescan_api_experiment.Experiment = prescan_api_experiment.loadExperimentFromFile(pb)
            if load_yaml:
                with open(pb_yaml, "rt") as file:
                    self.xp_yaml: dict = yaml.load(file, yaml.FullLoader)
        except PrescanException as ee:
            uniLog.logger.error(self.__class__.__name__ + ": " + str(ee))
            raise PrescanException("load pb/yaml file failed")
        else:
            self.objectsSensorsParser = ObjectsSensorsParser(self.xp, self.xp_yaml, load_yaml, enable_all_port)

    def copy_to_project(self, dst: str = "./simcpp"):
        """
        TODO: call this first
        :param dst:
        :return:
        """
        self.dst = dst
        if os.path.exists(dst):
            enter = input("Deleting the exiting project?(y/n):\n")
            if enter in ["y", "Y", "yes"]:
                try:
                    os.rmdir(dst)
                except OSError:
                    shutil.rmtree(dst)
                except Exception as ee:
                    print(ee)
                finally:
                    shutil.copytree("./templates/simcpp", dst)
                    print("deleting the exiting project and copy templates to project")
            else:
                print("not copy templates to project")
                sys.exit()
        else:
            shutil.copytree("./templates/simcpp", dst)
            print(f"copy templates to {self.dst}")

    @staticmethod
    def overwrite_files(files: Dict[str, Dict[str, str]], new_path: str = ""):
        """
        TODO:
        :param files:
        :param new_path: if has, this function should work with only one file
        :return:
        """
        for path, flags_contents in files.items():
            with open(path, "r") as file:
                text = file.readlines()
                for flag, content in flags_contents.items():
                    text[text.index(flag)] = content
            write_to_path = new_path if new_path else path
            with open(write_to_path, "w") as file:
                file.writelines(text)

    def _add_comments(self, _object: ObjectSensors):
        """
        TODO: add comments for simmodel.h file
        :param _object:
        :return:
        """
        propertyComment = f"{Generator.space2}//Define properties for {_object.ps_object.name}\n"
        self.properties += propertyComment

        updatesComment = f"{Generator.space2}//Update states for {_object.ps_object.name}\n"
        self.updates += updatesComment

        registerUnitsComment = f"{Generator.space2}//Register units for {_object.ps_object.name}\n"
        self.registerUnits += registerUnitsComment

        stepsComment = f"{Generator.space2}//Demux sensors for {_object.ps_object.name}\n"
        self.steps += stepsComment

    def _is_object_always_on_sheet(self, _object: ObjectSensors):
        """
        TODO: is actor always generated for GUI config
        :param _object:
        :return:
        """
        for i in range(len(self.xp.objects)):
            if self.xp.getString("pimp/worldmodel", f"object:{i}/name") == _object.ps_object.name:
                return bool(self.xp.getBool("pimp/worldmodel", f"object:{i}/genericModel/alwaysOnCompilationSheet"))
        return False

    def _is_object_to_generate(self, _object: ObjectSensors):
        """
        TODO: generate actor with trajectory or dynamics or always_on_sheet
        :param _object:
        :return:
        """
        if _object.to_generate_code or self._is_object_always_on_sheet(_object):
            return True
        return False

    def generate_codes_with_sim_time(self):
        if self.enable_sim_time:
            self.includes += f'''#include "utils/printsteptime.h"\n'''
            self.properties += f"{Generator.space2}//To print simulation step time info;\n"
            self.properties += f"{Generator.space2}StepTimeInfo stepTimeInfo;\n"
            self.initialize += f"{Generator.space2}stepTimeInfo.initialize();\n"
            self.step_start += f"{Generator.space2}stepTimeInfo.step_start_time(simulation);\n"
            self.step_end += f"{Generator.space2}stepTimeInfo.step_end_time();\n"

    def generate_object_provider(self):
        if self.xp.getBool('pimp/objectlistprovidermodel', 'detectionOn'):
            self.includes += f'''#include "worldobjects/objectsprovider.h"\n'''

            self.properties += f"{Generator.space2}//Create ObjectListProvider\n"
            self.properties += f"{Generator.space2}ObjectsProvider objectsProvider;\n"

            self.registerUnits += f"{Generator.space2}//Register ObjectsProvider\n"
            self.registerUnits += f"{Generator.space2}objectsProvider.registerSimulationUnits(experiment, simulation);\n"

            self.initialize += f"{Generator.space2}//Initialize ObjectsProvider\n"
            self.initialize += f"{Generator.space2}objectsProvider.initialize(simulation);\n"
            self.initialize += f"{Generator.space2}//use 'objectsProvider.get_object_from_ID(NumericID)'\n"

    def generate_codes(self):
        """
        TODO:
        :return:
        """
        self.generate_codes_with_sim_time()
        self.generate_object_provider()
        for _object in self.objectsSensorsParser.ParsedObjectsSensors:  # type: ObjectSensors
            if not self._is_object_to_generate(_object):
                continue
            sensor_demux = ""
            sensor_includes = ""
            properties = ""
            registerUnits = ""
            updates = ""
            steps = ""
            terminate = ""
            constructor = ""
            initialize = ""
            worldObject = _object.ps_object.name
            className = f"class {worldObject}" + "{" + f"\npublic:\n  explicit {worldObject}()" + "{\n"
            deconstructfunc = f"  ~{worldObject}()" + "{\n"

            """generate #include in 'worldobject.h' files"""
            includes: List[Include] = [incl(self.dst) for incl in incl_cls]
            for include in includes:
                include.generate_codes(_object)
                sensor_demux += include.sensorDemux
                sensor_includes += include.includes

            """generate codes in 'worldobject.h' files"""
            generators: Dict[str, Generator] = {generator.sensorName: generator() for generator in generator_cls}
            for generator in generators.values():
                generator.generate_codes(_object)
                properties += generator.properties
                registerUnits += generator.registerUnits
                steps += generator.steps
                constructor += generator.constructor

            """update amesim if has amesim else update trajectory else nothing"""
            if generators[Sensor.AmesimDynamics].has_amesim:
                updates += generators[Sensor.AmesimDynamics].updates + "\n"
            elif generators[Sensor.Trajectory].has_trajectory:
                updates += generators[Sensor.Trajectory].updates + "\n"
            else:
                pass

            """To generate all 'worldobject.h' files"""
            object_model_path = self.dst + "/simmodel/worldobjects/worldobject.h"
            object_model = {
                "//INCLUDES//\n": sensor_includes,
                "//SENSORMAP//\n": sensor_demux,
                "//ClASSNAME//\n": className,
                "//CONSTRUCT//\n": constructor,
                "//REGISTER//\n": registerUnits,
                "//UPDATE//\n": updates,
                "//STEP//\n": steps,
                "//PROPERTIES//\n": properties,
                "//DECONSTRUCTFUNC//\n": deconstructfunc,
            }
            files = {
                object_model_path: object_model,
            }
            self.overwrite_files(files, new_path=self.dst + f"/simmodel/worldobjects/{worldObject}.h")

            """To overwrite simmodel.h file"""
            self._add_comments(_object)
            self.includes += f'''#include "worldobjects/{worldObject}.h"\n'''
            self.properties += f"{Generator.space2}{worldObject} ps_{worldObject};\n"
            self.registerUnits += f"{Generator.space2}ps_{worldObject}.registerSimulationUnits(experiment, simulation);\n"
            self.updates += f"{Generator.space2}ps_{worldObject}.updateState();\n"
            self.steps += f"{Generator.space2}ps_{worldObject}.step(simulation, this);\n"

    def write_to_simmodel(self):
        simmodel_h_path = self.dst + "/simmodel/simmodel/simmodel.h"
        simmodel_h = {
            "//INCLUDES//\n": self.includes,
            "//PROPERTIES//\n": self.properties,
            "//REGISTER//\n": self.registerUnits,
            "//UPDATE//\n": self.updates,
            "//STEP//\n": self.steps,
            "//STEPSTART//\n": self.step_start,
            "//STEPEND//\n": self.step_end,
            "//INITIALIZE//\n": self.initialize,
        }

        cmake_list_path = self.dst + "/CMakeLists.txt"
        cmake_list = {
            "#PRESCAN_DIR#\n": f'''set(Prescan_DIR "{self.ps_dir}")\n''',
        }

        files = {
            simmodel_h_path: simmodel_h,
            #cmake_list_path: cmake_list,
        }

        self.overwrite_files(files)
