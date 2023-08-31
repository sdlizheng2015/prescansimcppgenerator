#!/usr/bin
# -*- coding: utf-8 -*-
# @Time    : 7/17/2023 2:09 PM
# @Team    : Siemens Prescan SLS
# @Author  : Yi Yang
# @Support : prescansls.sisw@siemens.com

from rename_api_namespace.prescan_python_dmapi import *
from sensors.MetaSensor import Sensor

__load_module__ = ["Depthcamera"]


class Depthcamera(Sensor):
    name = Sensor.Depthcamera
    getSensorsAPI = prescan_api_depthcamera.getAttachedDepthCameraSensors
    pimp_model = "pimp/cameramodel"

    def __init__(self, camera: prescan_api_depthcamera.DepthCameraSensor, xp: prescan_api_experiment.Experiment, xp_yaml: dict):
        super().__init__()
        self.xp = xp
        self.xp_yaml = xp_yaml
        self.camera = camera
        self.depthCamFarClipping = 150
        self.outputCameraImage = True
        self.parse()

    def parse(self):
        for i in range(100):
            name = self.xp.getString(Depthcamera.pimp_model, f"sensor:{i}/sensorBase/name")
            if name == self.camera.name:
                self.outputCameraImage = bool(self.xp.getBool(Depthcamera.pimp_model, f"sensor:{i}/outputCameraImage"))
                self.depthCamFarClipping = bool(self.xp.getDouble(Depthcamera.pimp_model, f"sensor:{i}/depthCamera/depthCamFarClipping"))
                break
        if not self.outputCameraImage:
            raise Exception(f"Camera: {self.camera.name} output is not enabled")

    def __str__(self):
        return f"    {self.camera.name}: \n" \
               f"      outputCameraImage: {self.outputCameraImage}\n" \
               f"      FarClipping: {self.depthCamFarClipping}[m]\n"
