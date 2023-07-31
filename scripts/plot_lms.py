#!/usr/bin
# -*- coding: utf-8 -*-
# @Time    : 7/19/2023 4:20 PM
# @Team    : Siemens Prescan SLS
# @Author  : Yi Yang
# @Support : prescansls.sisw@siemens.com

"""
// ouput lms data
    ps_Audi_A3_1.step(simulation, this);
    int i = 0;
    for (const auto & line : *ps_Audi_A3_1.lmsUnit_Audi_A3_1_LMSSensor_1_Lines){
      std::cout << "Line" + std::to_string(i) << "= [";
      for (const auto & point : line){
        std::cout << "{'x': " << point.X << ", "
                  << "'y': " << point.Y << ", "
                  << "'z': " << point.Z << ", "
                  << "'curve': " << point.Curvature << "}, ";
      }
      std::cout << "]" << std::endl;
      i ++;
    }
"""

import numpy as np
import matplotlib.pyplot as plt


Line0= [{'x': 4.133, 'y': 1.763, 'z': -1.320, 'curve': 0.000}, {'x': 9.133, 'y': 1.763, 'z': -1.320, 'curve': 0.000}, {'x': 14.133, 'y': 1.763, 'z': -1.320, 'curve': 0.000}, {'x': 19.133, 'y': 1.763, 'z': -1.320, 'curve': 0.000}, {'x': 24.133, 'y': 1.763, 'z': -1.320, 'curve': 0.000}, {'x': 29.133, 'y': 1.763, 'z': -1.320, 'curve': 0.000}, {'x': 34.133, 'y': 1.763, 'z': -1.320, 'curve': 0.000}, {'x': 39.133, 'y': 1.763, 'z': -1.320, 'curve': 0.000}, {'x': 44.133, 'y': 1.763, 'z': -1.320, 'curve': 0.000}, {'x': 49.133, 'y': 1.763, 'z': -1.320, 'curve': 0.000}, ]
Line1= [{'x': 4.073, 'y': -1.737, 'z': -1.320, 'curve': -0.000}, {'x': 9.073, 'y': -1.737, 'z': -1.320, 'curve': -0.000}, {'x': 14.073, 'y': -1.737, 'z': -1.320, 'curve': 0.000}, {'x': 19.073, 'y': -1.737, 'z': -1.320, 'curve': 0.000}, {'x': 24.073, 'y': -1.737, 'z': -1.320, 'curve': 0.000}, {'x': 29.073, 'y': -1.737, 'z': -1.320, 'curve': 0.000}, {'x': 34.073, 'y': -1.737, 'z': -1.320, 'curve': 0.000}, {'x': 39.073, 'y': -1.737, 'z': -1.320, 'curve': 0.000}, {'x': 44.073, 'y': -1.737, 'z': -1.320, 'curve': 0.000}, {'x': 49.073, 'y': -1.737, 'z': -1.320, 'curve': 0.000}, ]
Line2= [{'x': 12.338, 'y': 5.263, 'z': -1.320, 'curve': 0.000}, {'x': 17.338, 'y': 5.263, 'z': -1.320, 'curve': 0.000}, {'x': 22.338, 'y': 5.263, 'z': -1.320, 'curve': 0.000}, {'x': 27.338, 'y': 5.263, 'z': -1.320, 'curve': 0.000}, {'x': 32.338, 'y': 5.263, 'z': -1.320, 'curve': 0.000}, {'x': 37.338, 'y': 5.263, 'z': -1.320, 'curve': 0.000}, {'x': 42.338, 'y': 5.263, 'z': -1.320, 'curve': 0.000}, {'x': 47.338, 'y': 5.263, 'z': -1.320, 'curve': 0.000}, ]

Line0_X = [point['x'] for point in Line0]
Line0_Y = [point['y'] for point in Line0]

for i in range(3):
    exec(f"Line{i}_X = [point['x'] for point in Line{i}]")
    exec(f"Line{i}_Y = [point['y'] for point in Line{i}]")
    exec(f"plt.scatter(Line{i}_X, Line{i}_Y)")
plt.grid()
plt.axis('equal')
plt.show()
