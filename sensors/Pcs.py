from rename_api_namespace.prescan_python_dmapi import *
from sensors.MetaSensor import Sensor

__load_module__ = ["Pcs"]


class Pcs(Sensor):
    name = Sensor.Pcs
    getSensorsAPI = prescan_api_pcs.getAttachedPcsSensors
    pimp_model = "pimp/cameramodel"

    class OutputMode:
        Range = "range"
        SensorPosition = "sensorPosition"
        WorldPosition = "worldPosition"

    class OutChannels:
        outputDopplerVelocity = "outputDopplerVelocity"
        outputIntensity = "outputIntensity"
        outputObjectID = "outputObjectID"
        outputAngles = "outputAngles"
        integerOutput = "integerOutput"

    def __init__(self, pcs: prescan_api_pcs.PcsSensor, xp: prescan_api_experiment.Experiment, xp_yaml: dict):
        super().__init__()
        self.xp = xp
        self.xp_yaml = xp_yaml
        self.pcs = pcs
        self.outputMode = Pcs.OutputMode.Range
        self.outputDopplerVelocity = False
        self.outputIntensity = False
        self.outputObjectID = False
        self.outputAngles = False
        self.integerOutput = False
        self.parse()

    def parse(self):
        for i in range(100):
            name = self.xp.getString(Pcs.pimp_model, f"sensor:{i}/sensorBase/name")
            if name == self.pcs.name:
                """getString not work for this parameter now"""
                # self.outputMode = self.xp.getString(Pcs.pimp_model, f"sensor:{i}/pointCloudSensor/sensorOutputMode")
                self.outputMode = self.xp_yaml["sensor"][i]["pointCloudSensor"]["sensorOutputMode"]
                self.integerOutput = bool(self.xp.getBool(Pcs.pimp_model, f"sensor:{i}/pointCloudSensor/integerOutput"))
                if self.integerOutput:
                    raise Exception("integerOutput is not supported")
                self.outputIntensity = bool(
                    self.xp.getBool(Pcs.pimp_model, f"sensor:{i}/pointCloudSensor/outputIntensity"))
                self.outputObjectID = bool(
                    self.xp.getBool(Pcs.pimp_model, f"sensor:{i}/pointCloudSensor/outputObjectID"))
                self.outputDopplerVelocity = bool(
                    self.xp.getBool(Pcs.pimp_model, f"sensor:{i}/pointCloudSensor/outputDopplerVelocity"))
                self.outputAngles = bool(self.xp.getBool(Pcs.pimp_model, f"sensor:{i}/pointCloudSensor/outputAngles"))
                break

    def __str__(self):
        return f"    {self.pcs.name}: \n" \
               f"      outputMode: {self.outputMode}\n" \
               f"      integerOutput: {self.integerOutput}\n" \
               f"      outputIntensity: {self.outputIntensity}\n" \
               f"      outputObjectID: {self.outputObjectID}\n" \
               f"      outputDopplerVelocity: {self.outputDopplerVelocity}\n" \
               f"      outputAngles: {self.outputAngles}\n"
