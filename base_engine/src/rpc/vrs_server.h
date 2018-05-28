/*
 * vrs_server.h
 *	视频检索系统rpc服务器
 *  Created on: 2018年1月19日
 *      Author: Yunquan Cheng
 */
#pragma once

#include <string>

namespace vrs
{
namespace rpc
{

class vrs_server
{
private:
	std::string server_address;

public:
	vrs_server(const std::string &server_address);
	virtual ~vrs_server();

	void run();//运行服务器
};

}
} /* namespace vrs */
