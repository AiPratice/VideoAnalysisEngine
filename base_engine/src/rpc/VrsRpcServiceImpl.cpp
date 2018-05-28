/*
 * VrsRpcServiceImpl.cpp
 *
 *  Created on: 2018年1月19日
 *      Author: Yunquan Cheng
 */

#include "../config.h"
#include "../common/utils/io_utils.h"
#include "../models/frames.h"
#include "../rpc/VrsRpcServiceImpl.h"
#include "../rpc/vrs.grpc.pb.h"
#include "../rpc/vrs.pb.h"
#include "../components/searcher/searcher.h"
#include "../task/process_video.h"
#include "../task/merge_index.h"
#include <boost/log/trivial.hpp>
#include <google/protobuf/stubs/common.h>
#include <string>
#include <vector>

using namespace std;

namespace vrs {
namespace rpc {

VrsRpcServiceImpl::VrsRpcServiceImpl() {
	gdindex_path = config::get_gdindex_path();
	if (common::io_utils::is_file_exist(gdindex_path)) {
		//如果全局描述算子索引文件已经存在，则进行初始化操作
		init();
	} else {
		BOOST_LOG_TRIVIAL(info)<< "找不到索引文件:" << gdindex_path;
	}

	process_video = new task::process_video();
	merge_index = new task::merge_index();
}

VrsRpcServiceImpl::~VrsRpcServiceImpl() {
	if (searcher) {
		delete searcher;
		searcher = nullptr;
	}
	if (process_video) {
		delete process_video;
		process_video = nullptr;
	}
	if (merge_index) {
		delete merge_index;
		merge_index = nullptr;
	}
}

void VrsRpcServiceImpl::init() {
	models::frames frames_model;
	frame_ids = frames_model.get_shot_frame_ids(); //获取所有的帧id
	vector<uint> keyframe_ids_for_eval; //帧id映射（用于全局描述算子查询）
	for (decltype(frame_ids.size()) i = 0; i < frame_ids.size(); i++) {
		keyframe_ids_for_eval.push_back(i);
	}
	searcher = new components::searcher(gdindex_path, keyframe_ids_for_eval);
	searcher->set_word_selection_mode(0);
	searcher->set_word_selection_thresh(10);
	searcher->set_local_descriptor_mode(0);
	searcher->set_word_selection_thresh_rerank(8);
	searcher->init();
}

grpc::Status VrsRpcServiceImpl::Search(grpc::ServerContext *context,
		const SearchRequest *request,
		grpc::ServerWriter<SearchResult> *writer) {
	if (!searcher) {
		if (common::io_utils::is_file_exist(gdindex_path)) {
			init();
		} else {
			return grpc::Status::CANCELLED;
		}
	}

	//如果全局描述算子已经初始化完成
	const string img_path = request->imgpath();
	BOOST_LOG_TRIVIAL(debug)<< "rpc search:" << img_path;
	vector<pair<float, uint>> results = searcher->search(img_path);
	std::size_t num_results = results.size() > 100 ? 100 : results.size();
	for (std::size_t i = 0; i < num_results; i++) {
		SearchResult searchResult;
		ulong frame_id = frame_ids[results[i].second];
		searchResult.set_score(results[i].first);
		searchResult.set_frameid(
				static_cast<::google::protobuf::uint64>(frame_id));
		writer->Write(searchResult);
	}
	return grpc::Status::OK;
}

grpc::Status VrsRpcServiceImpl::ProcessVideo(grpc::ServerContext* context,
		const ProcessVideoRequest* request, Result* response) {
	process_video->process(request->videoid());
	response->set_code(200);
	return grpc::Status::OK;
}

grpc::Status VrsRpcServiceImpl::Merge(grpc::ServerContext *context,
		const Empty *request, Result *response) {
	merge_index->merge();
	response->set_code(200);
	gdindex_path = config::get_gdindex_path();
	if (common::io_utils::is_file_exist(gdindex_path)) {
		if (searcher) {
				delete searcher;
				searcher = nullptr;
		}
		init(); //重新初始化
	}
	return grpc::Status::OK;
}

}
} /* namespace vrs */
