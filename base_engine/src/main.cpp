#include "config.h"
#include "rpc/vrs_server.h"

#include "task/process_video.h"
#include "common/utils/io_utils.h"

#include <boost/log/trivial.hpp>
#include <cstdio>
#include <vector>

//test
#include "test/test_keyframe_extract.h"

using namespace std;
using namespace vrs;

void test_process_video(){
	BOOST_LOG_TRIVIAL(debug) << "视频处理测试";
	task::process_video process_video;
	process_video.process(1);
}

int main(int argc, char **argv)
{
	config::init("config.yaml");
	
	//测试
//	test_process_video();
	
	//rpc服务器
	rpc::vrs_server vrs_server(config::get_rpc_server_address());
	vrs_server.run();
}
