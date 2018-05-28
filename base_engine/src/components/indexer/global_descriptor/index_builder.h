#pragma once

#include "definition.h"
#include "gdindex.h"
#include <string>
#include <vector>

namespace vrs
{
namespace components
{
class index_builder
{
public:
	/*
	 * 建立以镜头为基础的全局索引
	 * vector<string> &feature_file_paths 特征文件路径数组
	 * const char *out_file_path 索引文件输出路径
	 * uint num_gaussians
	 * uint ld_mode 局部描述算子的模式(SIFT OR SIFT_GEO)
	 * int shot_mode
	 * int verbose_level
	 * const char *gdindex_parameters_path
	 * bool gd_intra_normalization
	 * bool gd_unbinarized
	 */
	static bool build_shot_based(std::vector<std::string> &feature_file_paths,
	                             const char *out_file_path, uint num_gaussians = 512, uint ld_mode =
	                                 SIFT_LOCAL_DESCRIPTOR, int shot_mode = SHOT_MODE_INDEP_KEYF,
	                             int shot_keyf = -1, int verbose_level = 1,
	                             const char *gdindex_parameters_path = "./trained_parameters",
	                             bool single_shot = false, bool gd_intra_normalization = false,
	                             bool gd_unbinarized = false);

	/*
	 * 建立以帧为基础的全局索引
	 * vector<string> &feature_file_paths 特征文件路径数组
	 * const char *out_file_path 索引文件输出路径
	 * uint num_gaussians
	 * uint ld_mode 局部描述算子的模式(SIFT OR SIFT_GEO)
	 * int verbose_level
	 * const char *gdindex_parameters_path
	 * bool gd_intra_normalization
	 * bool gd_unbinarized
	 */
	static bool build_frame_based(const std::vector<std::string> &feature_file_paths,
	                              const char *out_file_path, uint num_gaussians = 512, uint ld_mode =
	                                  SIFT_LOCAL_DESCRIPTOR, int verbose_level = 3,
	                              const char *gdindex_parameters_path = "./trained_parameters",
	                              bool gd_intra_normalization = false, bool gd_unbinarized = false);
};

}
} /* namespace vrs */
