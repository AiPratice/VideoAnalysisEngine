#pragma once

#include <sstream>
#include <string>
#include <cstdint>

namespace vrs
{
namespace entities
{

/**
 * 镜头帧结构体
 */
struct frame {
	std::uint64_t frame_id;
	std::uint32_t video_id;
	std::string img_path;//帧图片路径
	std::string feature_path;//特征文件路径
	bool is_shot_frame;//是否为镜头分割点
	std::uint32_t second;//秒

	std::string to_string();
};

}
}
