/*
 * vrs_server.cpp
 *
 *  Created on: 2018年1月19日
 *      Author: Yunquan Cheng
 */

#include "../rpc/vrs_server.h"

#include <boost/log/trivial.hpp>
#include <string>
#include <memory>

#include "../rpc/VrsRpcServiceImpl.h"

using namespace std;

namespace vrs {
namespace rpc{

vrs_server::vrs_server(const string &server_address) {
	this->server_address = server_address;
}

vrs_server::~vrs_server() {

}

void vrs_server::run(){
	VrsRpcServiceImpl service;
	grpc::ServerBuilder builder;
	builder.AddListeningPort(server_address,grpc::InsecureServerCredentials());
	builder.RegisterService(&service);

	unique_ptr<grpc::Server> server(builder.BuildAndStart());

	BOOST_LOG_TRIVIAL(info) << "服务器已开启:" << server_address;
	server->Wait();
}

}
} /* namespace vrs */
