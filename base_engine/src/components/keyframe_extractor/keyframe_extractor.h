#pragma once

#include <cstdint>
#include <iostream>
#include <list>
#include <string>
#include <vector>

namespace vrs
{
namespace components
{
namespace keyframe_extractor
{

constexpr std::uint32_t FRAME_MAX_HEIGHT = 480;//视频帧最大高度

/*
 * 执行分解
 * @param video_path 视频路径
 * @param out_folder 输出视频帧的文件夹
 * @param filters ffmpeg 过滤器
 * @param rate 率
 */
//std::vector<std::string> extract(const std::string &video_path,
//                                 const std::string &out_folder, const std::string &filters =
//                                     "scale=-1:480", int rate = 1);

/*
 * @brief 执行分解
 * @param video_path 视频路径
 * @param out_folder 输出视频帧的文件夹
 */
std::list<std::string> extract(const std::string &video_path,const std::string &out_dir_path);

}
}
}
