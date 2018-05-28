#pragma once
/*
 * process_video.h
 *	视频上传完成后执行视频处理操作
 *  Created on: 2018年1月15日
 *      Author: Yunquan Cheng
 */

#include "../config.h"
#include "../components/keyframe_extractor/keyframe_extractor.h"
#include "../components/shot_detector/shot_detector.h"
#include "../entities/video.h"
#include "../models/frames.h"
#include "../models/videos.h"

#include <cstdint>
#include <sstream>

namespace vrs {
namespace task{

class process_video {
private:
	models::videos *videos_model = nullptr;
	models::frames *frames_model = nullptr;
public:
	process_video();

	virtual ~process_video();

	/*
	 * @brief 处理视频
	 * @param video 数据库视频实体
	 */
	void process(const entities::video &video);

	/*
	 * @brief 处理视频
	 * @param video_id 视频id
	 */
	void process(std::uint32_t video_id);
};

}
} /* namespace vrs */
