/*
 * VrsRpcServiceImpl.h
 *
 *  Created on: 2018年1月19日
 *      Author: Yunquan Cheng
 */
#pragma once

#include <memory>
#include <string>
#include <vector>

#include <grpc++/grpc++.h>

#include "../components/searcher/searcher.h"
#include "../rpc/vrs.grpc.pb.h"
#include "../rpc/vrs.pb.h"
#include "../task/process_video.h"
#include "../task/merge_index.h"

namespace vrs
{
namespace rpc
{

class VrsRpcServiceImpl final:public VrsRpc::Service
{
private:
	std::string gdindex_path;
	components::searcher *searcher = nullptr; //视频搜索
	
	task::process_video *process_video = nullptr; //视频处理
	task::merge_index *merge_index = nullptr;
	
	std::vector<std::uint64_t> frame_ids;//视频帧id向量（从数据库中获得）

	/*
	 * @brief 初始化
	 */
	void init();

public:
	VrsRpcServiceImpl();
	virtual ~VrsRpcServiceImpl();

	/*
	 * @biref 根据图像路径进行搜索，并返回评分结果
	 * @param context grpc服务端上下文
	 * @param searchRequest 客户端发送的搜索请求
	 * @param writer 返回给客户端的搜索结果流
	 * @return grpc状态
	 */
	grpc::Status Search(grpc::ServerContext *context,
	                    const SearchRequest *request,
	                    grpc::ServerWriter<SearchResult> *writer) override;

	/*
	 * @brief 视频处理
	 * @param context grpc服务端上下文
	 * @param request 视频处理请求
	 * @param response 视频处理结果响应
	 * @return grpc状态
	 */
	grpc::Status ProcessVideo(grpc::ServerContext* context,
	                          const ProcessVideoRequest* request,
	                          Result* response) override;

	/*
	 * @brief 合并各个视频的索引为全局索引
	 * @param context grpc服务端上下文
	 * @param request 合并处理空请求
	 * @param response 合并处理结果响应
	 * @return grpc状态
	 */
	grpc::Status Merge(grpc::ServerContext* context,
	                   const Empty* request,
	                   Result* response) override;
};

}
} /* namespace vrs */
