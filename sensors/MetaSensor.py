#!/usr/bin
# -*- coding: utf-8 -*-
# @Time    : 7/17/2023 2:09 PM
# @Team    : Siemens Prescan SLS
# @Author  : Yi Yang
# @Support : prescansls.sisw@siemens.com

from abc import abstractmethod, ABCMeta

__load_module__ = []


class Sensor(metaclass=ABCMeta):
    getSensorsAPI = lambda ps_object: []
    """
    This is the abstract class for sensor types.
    Register sensor name here, which must be the same as sensor class name
    """

    Air = "Air"
    AmesimDynamics = "AmesimDynamics"
    Camera = "Camera"
    Fullwaveformlidar = "Fullwaveformlidar"
    Pcs = "Pcs"
    Physicsbasedcameraunreal = "Physicsbasedcameraunreal"
    Pointcloudlidar = "Pointcloudlidar"
    SelfUnit = "SelfUnit"  # not a real sensor
    StateActuator = "StateActuator"  # not a real sensor
    Trajectory = "Trajectory"
    Lms = "Lms"
    Alms = "Alms"
    Depthcamera = "Depthcamera"
    Brs = "Brs"
    Iss = "Iss"
    PathFollower = "PathFollower"  # not a real sensor

    NotRealSensors = [SelfUnit, StateActuator, PathFollower]

    def __init__(self):
        ...

    @abstractmethod
    def parse(self):
        """
        TODO: parse the configurations for each PreScan sensor from pb file
        :return:
        """
        ...
