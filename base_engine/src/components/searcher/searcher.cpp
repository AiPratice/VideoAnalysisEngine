/*
 * searcher.cpp
 *	搜索器
 *  Created on: 2018年1月1日
 *      Author: Yunquan Cheng
 */

#include "searcher.h"
#include "../../config.h"
#include "../indexer/local_descriptor/sift_feat.h"
#include "../indexer/global_descriptor/definition.h"
#include "../../common/utils/io_utils.h"
#include <boost/log/trivial.hpp>
#include <chrono>
#include <cstdio>
#include <string>
#include <vector>

using namespace std;

namespace vrs {
namespace components{

searcher::~searcher() {
	if (gi) {
		delete gi;
		gi = nullptr;
	}
	if (gi_rerank) {
		delete gi_rerank;
		gi_rerank = nullptr;
	}
	if (query_index) {
		delete query_index;
		query_index = nullptr;
	}
}

void searcher::init() {
	/*if (shot_mode == SHOT_MODE_INDEP_KEYF && !keyframe_ids_for_eval.empty()) {

	} else if ((shot_mode == SHOT_MODE_SHOT_AGG
			|| shot_mode == SHOT_MODE_GLOBAL_AGG
			|| shot_mode == SHOT_MODE_TRACK_AGG)
			&& !shot_first_frames.empty()) {

	} else if (shot_mode == -1) {
		//基于视频的每一帧
		for (uint i = 0; i < db_list.size(); i++) {
			keyframe_ids_for_eval.push_back(i);
		}
	} else {
		BOOST_LOG_TRIVIAL(error)<< "未能识别的镜头模式:" << shot_mode;
		return;
	}*/

	//初始化gdindex并设置参数
	BOOST_LOG_TRIVIAL(info) << "正在初始化gdindex...";

	gi = new gdindex();
	uint ld_length,ld_frame_length;
	string ld_extension,ld_name;
	if(local_descriptor_mode == SIFT_LOCAL_DESCRIPTOR) {
		ld_length = SIFT_LENGTH;
		ld_frame_length = SIFT_FRAME_LENGTH;
		ld_extension = SIFT_EXTENSION;
		ld_name = SIFT_NAME;
	} else if(local_descriptor_mode == SIFTGEO_LOCAL_DESCRIPTOR) {
		ld_length = SIFTGEO_LENGTH;
		ld_frame_length = SIFTGEO_FRAME_LENGTH;
		ld_extension = SIFTGEO_EXTENSION;
		ld_name = SIFTGEO_NAME;
	} else {
		BOOST_LOG_TRIVIAL(error) << "无法识别的局部描述算子:" << local_descriptor_mode;
	}
	gi->set_index_parameters(ld_length, ld_frame_length, ld_extension, ld_name,
			LD_PCA_DIM, LD_PRE_PCA_POWER_DEFAULT, number_gaussians,
			GD_POWER_DEFAULT, gd_intra_normalization,
			gd_unbinarized,
			gdindex_trained_parameters_path,
			verbose_level);
	gi->set_query_parameters(min_number_words_visited, asym_scoring_mode,
			word_selection_mode, word_selection_thresh,
			score_den_power_norm,
			gdindex_trained_parameters_path,
			verbose_level);
	gi->read(gdindex_path); //加载gdindex文件
	BOOST_LOG_TRIVIAL(info) << "从" << gdindex_path << " 加载gdindex";

	if(number_scenes_to_rerank != 0) {
		gi_rerank = new gdindex();
		gi_rerank->set_index_parameters(ld_length, ld_frame_length, ld_extension, ld_name,
				LD_PCA_DIM, LD_PRE_PCA_POWER_DEFAULT, number_gaussians_rerank,
				GD_POWER_DEFAULT, gd_intra_normalization,
				gd_unbinarized,
				gdindex_trained_parameters_path,
				verbose_level);
		gi_rerank->set_query_parameters(min_number_words_visited, asym_scoring_mode_rerank,
				word_selection_mode, word_selection_thresh_rerank,
				score_den_power_norm_rerank,
				gdindex_trained_parameters_path,
				verbose_level);
		if(!gdindex_path_rerank.empty()) {
			gi_rerank->read(gdindex_path_rerank);
			BOOST_LOG_TRIVIAL(info) << "从" << gdindex_path_rerank << " 加载gdindex rerank";
		}
	}

	if(!query_index_path.empty()) {
		query_index = new gdindex();
		query_index->set_index_parameters(ld_length, ld_frame_length, ld_extension, ld_name,
				LD_PCA_DIM, LD_PRE_PCA_POWER_DEFAULT, number_gaussians,
				GD_POWER_DEFAULT, gd_intra_normalization,
				gd_unbinarized,
				gdindex_trained_parameters_path,
				verbose_level);
		query_index->set_query_parameters(min_number_words_visited, asym_scoring_mode,
				word_selection_mode, word_selection_thresh,
				score_den_power_norm,
				gdindex_trained_parameters_path,
				verbose_level);
		query_index->read(query_index_path);
		BOOST_LOG_TRIVIAL(info) << "从" << query_index_path << " 加载gdindex query";
	}

	/*if(shot_mode == -1) {
		assert(db_list.size() == gi->get_number_global_descriptors());
	}*/
}

vector<pair<float,uint>> searcher::search(const string &query_img_path){
	vector<pair<float,uint>> results_query;
	string feature_file_path = query_img_path+"."+config::get_local_descriptor_extension();
	components::sift_feat::image_to_feature(query_img_path,feature_file_path);
	gi->perform_query(feature_file_path,
			            query_index,
			            0,
			            keyframe_ids_for_eval,
			            results_query,
			            number_scenes_to_rerank,
			            gi_rerank,
			            group_lists_rerank,
			            verbose_level);
	std::remove(feature_file_path.c_str());//清理临时的特征文件
	return results_query;
}

}
}
/* namespace vrs */
