# Date: 10/26/2022,1:45 PM
# Author: OldYoung
# Project: results

class EgoVector:
    OrientationYaw = 0
    PositionX = 1
    PositionY = 2
    PositionZ = 3
    Velocity = 4


class AIRVector:
    simTime = 0
    ID = 1
    Range = 2
    Azimuth = 3
    Elevation = 4
    Heading = 5
    SingleObjMessages = 6


class DetectResults:
    NoFrame = "NoFrame"
    NoObject = "NoObject"


class SyncError(Exception):
    def __init__(self):
        super(SyncError, self).__init__()

    def __str__(self):
        return "Not find sync object"


if __name__ == '__main__':
    try:
        raise SyncError
    except Exception as ee:
        print(ee)
