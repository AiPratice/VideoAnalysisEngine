#pragma once

#include <vector>
#include <cstring>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>

namespace vrs
{

namespace common
{

class feature_set {
private:
	std::vector<float*> descriptors,frames;
	uint descriptor_length,frame_length,num_features;
public:
	feature_set()
		:descriptor_length(128),frame_length(4),num_features(0){

	}
	feature_set(uint descriptor_length,uint frame_length)
		:descriptor_length(descriptor_length),frame_length(frame_length),num_features(0){

	}
	feature_set(feature_set *other);
	virtual ~feature_set();

	void add_feature(float *descriptor,float *frame);

	void print();

	/*descriptor*/
	const  std::vector<float*>& get_descriptors() const {
		return descriptors;
	}

	void set_descriptors(const  std::vector<float*>& descriptors) {
		this->descriptors = descriptors;
	}

	float *get_descriptor_at(const uint i) const {
		return descriptors[i];
	}

	/*frame*/
	const  std::vector<float*>& get_frames() const {
		return frames;
	}

	void set_frames(const  std::vector<float*>& frames) {
		this->frames = frames;
	}

	float *get_frame_at(const uint i) const {
		return frames[i];
	}

	uint get_descriptor_length() const {
		return descriptor_length;
	}

	void set_descriptor_length(uint descriptorLength) {
		descriptor_length = descriptorLength;
	}

	uint get_frame_length() const {
		return frame_length;
	}

	void set_frame_length(uint frameLength) {
		frame_length = frameLength;
	}

	uint get_num_features() const {
		return num_features;
	}

	void set_num_features(uint numFeatures) {
		num_features = numFeatures;
	}
};

}

}

