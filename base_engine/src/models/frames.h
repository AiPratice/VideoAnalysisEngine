#pragma once

#include <vector>
#include <string>
#include "base_model.h"
#include "../entities/frame.h"

namespace vrs
{

namespace models
{

class frames: public models::base_model
{
private:
	char *sql_buf = nullptr; //sql语句缓存
public:
	frames();
	virtual ~frames();

	/*
	 * 插入视频帧信息到数据库中
	 * @param frame 帧结构体
	 */
	bool insert_frame(const entities::frame &frame);

	/**
	 * 获取所有帧
	 */
	std::vector<entities::frame> get_all_frames();

	/*
	 * 清理所有帧的数据库记录（危险操作，谨慎调用）
	 */
	void truncate();

	/*
	 * @brief 获取所有的特征文件路径
	 * @return 返回路径字符串向量
	 */
	std::vector<std::string> get_feature_paths();

	/*
	 * @brief 获取视频的所有特征路径
	 * @param video_id 视频id
	 * @return 返回路径向量
	 */
	std::vector<std::string> get_feature_paths_by_video(std::uint32_t video_id);

	/*
	 * @brief 获取视频的所有特征路径
	 * @param video_id 视频id
	 * @return 返回路径向量
	 */
	std::vector<std::string> get_shot_feature_paths_by_video(std::uint32_t video_id);

	/*
	 * @brief 获取所有帧图像路径
	 * @return 返回图像帧路径字符串向量
	 */
	std::vector<std::string> get_img_paths();

	/*
	 * @brief 获取所有帧图像的id
	 * @return 返回帧图像id向量
	 */
	std::vector<std::uint64_t> get_frame_ids();

	/*
	 * @brief 获取所有镜头帧图像的id
	 * @return 返回帧图像id向量
	 */
	std::vector<std::uint64_t> get_shot_frame_ids();
};

}

}
