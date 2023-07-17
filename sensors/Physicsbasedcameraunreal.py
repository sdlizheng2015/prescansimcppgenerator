#!/usr/bin
# -*- coding: utf-8 -*-
# @Time    : 7/17/2023 2:09 PM
# @Team    : Siemens Prescan SLS
# @Author  : Yi Yang
# @Support : prescansls.sisw@siemens.com

from rename_api_namespace.prescan_python_dmapi import *
from sensors.MetaSensor import Sensor

__load_module__ = ["Physicsbasedcameraunreal"]


class Physicsbasedcameraunreal(Sensor):
    name = Sensor.Physicsbasedcameraunreal
    getSensorsAPI = prescan_api_physicsbasedcameraunreal.getAttachedPhysicsBasedCameraUnrealSensors
    pimp_model = "pimp/pbcucameramodel"

    def __init__(self, pbc: prescan_api_physicsbasedcameraunreal.PhysicsBasedCameraUnrealSensor,
                 xp: prescan_api_experiment.Experiment, xp_yaml: dict):
        super().__init__()
        self.xp = xp
        self.xp_yaml = xp_yaml
        self.pbc = pbc
        self.outputImages = False
        self.outputDistance = False
        self.outputImageSegmentation = False
        self.outputOpticalFlow = False
        self.outputIlluminance = False

        self.parse()

    def parse(self):
        for i in range(100):
            name = self.xp.getString(Physicsbasedcameraunreal.pimp_model,
                                     f"worldViewerCameraList/worldViewerCamera:{i}/cameraId")
            if name == self.pbc.name:
                self.outputImages = bool(self.xp.getBool(Physicsbasedcameraunreal.pimp_model,
                                                         f"worldViewerCameraList/worldViewerCamera:{i}/outputImages"))
                self.outputDistance = bool(self.xp.getBool(Physicsbasedcameraunreal.pimp_model,
                                                           f"worldViewerCameraList/worldViewerCamera:{i}/outputDistance"))
                self.outputImageSegmentation = bool(self.xp.getBool(Physicsbasedcameraunreal.pimp_model,
                                                                    f"worldViewerCameraList/worldViewerCamera:{i}/outputImageSegmentation"))
                self.outputOpticalFlow = bool(self.xp.getBool(Physicsbasedcameraunreal.pimp_model,
                                                              f"worldViewerCameraList/worldViewerCamera:{i}/outputOpticalFlow"))
                self.outputIlluminance = bool(self.xp.getBool(Physicsbasedcameraunreal.pimp_model,
                                                              f"worldViewerCameraList/worldViewerCamera:{i}/outputIlluminance"))
                break

    def __str__(self):
        return f"    {self.pbc.name}: \n" \
               f"      outputImages: {self.outputImages}\n" \
               f"      outputDistance: {self.outputDistance}\n" \
               f"      outputOpticalFlow: {self.outputOpticalFlow}\n" \
               f"      outputImageSegmentation: {self.outputImageSegmentation}\n" \
               f"      outputIlluminance: {self.outputIlluminance}\n"
