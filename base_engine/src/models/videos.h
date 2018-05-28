#pragma once


#include "../models/base_model.h"
#include "../entities/video.h"
#include <vector>
#include <string>
#include <list>
#include <cstdint>

namespace vrs
{
namespace models
{
class videos: public base_model
{
public:
	videos();
	virtual ~videos();

	/**
	 * 获取数据库中所有视频的路径
	 */
	std::vector<std::string> get_all_video_paths();

	/*
	 * @brief 获取所有视频的全局描述算子索引路径
	 * @return 返回一个路径字符串list
	 */
	std::list<std::string> get_all_gdindex_paths();

	/*
	 * 获取所有的视频
	 */
	std::list<entities::video> get_all_videos();

	/*
	 * 获取所有未处理的视频
	 */
	std::list<entities::video> get_unprocess_videos();

	/*
	 * @brief 获取待处理的视频队列
	 */
	std::list<entities::video> get_pending_videos();

	/*
	 * @brief 插入视频id到视频待处理队列
	 * @param video_id 视频id
	 * @return 插入成功返回true，否则返回false
	 */
	bool insert_pending_video(std::uint32_t video_id);

	/*
	 * 设置视频状态
	 */
	bool set_video_status(std::uint32_t video_id, std::uint16_t status);

	/*
	 * @brief 获取视频
	 * @param video_id 数据库视频id
	 * @return 返回视频数据库实体
	 */
	entities::video get_video(std::uint32_t video_id);

	/*
	 * @brief 更新数据库视频信息
	 * @param video 视频实体
	 * @return 设置成果返回true，否则返回false
	 */
	bool set_video(const entities::video &video);
};
}
} /* namespace vrs */
