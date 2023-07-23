#!/usr/bin
# -*- coding: utf-8 -*-
# @Time    : 7/22/2023 6:54 PM
# @Team    : Siemens Prescan SLS
# @Author  : Yi Yang
# @Support : prescansls.sisw@siemens.com

import numpy as np
import matplotlib.pyplot as plt

# 创建画布和坐标轴
fig, ax = plt.subplots()

# 设置初始数据
x = np.linspace(0, 2 * np.pi, 100)
y = np.sin(x)

# 绘制初始图形
line, = ax.plot(x, y, color='b')

# 设置坐标轴范围
ax.set_xlim(0, 2 * np.pi)
ax.set_ylim(-1, 1)

# 实时绘图
for i in range(100):
    # 更新数据
    y = np.sin(x + i * 0.1)

    # 更新图形
    line.set_ydata(y)

    # 暂停一小段时间，使图形实时更新
    plt.pause(0.001)

# 显示图形
# plt.show()
