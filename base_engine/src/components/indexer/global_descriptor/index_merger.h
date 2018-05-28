#pragma once

#include <list>
#include <cstdint>
#include <string>
#include "definition.h"
#include "gdindex.h"

namespace vrs
{

namespace components
{

class index_merger
{
public:

	/*
	 * @brief索引合并
	 * @param gdindex_paths 待合并的全局描述算子索引文件
	 * @param out_gdindex_path 新生成的全局描述算子文件输出路径
	 */
	static void merge(const std::list<std::string> &gdindex_paths,
	                  const std::string &out_gdindex_path,
	                  bool gd_unbinarized = false,
	                  std::uint32_t centroids = 512,
	                  std::uint32_t num_threads = 1,
	                  std::uint32_t ld_mode = SIFT_LOCAL_DESCRIPTOR);
};

}

}
