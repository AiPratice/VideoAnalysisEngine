/*
 * test_keyframe_extract.cpp
 *
 *  Created on: 2018年2月3日
 *      Author: Yunquan Cheng
 */

#include "test_keyframe_extract.h"
#include "../components/keyframe_extractor/keyframe_extractor.h"

#include <boost/log/trivial.hpp>
#include <list>
#include <string>

using namespace std;
using namespace vrs;

void test_keyframe_extract() {
	list<string> paths =
			components::keyframe_extractor::extract(
					"/home/pkdog/vrs_storage/videos/ac59847b0c16ae15e6b1b0e4c210ad97.mp4",
					"/home/pkdog/vrs_storage/keyframes/1");
	for(const string &path : paths){
		BOOST_LOG_TRIVIAL(info) << path;
	}
}
