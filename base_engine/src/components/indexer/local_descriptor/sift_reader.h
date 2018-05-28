#pragma once

#include "../../../common/feature_set/feature_set.h"
#include <sys/types.h>

namespace vrs {
namespace components{
constexpr uint MAX_NUM_FEATURES = 5000;
constexpr float DESC_DIVISOR = 512.0;
constexpr int SIFTGEO_DESC_SIZE = 168;

class sift_reader {
public:
	static bool read_sift_file(const char *path, const uint &frame_length,
		 	const uint &desc_length, common::feature_set &feat_set);
	static bool read_sift_geo_file(const char *path, const uint &frame_length,
			const uint &desc_length, common::feature_set &feat_set);
};

}
} /* namespace vrs */