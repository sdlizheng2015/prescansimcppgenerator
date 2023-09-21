
#include "visualizationtools.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "Eigen/Dense"
#include <vector>


void visualize_depth(uint32_t ID, std::shared_ptr<std::vector<float>> D, 
                     uint32_t res_x, uint32_t res_y){
    if(D != nullptr){
        Eigen::Map<Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>> normal_image(D->data(), res_x, res_y);
        if (normal_image.maxCoeff() != normal_image.minCoeff()) {
            normal_image = (normal_image - normal_image.minCoeff() * Eigen::MatrixXf::Ones(res_x, res_y))
            / (normal_image.maxCoeff() - normal_image.minCoeff());
        }
        else {
            normal_image = Eigen::MatrixXf::Zero(res_x, res_y);
        }
        cv::Mat image(res_x, res_y, CV_32FC1, normal_image.data());
        cv::Mat rotated_image, flipped_image;
        cv::rotate(image, rotated_image, cv::ROTATE_90_CLOCKWISE);
        cv::flip(rotated_image, flipped_image, 1);
        cv::imshow(std::to_string(ID), flipped_image);
        cv::waitKey(1);
    }
}

void visualize_BRGU8(uint32_t ID, std::shared_ptr<std::vector<unsigned char>> BGRU8, 
                     uint32_t res_x, uint32_t res_y){
    if(BGRU8 != nullptr){
        cv::Mat image(res_y, res_x, CV_8UC3, BGRU8->data());
        cv::imshow(std::to_string(ID), image);
        cv::waitKey(1);
    }
}


void visualize_SimulinkU8(uint32_t ID, 
                          std::shared_ptr<std::vector<uint8_t>> R, 
                          std::shared_ptr<std::vector<uint8_t>> G,
                          std::shared_ptr<std::vector<uint8_t>> B,
                          uint32_t res_x, uint32_t res_y){
    if (R != nullptr && G != nullptr && B != nullptr){
        std::vector<uint8_t> rMsg;
        rMsg.reserve(res_x * res_y * 3);
        for (int i = 0; i < R->size(); i++) {
            //cv by default is BGR format
            rMsg.push_back((*B)[i]);
            rMsg.push_back((*G)[i]);
            rMsg.push_back((*R)[i]);
        }
        cv::Mat cv_image(res_x, res_y, CV_8UC3, rMsg.data());
        cv::Mat rotated_image, flipped_image;
        cv::rotate(cv_image, rotated_image, cv::ROTATE_90_CLOCKWISE);
        cv::flip(rotated_image, flipped_image, 1);
        cv::imshow(std::to_string(ID), flipped_image);
        cv::waitKey(1);
    }
}


void visualize_Pcs(uint32_t ID, 
                   std::shared_ptr<std::vector<float>> Y, 
                   std::shared_ptr<std::vector<float>> Z,
                   uint32_t res_x, uint32_t res_y){
	if (Z != nullptr && Y != nullptr){
		cv::Mat image = cv::Mat::zeros(res_x, res_y, CV_8UC3);
		int centerY = image.rows / 2;
		int centerX = image.cols / 2;
		for (int i=0; i< Y->size(); i++){
		  cv::circle(image, cv::Point(centerX - static_cast<int>((*Y)[i] * 100), centerY - static_cast<int>((*Z)[i]*100)), 
                     2, cv::Scalar(0, 125, 125), 0);
		}
		cv::imshow(std::to_string(ID), image);
		cv::waitKey(1);
	}
}