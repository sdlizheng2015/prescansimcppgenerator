#pragma once

#include <iostream>
#include <vector>
#include <stddef.h>
#if _WIN64
#include <corecrt_math_defines.h>
#endif
#include <cmath>


/**
 * @brief to visualize the depth camera sensor
 * 
 * @param ID an unique name for the visualize window
 * @param D a float vector shared_ptr to depth camera data
 * @param res_x resolution X
 * @param res_y resolution Y
 */
void visualize_depth(uint32_t ID, std::shared_ptr<std::vector<float>> D, 
                     uint32_t res_x, uint32_t res_y);

/**
 * @brief 
 * 
 * @param ID 
 * @param BGRU8 
 * @param res_x 
 * @param res_y 
 */
void visualize_BRGU8(uint32_t ID, std::shared_ptr<std::vector<unsigned char>> BGRU8, 
                     uint32_t res_x, uint32_t res_y);


/**
 * @brief 
 * 
 * @param ID 
 * @param R 
 * @param G 
 * @param B 
 * @param res_x 
 * @param res_y 
 */
void visualize_SimulinkU8(uint32_t ID, 
                          std::shared_ptr<std::vector<uint8_t>> R, 
                          std::shared_ptr<std::vector<uint8_t>> G,
                          std::shared_ptr<std::vector<uint8_t>> B,
                          uint32_t res_x, uint32_t res_y);

/**
 * @brief 
 * 
 * @param ID 
 * @param Y 
 * @param Z 
 * @param res_x 
 * @param res_y 
 */
void visualize_Pcs(uint32_t ID, 
                   std::shared_ptr<std::vector<float>> Y, 
                   std::shared_ptr<std::vector<float>> Z,
                   uint32_t res_x = 500, uint32_t res_y = 1000);
