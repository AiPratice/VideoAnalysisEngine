#pragma once

#include <cstdint>
#include <string>

namespace vrs
{

namespace entities
{

class video_status
{
public:
	static constexpr std::uint16_t UNPROCESS = 0, //未处理
	                               PROCESSING = 1, //处理中
	                               PROCESSED = 2; //已经处理完成
};

struct video {
	std::uint32_t video_id; //视频id
	std::string name; //视频名称
	std::string description; //视频描述
	std::string path; //视频路径
	std::string gdindex_path; //该视频的全局描述算子索引路径
	std::uint16_t status;

	video(std::uint32_t video_id=0, const std::string &name="",
	      const std::string &description="", const std::string &path="", const std::string &gdindex_path="", std::uint16_t status=video_status::UNPROCESS)
	{
		this->video_id = video_id;
		this->name = name;
		this->description = description;
		this->path = path;
		this->gdindex_path = gdindex_path;
		this->status = status;
	}
};

}

}
