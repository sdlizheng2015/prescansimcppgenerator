#!/usr/bin
# -*- coding: utf-8 -*-
# @Time    : 7/17/2023 2:09 PM
# @Team    : Siemens Prescan SLS
# @Author  : Yi Yang
# @Support : prescansls.sisw@siemens.com

from rename_api_namespace.prescan_python_dmapi import *
import sys
from prescan.internal import PrescanException
import yaml
from logger import uniLog
from typing import List
from sensors.MetaSensor import Sensor
from utils.load_modules import get_cls
from typing import Dict


class ObjectSensors:
    def __init__(self, ps_object: prescan_api_types.WorldObject):
        self.ps_object: prescan_api_types.WorldObject = ps_object
        sensor_names = [os.path.splitext(module)[0] for module in os.listdir("./sensors") if
                        os.path.splitext(module)[1] == ".py"]
        sensors_cls, _ = get_cls(sensor_names, "sensors")
        self.sensors_cls: Dict[str, callable(Sensor)] = {sensor.name: sensor for sensor in sensors_cls}
        self.objectSensors: Dict[str, List[Sensor]] = {sensor.name: [] for sensor in sensors_cls}

    def _objects_info(self) -> str:
        info = ""
        for sensor_name, sensors in self.objectSensors.items():
            info += f"  {sensor_name} num: {len(sensors)}\n"
            for sensor in sensors:
                info += f"{str(sensor)}\n"
                info = info[0:-1]
        return info[0:-1] if info else info

    def __str__(self):
        return f"{self.ps_object.name} \n" \
               f"{self._objects_info()}"


class ObjectsSensorsParser:
    def __init__(self, xp: prescan_api_experiment.Experiment, xp_yaml: dict, load_yaml: bool):
        self._objects_sensors: List[ObjectSensors] = []
        self._xp_yaml = xp_yaml
        self._xp = xp
        self._parse_objects()
        self._register_sensors()

    @property
    def ParsedObjectsSensors(self):
        return self._objects_sensors

    def _parse_objects(self):
        for _object in self._xp.objects:
            self._objects_sensors.append(ObjectSensors(_object))

    def _register_sensors(self):
        for _object in self._objects_sensors:  # type: ObjectSensors
            for sensor_name, sensor_cls in _object.sensors_cls.items():
                try:
                    sensors = sensor_cls.getSensorsAPI(_object.ps_object)
                except PrescanException as ee:
                    uniLog.logger.warn(self.__class__.__name__ + ": " + str(ee))
                else:
                    _object.objectSensors[sensor_name].extend(
                        [sensor_cls(sensor, self._xp, self._xp_yaml) for sensor in sensors])
