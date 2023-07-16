from rename_api_namespace.prescan_python_dmapi import *
from sensors.MetaSensor import Sensor

__load_module__ = ["Pointcloudlidar"]


class Pointcloudlidar(Sensor):
    name = Sensor.Pointcloudlidar
    getSensorsAPI = prescan_api_pointcloudlidar.getAttachedPointCloudLidarSensors
    pimp_model = "pointcloudlidarmodel"

    def __init__(self, pcl: prescan_api_pointcloudlidar.PointCloudLidarSensor,
                 xp: prescan_api_experiment.Experiment, xp_yaml: dict):
        super().__init__()
        self.xp = xp
        self.xp_yaml = xp_yaml
        self.pcl = pcl
        self.enablePowerCalculation = False
        self.parse()

    def parse(self):
        self.enablePowerCalculation = self.pcl.powerCalculation

    def __str__(self):
        return f"    {self.pcl.name}: \n" \
               f"      enablePowerCalculation: {self.enablePowerCalculation}\n"
