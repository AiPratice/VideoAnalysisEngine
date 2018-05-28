/*
 * build_index.cpp
 *
 *  Created on: 2018年1月18日
 *      Author: Yunquan Cheng
 */

#include "build_index.h"
#include "../components/indexer/global_descriptor/index_builder.h"
#include "../config.h"
#include "../models/frames.h"

#include <string>
#include <sstream>
#include <list>
#include <sstream>

using namespace std;

namespace vrs {
namespace task{

//void build_index::build(){
//	models::frames frames_model;
//	vector<string> feature_file_paths = frames_model.get_feature_paths();
//	string out_file_path = config::get_gdindex_path();
//	BOOST_LOG_TRIVIAL(info) << "以帧为基础的索引建立任务:";
//	components::index_builder::build_frame_based(feature_file_paths, out_file_path.c_str());
//	BOOST_LOG_TRIVIAL(info) << "建立完成";
//}

string build_index::build(uint32_t video_id){
	models::frames frames_model;
	vector<string> feature_file_paths = frames_model.get_shot_feature_paths_by_video(video_id);//获取视频的镜头帧特征
	ostringstream oss;
	oss << config::get_global_descriptors_dir_path() << '/' << video_id << '.' << FILE_EXTENSION;
	string out_file_path = oss.str();
	BOOST_LOG_TRIVIAL(info) << "为视频" << video_id << "建立全局描述算子索引:" << out_file_path;
	components::index_builder::build_frame_based(feature_file_paths, out_file_path.c_str());
	BOOST_LOG_TRIVIAL(info) << "建立完成";
	return out_file_path;
}

}
} /* namespace vrs */
