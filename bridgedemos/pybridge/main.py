import mmap
from bridge_msgs.basic_msgs.header_pb2 import Header
from bridge_msgs.basic_msgs.geometry_pb2 import *
from bridge_msgs.chassis_msgs.chassis_pb2 import Chassis
from bridge_msgs.control_msgs.control_cmd_pb2 import ControlCommand
from bridge_msgs.perception_msgs.obstacle_pb2 import *
from bridge_msgs.localiztion_msgs.localization_pb2 import *
from google.protobuf.message import Message
from shmtypes.bridge_shm import shmHeader, ShmHandler, HandlerManager

import threading

change = False
import matplotlib.pyplot as plt
import numpy as np


def change_direction():
    global change
    if input("input:\n"):
        change = True


if __name__ == '__main__':
    # thread = threading.Thread(target=change_direction, args=(), kwargs={}, daemon=True)
    # thread.start()

    syncHandler = ShmHandler("Sync", 1024)
    syncHeader: Message = Header()

    localizationHandler = ShmHandler("Localization", 1024)
    location: Message = LocalizationStatus()

    chassisHandler = ShmHandler("Chassis", 1024)
    chassis: Message = Chassis()

    obstaclesHandler = ShmHandler("Obstacles", 1024)
    obstacles: Message = Obstacles()

    polyLanesHandler = ShmHandler("PolyLanes", 1024 * 5)
    polyLanes: Message = PolyLanes()

    pointLanesHandler = ShmHandler("PointLanes", 1024 * 10)
    pointLanes: Message = PointLanes()

    controlHandler = ShmHandler("Control", 1024)
    control: Message = ControlCommand()

    handlerManger = HandlerManager()
    handlerManger.add_handler(syncHandler)
    handlerManger.add_handler(localizationHandler)
    handlerManger.add_handler(chassisHandler)
    handlerManger.add_handler(obstaclesHandler)
    handlerManger.add_handler(polyLanesHandler)
    handlerManger.add_handler(controlHandler)
    handlerManger.add_handler(pointLanesHandler)

    handlerManger.initial_shmhandlers(True)

    while True:
        syncHandler.read_shm(shmHeader, syncHeader, 0.1, True)
        # print(syncHeader)
        if syncHeader.timestamp_sec < 0:
            break
        localizationHandler.read_shm(shmHeader, location, 0.1, True)
        # print(location)
        chassisHandler.read_shm(shmHeader, chassis, 0.1, True)
        # print(chassis)
        obstaclesHandler.read_shm(shmHeader, obstacles, 0.1, True)
        # print(obstacles)
        polyLanesHandler.read_shm(shmHeader, polyLanes, 0.1, True)
        # print(polyLanes)
        pointLanesHandler.read_shm(shmHeader, pointLanes, 0.1, True)
        #print(pointLanes)
        """============== Add your algorithm here =============="""
        plt.clf()
        plt.xlim(-10, 10)
        plt.ylim(-10, 60)
        plt.axis("equal")
        for pointLane in pointLanes.lane:  # type: PointLane
            X = []
            Y = []
            for point in pointLane.point:
                X.append(point.x)
                Y.append(point.y)
            plt.scatter(X, Y, marker='o', s=10, color='black')

        for polyLane in polyLanes.lane:  # type: PolyLane
            distance_i = np.linspace(0, polyLane.length, 10)
            X_i = polyLane.X.N0 + polyLane.X.N1 * distance_i + polyLane.X.N2 * distance_i ** 2 + polyLane.X.N3 * distance_i ** 3
            Y_i = polyLane.Y.N0 + polyLane.Y.N1 * distance_i + polyLane.Y.N2 * distance_i ** 2 + polyLane.Y.N3 * distance_i ** 3
            plt.plot(X_i, Y_i)
        for obstacle in obstacles.perception_obstacle:
            x = obstacle.position.x
            y = obstacle.position.y
            plt.scatter(x, y, marker='o', s=50, color='blue')
        plt.scatter(0, 0, marker='o', s=50, color='red')
        plt.pause(0.001)

        """====================================================="""
        control.brake = 0
        control.throttle = 1
        control.steering = 0

        if change:
            control.gear_location = Chassis.GearPosition.GEAR_DRIVE
        else:
            control.gear_location = Chassis.GearPosition.GEAR_REVERSE
        """====================================================="""


        controlHandler.write_shm(shmHeader, control, 0.1, True)
        # print(control)
    handlerManger.free_shmhandlers()
