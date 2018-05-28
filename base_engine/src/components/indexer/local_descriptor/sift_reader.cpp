#include "sift_reader.h"
#include "../../../common/feature_set/feature_set.h"
#include <boost/log/trivial.hpp>
#include <cstdio>
#include <sys/types.h>
#include <fstream>

using namespace std;
using namespace vrs::common;

namespace vrs
{
namespace components
{
bool sift_reader::read_sift_file(const char *path, const uint &frame_length,
                                 const uint &desc_length, feature_set &feat_set)
{
	if(!path) {
		BOOST_LOG_TRIVIAL(error) << "读取sift文件的路径不能为空";
		return false;
	}
	FILE *fp = fopen(path,"rb");
	if(!fp) {
		BOOST_LOG_TRIVIAL(error) << "无法读取sift文件";
		return false;
	}
	uint num_features = 0;//从特征文件得到的特征数量
	fread(&num_features,sizeof(uint),1,fp);
	num_features = min(MAX_NUM_FEATURES,num_features);

	//读取关键点和描述算子
	for(uint i=0; i<num_features; i++) {
		float *frame = new float[frame_length];
		float *descriptor = new float[desc_length];
		float fL2Norm = 0;
		fread(frame,sizeof(float),frame_length,fp);
		fread(descriptor,sizeof(float),desc_length,fp);
		for(uint j=0; j<desc_length; j++) {
			descriptor[j] /= DESC_DIVISOR;
			fL2Norm += descriptor[j]*descriptor[j];
		}
		if(isnan(fL2Norm)) {
			//如果是一个非数值，那么就放弃此段内容
			delete[] frame;
			delete[] descriptor;
			frame = nullptr;
			descriptor = nullptr;
			continue;
		}
		feat_set.add_feature(descriptor,frame);
	}
	fclose(fp);
	return true;
}

bool sift_reader::read_sift_geo_file(const char *path,const uint &frame_length,const uint &desc_length,feature_set &feat_set)
{
	if(!path) {
		BOOST_LOG_TRIVIAL(error) << path << " 读取sift geo文件的路径不能为空";
		return false;
	}
	FILE *fp = fopen(path,"rb");
	if(!fp) {
		BOOST_LOG_TRIVIAL(error) << path << " 无法读取sift geo文件";
		return false;
	}
	uint num_features = 0;
	fseek(fp,0,SEEK_END);
	long int size = ftell(fp);//获取文件大小
	fseek(fp,0,SEEK_SET);
	num_features = size/SIFTGEO_DESC_SIZE;
	num_features = std::min(num_features,MAX_NUM_FEATURES);
	for(uint i=0; i<num_features; i++) {
		int desc_len_in_file = 0;
		uint8_t u_desc[desc_length];
		float *frame = new float[frame_length];
		float *descriptor = new float[desc_length];
		fread(frame,sizeof(float),frame_length,fp);
		fread(&desc_len_in_file,sizeof(int),1,fp);
		fread(u_desc,sizeof(uint8_t),desc_length,fp);
		if((uint8_t)desc_len_in_file != desc_length) {
			BOOST_LOG_TRIVIAL(error) << path << " sift geo文件格式错误";
			return false;
		}
		float fL2Norm = 0;
		for(uint j=0; j<desc_length; j++) {
			descriptor[j] = descriptor[j] / DESC_DIVISOR;
			fL2Norm += descriptor[j]*descriptor[j];
		}
		if(isnan(fL2Norm)) {
			delete[] frame;
			delete[] descriptor;
			frame = nullptr;
			descriptor = nullptr;
			continue;
		}
		feat_set.add_feature(descriptor,frame);
	}
	fclose(fp);
	return true;
}

}
}
