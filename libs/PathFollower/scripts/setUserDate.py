import prescan.api
import prescan.api.experiment
import prescan.api.roads
import prescan.api.scenario
import prescan.api.trajectory
import prescan.api.opendrive
import prescan.api.userdata
import prescan.api.viewer
import sys
import argparse
from getSingleCollisionPoint import getSingleCollisionPoint

_argparse = argparse.ArgumentParser()
_argparse.add_argument("pb", type=str, help="Prescan pb file")
_argparse.add_argument("ego_name", type=str, help="Ego vehcile name")
_argparse.add_argument("ego_spd", type=float, help="Ego speed [m/s]")
_argparse.add_argument("tar_name", type=str, help="target name")
_argparse.add_argument("tar_spd", type=float, help="target speed [m/s]")


if __name__ == '__main__':
    args = _argparse.parse_args()
    pbName = args.pb
    xp = prescan.api.experiment.loadExperimentFromFile(pbName)
    userData = prescan.api.userdata.getUserData(xp)

    ego_name = args.ego_name
    ego_speed = args.ego_spd

    target_speed = args.tar_spd
    target_name = args.tar_name

    ego_trigger_distance = "EgoTriggerDistance"

    valid, ego_trg_d, ttc, x_c, y_c = getSingleCollisionPoint(pbName, ego_name, target_name, ego_speed, target_speed)
    if not valid:
        print("Get collision invalid...")
        sys.exit(1)

    try:
        element_intensity: prescan.api.userdata.UserDataElementBase = userData.getElementByName(ego_trigger_distance)
    except Exception as ee:
        pass
    else:
        element_intensity.remove()
    userData.createDouble(ego_trigger_distance, ego_trg_d)

    try:
        target = xp.getObjectByName(target_name)
    except Exception as ee:
        print("no such target")
    else:
        target_trj = prescan.api.trajectory.getActiveTrajectory(target)
        target_trj.speedProfile.setConstantSpeed(target_speed)

    """NOTE: When used in HEEDS, replace pbName with 'SimInput.pb'"""
    xp.saveToFile(pbName)
