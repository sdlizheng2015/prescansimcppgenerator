#pragma once


#include <iostream>
#include <math.h>
#include <cmath>


double sign(double value){
  return value >= 0 ? 1.0 : -1.0;
}


double change_heading_range(double yaw, bool positive = true){
  /*
  TODO: 将航向加转化为 0 ~ 360 deg 或 -180 ~ 180 deg
  :param yaw: 输入航向角 [deg]
  :param positive:
      1: [0, 360]
      0: [-180, 180]
  :return:
  */
  int circle;
  if (yaw == 0.0){
    circle = 0;
  }else{
    circle = int(yaw / (sign(yaw) * 360.0));
  }
  auto heading = yaw - sign(yaw) * circle * 360.0;
  if (heading < 0){
    heading = heading + 360.0;
  }
  if (positive){
    return heading;
  }else{
    return heading > 180 ? heading - 360.0 : heading;
  }
}

void eula_to_quaternion_zyx(double *eula, double *q){
    double yaw = eula[0];
    double pitch = eula[1];
    double roll = eula[2];

    double cy = cos(yaw * 0.5);
    double sy = sin(yaw * 0.5);
    double cp = cos(pitch * 0.5);
    double sp = sin(pitch * 0.5);
    double cr = cos(roll * 0.5);
    double sr = sin(roll * 0.5);

    double w = cy * cp * cr + sy * sp * sr;
    double x = cy * cp * sr - sy * sp * cr;
    double y = sy * cp * sr + cy * sp * cr;
    double z = sy * cp * cr - cy * sp * sr;

    q[0] = w;
    q[1] = x;
    q[2] = y;
    q[3] = z;
}

void threeaxisrot(double r11, double r12, double r21, double r31, double r32, double *eula){
    eula[2] = atan2(r31, r32);
    eula[1] = asin(r21);
    eula[0] = atan2(r11, r12);
}

void quaternion2Eula_zyx(double *q, double* eula){
    double w = q[0];
    double x = q[1];
    double y = q[2];
    double z = q[3];
    threeaxisrot(
        2 * (x * y + w * z),
        w * w + x * x - y * y - z * z,
        -2 * (x * z - w * y),
        2 * (y * z + w * x),
        w * w - x * x - y * y + z * z, eula);
}