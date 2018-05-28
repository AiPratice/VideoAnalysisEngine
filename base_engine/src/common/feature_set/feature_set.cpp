#include "feature_set.h"

#include <sstream>
#include <string>
#include <cstdio>
#include <boost/log/trivial.hpp>

using namespace std;

namespace vrs{
namespace common{

feature_set::feature_set(feature_set *other) {
	descriptor_length = other->descriptor_length;
	frame_length = other->frame_length;
	vector<float*> other_descriptors = other->get_descriptors(),
			other_frames = other->get_frames();
	num_features = 0;
	for(unsigned int i=0;i<other->num_features;i++){
		float *descriptor_copy = new float[descriptor_length],
				*frame_copy = new float[frame_length];
		memcpy(descriptor_copy,other_descriptors[i],sizeof(float)*descriptor_length);
		memcpy(frame_copy,other_frames[i],sizeof(float)*frame_length);
		add_feature(descriptor_copy,frame_copy);
	}
}

feature_set::~feature_set() {
	//资源释放
	for(auto iter = descriptors.begin();iter!=descriptors.end();++iter){
		float *descriptor = *iter;
		if(descriptor){
			delete []descriptor;
			descriptor = nullptr;
		}
	}
	for(auto iter = frames.begin();iter!=frames.end();++iter){
		float *frame = *iter;
		if(frame){
			delete []frame;
			frame = nullptr;
		}
	}
}

/*
 * 添加特征
 */
void feature_set::add_feature(float *descriptor,float *frame){
	descriptors.push_back(descriptor);
	frames.push_back(frame);
	num_features++;
}

/*
 * 打印输出特征集的内容
 */
void feature_set::print(){
	const int BUF_LEN = 256;
	char buf[BUF_LEN];
	ostringstream oss;
	oss << endl;
	for(unsigned int i=0;i<num_features;i++){
		if(frame_length>=4){
			snprintf(buf,BUF_LEN,"(%d) frames [%.2f %.2f %.2f %.2f], descriptors [%.2f %.2f %.2f %.2f ... %.2f]",
					i,frames[i][0],frames[i][1],frames[i][2],frames[i][3],
					descriptors[i][0],descriptors[i][1],descriptors[i][2],descriptors[i][3],
					descriptors[i][descriptor_length - 1]);
			oss << buf << endl;
		}
	}
	BOOST_LOG_TRIVIAL(debug) << oss.str();
}
		
}
}

