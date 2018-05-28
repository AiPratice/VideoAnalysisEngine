/*
 * process_video.cpp
 *
 *  Created on: 2018年1月15日
 *      Author: Yunquan Cheng
 */

#include "build_index.h"
#include "../config.h"
#include "../common/utils/io_utils.h"
#include "../components/keyframe_extractor/keyframe_extractor.h"
#include "../components/indexer/local_descriptor/sift_feat.h"
#include "../entities/video.h"
#include "process_video.h"
#include "../models/frames.h"
#include "../models/videos.h"

#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>
#include <cstdint>
#include <list>
#include <opencv/cv.hpp>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using namespace cv;

namespace vrs {
namespace task {

process_video::process_video() {
	videos_model = new models::videos();
	frames_model = new models::frames();
}

process_video::~process_video() {
	if (videos_model) {
		delete videos_model;
		videos_model = nullptr;
	}
	if (frames_model) {
		delete frames_model;
		frames_model = nullptr;
	}
}

void process_video::process(uint32_t video_id) {
	entities::video video = videos_model->get_video(video_id);
	if (video.status == entities::video_status::UNPROCESS) {
		process(video);
	}
}

void process_video::process(const entities::video &video) {
	ostringstream oss;
	//视频处理中
	videos_model->set_video_status(video.video_id,
			entities::video_status::PROCESSING);

	oss.str("");
	//视频帧图像文件夹路径
	oss << config::get_keyframes_dir_path() << "/" << video.video_id;
	string keyframes_outdir_path = oss.str();
	common::io_utils::notexist_createdir(keyframes_outdir_path);

	oss.str("");
	//视频特征文件夹路径
	oss << config::get_local_descriptors_dir_path() << "/" << video.video_id;
	string feats_outdir_path = oss.str();
	common::io_utils::notexist_createdir(feats_outdir_path);

	BOOST_LOG_TRIVIAL(info)<< "正在处理视频:" << video.path;
	BOOST_LOG_TRIVIAL(info)<< "局部描述算子输出路径" << feats_outdir_path;
	BOOST_LOG_TRIVIAL(info)<< "视频帧图像输出路径:" << keyframes_outdir_path;

	//执行分割帧
	list<string> keyframe_paths = components::keyframe_extractor::extract(
			video.path, keyframes_outdir_path);

	//是否是第一帧
	bool is_first_frame = true;
	Mat prev_frame; //前一帧的矩阵

	//分割结果进行镜头分割探测处理并存入数据库
	uint32_t count = 0;
	for (auto iter = keyframe_paths.begin(); iter != keyframe_paths.end();
			iter++) {
		entities::frame frame;
		frame.img_path = *iter;
		frame.video_id = video.video_id;
		Mat curr_frame = cv::imread(frame.img_path.c_str(),
				CV_LOAD_IMAGE_COLOR);
		if (!curr_frame.data) {
			continue; //如果数据无效，则跳过此帧图像
		}

		//视频镜头分割点探测
		if (is_first_frame) {
			frame.is_shot_frame = true; //默认置第一帧为第一个镜头
			is_first_frame = false;
			oss.str("");
			oss << feats_outdir_path << '/' << count << '.'
					<< config::get_local_descriptor_extension();
			string feature_path = oss.str();
			if (components::sift_feat::image_to_feature(frame.img_path,
					feature_path)) {
				frame.feature_path = feature_path;
			}
		} else if (components::shot_detector::shot_bound(curr_frame,
				prev_frame)) {
			frame.is_shot_frame = true; //成功检测到了一个镜头分割点

			//对镜头分割点去提取视频特征
			oss.str("");
			oss << feats_outdir_path << '/' << count << '.'
					<< config::get_local_descriptor_extension();
			string feature_path = oss.str();
			if (components::sift_feat::image_to_feature(frame.img_path,
					feature_path)) {
				frame.feature_path = feature_path;
			}
		} else {
			frame.is_shot_frame = false;
		}

		frame.second = count;

		frames_model->insert_frame(frame);
		curr_frame.copyTo(prev_frame);
		count++;
	}

	entities::video new_video = video;
	new_video.gdindex_path = build_index::build(video.video_id); //建立视频全局描述算子索引
	new_video.status = entities::video_status::PROCESSED;

	//标记视频已经处理过
	videos_model->set_video(new_video);
}

}
} /* namespace vrs */
