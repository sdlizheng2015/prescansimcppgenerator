# Date: 10/13/2022,8:19 PM
# Author: OldYoung
# Project: PostProcess

import os



import prescan.api.experiment
import prescan.api.types
from typing import Union

pimp_model = "pimp/worldmodel"


class PreScanWorldObject:
    name = "name"
    objectTypeName = "objectTypename"
    objectTypeID = "objectTypeID"
    numericalID = "numericalID"
    subType = "subType"
    description = "description"
    boundingBox = "boundingBox"
    boundingBox_x = "boundingBox_x"
    boundingBox_y = "boundingBox_y"
    boundingBox_z = "boundingBox_z"
    cobbOffset_x = "cobbOffset_x"
    cobbOffset_y = "cobbOffset_y"
    cobbOffset_z = "cobbOffset_z"
    cogOffset_x = "cogOffset_x"
    cogOffset_y = "cogOffset_y"
    cogOffset_z = "cogOffset_z"


_objectInfo = {}
experiment: Union[prescan.api.experiment.Experiment, None] = None


def load_objectInfo(pb_path: str = "./SimInput.pb"):
    """
    TODO: You must call this func first if you want to use this module
    :param pb_path:
    :return:
    """
    global _objectInfo, experiment
    experiment = prescan.api.experiment.loadExperimentFromFile(pb_path)
    for index, worldObject in enumerate(experiment.objects):  # type: prescan.api.types.WorldObject
        name = worldObject.name
        objectTypeName = experiment.getString(pimp_model, f"object:{index}/objectTypeName")
        description = experiment.getString(pimp_model, f"object:{index}/description")
        subType = experiment.getString(pimp_model, f"object:{index}/subType")
        objectTypeID = experiment.getInt32(pimp_model, f"object:{index}/objectTypeID")
        numericalID = experiment.getInt32(pimp_model, f"object:{index}/numericalID")

        boundingBox_x = experiment.getDouble(pimp_model, f"object:{index}/boundingSize/x")
        boundingBox_y = experiment.getDouble(pimp_model, f"object:{index}/boundingSize/y")
        boundingBox_z = experiment.getDouble(pimp_model, f"object:{index}/boundingSize/z")

        cobbOffset_x = experiment.getDouble(pimp_model, f"object:{index}/cobbOffset/x")
        cobbOffset_y = experiment.getDouble(pimp_model, f"object:{index}/cobbOffset/y")
        cobbOffset_z = experiment.getDouble(pimp_model, f"object:{index}/cobbOffset/z")

        cogOffset_x = experiment.getDouble(pimp_model, f"object:{index}/cogOffset/x")
        cogOffset_y = experiment.getDouble(pimp_model, f"object:{index}/cogOffset/y")
        cogOffset_z = experiment.getDouble(pimp_model, f"object:{index}/cogOffset/z")

        _objectInfo.setdefault(numericalID,
                               {PreScanWorldObject.name: name,
                                PreScanWorldObject.objectTypeID: objectTypeID,
                                PreScanWorldObject.description: description,
                                PreScanWorldObject.subType: subType,
                                PreScanWorldObject.objectTypeName: objectTypeName,
                                PreScanWorldObject.boundingBox:
                                    {PreScanWorldObject.boundingBox_x: boundingBox_x,
                                     PreScanWorldObject.boundingBox_y: boundingBox_y,
                                     PreScanWorldObject.boundingBox_z: boundingBox_z,
                                     PreScanWorldObject.cobbOffset_x: cobbOffset_x,
                                     PreScanWorldObject.cobbOffset_y: cobbOffset_y,
                                     PreScanWorldObject.cobbOffset_z: cobbOffset_z,
                                     PreScanWorldObject.cogOffset_x: cogOffset_x,
                                     PreScanWorldObject.cogOffset_y: cogOffset_y,
                                     PreScanWorldObject.cogOffset_z: cogOffset_z,
                                     }
                                })


def get_num_id_info(num_id: int):
    """
    TODO: from id get object info
    :param num_id:
    :return:
    """
    global _objectInfo
    assert num_id in _objectInfo.keys(), \
        f"Object with NumID {num_id} not found in current PreScan experiment"
    return _objectInfo[num_id]


def get_ego_num_id(ego_name: str) -> int:
    """
    TODO: from name get object id
    :param ego_name:
    :return:
    """
    global _objectInfo
    for key in _objectInfo.keys():
        if _objectInfo[key][PreScanWorldObject.name] == ego_name:
            return key
        raise Exception("Can not find EGO vehicle")


def get_sim_frequency():
    global experiment
    return experiment.scheduler.simulationFrequency
