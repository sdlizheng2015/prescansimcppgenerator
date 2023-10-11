import numpy as np
# import matplotlib.pyplot as plt
from shapely.geometry import LineString
from typing import Tuple
import prescan.api.experiment
import prescan.api.trajectory
from Define import *
from PreScanBox import *

def getSingleCollisionPoint(pb_path: str,
                            ego_name: str,
                            target_name: str,
                            ego_spd: float,
                            tar_spd: float) -> Tuple[bool, float, float, float, float]:
    '''
    getSingleCollisionPoint try to find a geometry intersection point of two
    paths. There should be only one intersection point, if more, you may need
    to pick up the one that you want from all intersection points



    :param ego_trj: ego car trajectory, Prescan.api.trajectory.Trajectory
    :param tar_trj: target car trajectory, Prescan.api.trajectory.Trajectory
    :param ego_spd: ego car speed, double
    :return:
        x_c y_c: collision point x and y
        ttc: time from ego start point to collision point at speed of ego_spd
        tar_std_spd: target speed needed to have a collision with ego
    '''

    experiment = prescan.api.experiment.loadExperimentFromFile(pb_path)
    ego = experiment.getObjectByName(ego_name)
    target = experiment.getObjectByName(target_name)

    ego_trj = prescan.api.trajectory.getActiveTrajectory(ego)
    tar_trj = prescan.api.trajectory.getActiveTrajectory(target)

    ego_length = ego_trj.path.length
    tar_length = tar_trj.path.length

    step = 0.1  # distance space [m]
    ego_LocatePoints = int(ego_length / step) + 1
    ego_TrjData = np.zeros((ego_LocatePoints, 2))

    tar_LocatePoints = int(tar_length / step) + 1
    tar_TrjData = np.zeros((tar_LocatePoints, 2))

    for i in range(0, ego_LocatePoints):
        point = ego_trj.path.poseAtDistance(i * step)
        point_x = point.position.x
        point_y = point.position.y
        ego_TrjData[i, :] = [point_x, point_y]

    for i in range(0, tar_LocatePoints):
        point = tar_trj.path.poseAtDistance(i * step)
        point_x = point.position.x
        point_y = point.position.y
        tar_TrjData[i, :] = [point_x, point_y]

    curve1_points = ego_TrjData
    curve2_points = tar_TrjData

    curve1_line = LineString(curve1_points)
    curve2_line = LineString(curve2_points)

    intersection = curve1_line.intersection(curve2_line)
    print("intersection:", intersection.geom_type)
    intersection_points = []
    if intersection.is_empty:
        print("Empty")
        return False, 0, 0, 0, 0
    elif intersection.geom_type == "Point":
        intersection_points = [intersection.xy]
        x_c = intersection.xy[0][0]
        y_c = intersection.xy[1][0]
        # print(x_c, y_c)
    else:
        return False, 0, 0, 0, 0

    # for point in intersection_points:
    #     plt.plot(*point, 'ro', label='Intersection Point')

    x1 = curve1_points[:, 0]
    y1 = curve1_points[:, 1]
    x2 = curve2_points[:, 0]
    y2 = curve2_points[:, 1]

    ego_n = np.argmin(abs(x1 - x_c))
    tar_n = np.argmin(abs(x2 - x_c))

    load_objectInfo(pb_path)
    ego_id = get_ego_num_id(ego_name)
    ego_info = get_num_id_info(ego_id)
    box_x = ego_info[PreScanWorldObject.boundingBox][PreScanWorldObject.boundingBox_x]
    cobbox = ego_info[PreScanWorldObject.boundingBox][PreScanWorldObject.cobbOffset_x]
    cogbox = ego_info[PreScanWorldObject.boundingBox][PreScanWorldObject.cogOffset_x]
    print(box_x,cobbox,cogbox)

    ego_d = ego_n * step
    tar_d = tar_n * step
    ttc = tar_d / tar_spd
    ego_trg_d = ego_d - ttc * ego_spd - (box_x / 2 + (cobbox - cogbox))
    if ego_trg_d <= 0:
        print("Ego acceleration distance is not enough...")
        return False, 0, 0, 0, 0

    # plt.plot(*curve1_points.T, label='Curve 1')
    # plt.plot(*curve2_points.T, label='Curve 2')
    #
    # plt.xlabel('X')
    # plt.ylabel('Y')
    # plt.legend()
    # plt.grid(True)
    # plt.show()
    print(f"Trigger distance: {ego_trg_d}, x: {x_c}, {y_c}")
    return True, ego_trg_d, ttc, x_c, y_c


if __name__ == '__main__':
    getSingleCollisionPoint(
        r"C:\Users\yiyan5ez\Desktop\prescan-ros2-agri-main\scenarioDatabase\AGCOModelRadarModelSimplifiedTest\AGCOModelRadarModelSimplifiedTest.pb",
        "Ego", "Target1", 10)
