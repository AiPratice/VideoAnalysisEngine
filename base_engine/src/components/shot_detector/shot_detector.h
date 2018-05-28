#pragma once

#include <string>
#include <list>
#include <opencv/cv.hpp>

namespace vrs
{
namespace components
{

namespace shot_detector
{
constexpr int FRAME_WIDTH = 480;

/*
 * @brief 通过对比当前帧和前一帧去判断是否检测到了一个镜头，检测镜头返回true，否则返回false
 * @param curr_frame 当前帧矩阵
 * @param prev_frame 前一帧矩阵
 * @param threshold 阀值，默认为0.7
 * @return 如果两个图像矩阵不属于同一个镜头返回true，否则返回false
 */
bool shot_bound(const cv::Mat& curr_frame, const cv::Mat& prev_frame,
                double threshold = 0.7);
}

}
}
