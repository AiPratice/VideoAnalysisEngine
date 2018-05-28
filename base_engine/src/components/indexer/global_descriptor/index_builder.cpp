#include "index_builder.h"
#include "definition.h"
#include "gdindex.h"
#include <boost/log/trivial.hpp>
#include <string>
#include <vector>

using namespace std;

namespace vrs
{
namespace components
{
bool index_builder::build_frame_based(const vector<string> &feature_file_paths,
                                      const char *out_file_path, uint num_gaussians, uint ld_mode, int verbose_level,
                                      const char *gdindex_parameters_path,
                                      bool gd_intra_normalization, bool gd_unbinarized)
{
	uint ld_length, ld_frame_length;
	string ld_extension, ld_name;
	switch (ld_mode) {
	case SIFT_LOCAL_DESCRIPTOR:
		//sift局部描述算子模式
		ld_length = SIFT_LENGTH;
		ld_frame_length = SIFT_FRAME_LENGTH;
		ld_extension = SIFT_EXTENSION;
		ld_name = SIFT_NAME;
		break;
	case SIFTGEO_LOCAL_DESCRIPTOR:
		//sift geo局部描述算子模式
		ld_length = SIFTGEO_LENGTH;
		ld_frame_length = SIFTGEO_FRAME_LENGTH;
		ld_extension = SIFTGEO_EXTENSION;
		ld_name = SIFTGEO_NAME;
		break;
	default:
		BOOST_LOG_TRIVIAL(error)<< "无法识别的局部描述算子:" << ld_mode;
		return false;
	}

	gdindex gi;
	gi.set_index_parameters(ld_length, ld_frame_length, ld_extension, ld_name,
	                        LD_PCA_DIM, LD_PRE_PCA_POWER_DEFAULT, num_gaussians,
	                        GD_POWER_DEFAULT, gd_intra_normalization, gd_unbinarized,
	                        gdindex_parameters_path, verbose_level);
	gi.set_query_parameters(0, ASYM_OFF, WORD_L1_NORM, 0, 0,
	                        gdindex_parameters_path, verbose_level);

	//生成数据库索引
	gi.generate_index(feature_file_paths,verbose_level);
	gi.write(out_file_path);
	return true;
}

}
} /* namespace vrs */
