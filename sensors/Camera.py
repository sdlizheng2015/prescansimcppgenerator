from rename_api_namespace.prescan_python_dmapi import *
from sensors.MetaSensor import Sensor

__load_module__ = ["Camera"]


class Camera(Sensor):
    name = Sensor.Camera
    getSensorsAPI = prescan_api_camera.getAttachedCameraSensors
    pimp_model = "pimp/cameramodel"
    ImageFormat = {0: "MonochromeU8", 1: "SimulinkColorbuffersU8", 2: "BGRU8"}

    def __init__(self, camera: prescan_api_camera.CameraSensor, xp: prescan_api_experiment.Experiment, xp_yaml: dict):
        super().__init__()
        self.xp = xp
        self.xp_yaml = xp_yaml
        self.camera = camera
        self.format: prescan_api_camera.ImageFormat = prescan_api_camera.ImageFormat.SimulinkColorbuffersU8
        self.outputCameraImage = False
        self.parse()

    def parse(self):
        self.format = self.camera.imageFormat
        for i in range(100):
            name = self.xp.getString(Camera.pimp_model, f"sensor:{i}/sensorBase/name")
            if name == self.camera.name:
                self.outputCameraImage = bool(self.xp.getBool(Camera.pimp_model, f"sensor:{i}/outputCameraImage"))
                break

    def __str__(self):
        return f"    {self.camera.name}: \n" \
               f"      outputCameraImage: {self.outputCameraImage}\n" \
               f"      format: {Camera.ImageFormat[self.format]}\n"
