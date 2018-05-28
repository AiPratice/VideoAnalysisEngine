#include "keyframe_extractor.h"

#include <iostream>
#include <cstdio>
#include <boost/log/trivial.hpp>
#include <list>
#include <opencv/cv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <string>
#include <sstream>
#include "../../common/utils/io_utils.h"

using namespace std;
using namespace vrs::common;

namespace vrs {
namespace components {
namespace keyframe_extractor {

list<string> extract(const string &video_path, const string &out_dir_path) {
	list<string> paths;
	cv::VideoCapture cap;
	ostringstream oss;
	vector<int> compression_params;
	compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
	compression_params.push_back(90);
	if (cap.open(video_path) && cap.isOpened()) {
		cv::Mat frame;
		int count = 0;
		while (cap.read(frame)) {
			cap.set(cv::CAP_PROP_POS_MSEC, count * 1000);
			oss.str("");
			oss << out_dir_path << '/' << count << ".jpg";
			string path = oss.str();
			cap >> frame;
			if (frame.data && cv::imwrite(path, frame, compression_params)) {
				paths.push_back(path);
				BOOST_LOG_TRIVIAL(debug) << path << "生成帧" << count;
			} else {
				BOOST_LOG_TRIVIAL(error)<< "视频切割帧存储失败 num:" << count << " video_path:" << video_path << " img_path:" << path;
			}
			count++;
		}
	} else {
		BOOST_LOG_TRIVIAL(error)<< "被切割视频打开失败:" << video_path;
	}
	return paths;
}

}
}
}
