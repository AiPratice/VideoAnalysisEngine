#pragma once
/*
 * build_index.h
 *
 *  Created on: 2018年1月18日
 *      Author: Yunquan Cheng
 */
#include "../entities/video.h"
#include "../models/frames.h"
#include <cstdint>

namespace vrs {
namespace task{
class build_index {
private:
	static constexpr const char *FILE_EXTENSION = "sift_scfv_idx_k512";//默认索引文件后缀
public:
	/*
	 * @brief 建立所有视频的全局描述算子索引
	 */
	static void build();
	
	/*
	 * @brief 建立特定视频的全局描述算子索引
	 * @param video_id 视频数据库实体id
	 * @return 视频全局描述算子索引文件路径
	 */
	static std::string build(std::uint32_t video_id);
};

}
} /* namespace vrs */

