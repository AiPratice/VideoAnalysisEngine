#include "../../../config.h"
#include "definition.h"
#include "gdindex.h"
#include "index_merger.h"

#include <boost/log/trivial.hpp>
#include <string>
#include <list>

using namespace std;

namespace vrs
{
namespace components
{

void index_merger::merge(const list<string> &gdindex_paths,
                        const string &out_gdindex_path,
                        bool gd_unbinarized,
                        uint32_t centroids,
                        uint32_t num_threads,
                        uint32_t ld_mode )
{
	const string gdindex_parameters_path = config::get_gdindex_parameters_path();
	uint32_t ld_length;
	uint32_t ld_frame_length;
	const char *ld_extension;
	const char *ld_name;
	switch(ld_mode) {
	case SIFT_LOCAL_DESCRIPTOR:
		ld_length = SIFT_LENGTH;
		ld_frame_length = SIFT_FRAME_LENGTH;
		ld_extension = SIFT_EXTENSION;
		ld_name = SIFT_NAME;
		break;
	case SIFTGEO_LOCAL_DESCRIPTOR:
		ld_length = SIFTGEO_LENGTH;
		ld_frame_length = SIFTGEO_FRAME_LENGTH;
		ld_extension = SIFTGEO_EXTENSION;
		ld_name = SIFTGEO_NAME;
		break;
	default:
		BOOST_LOG_TRIVIAL(error) << "无法合并索引。原因是未定义的局部描述算子模式:" << ld_mode;
		return;
	}

	if(num_threads == 1) {
		components::gdindex gdindex;
		gdindex.set_index_parameters(ld_length,
		                             ld_frame_length,
		                             ld_extension,
		                             ld_name,
		                             LD_PCA_DIM,
									 LD_PRE_PCA_POWER_DEFAULT,
		                             GD_NUMBER_GAUSSIANS_DEFAULT,
		                             GD_POWER_DEFAULT,
		                             GD_INTRA_NORMALIZATION_DEFAULT,
		                             gd_unbinarized,
		                             gdindex_parameters_path,
		                             3);
		gdindex.set_query_parameters(0, 0, 0, 0, 0, gdindex_parameters_path,3);
		
		//合并所有索引
		for(const string &path : gdindex_paths){
			gdindex.read(path,true);
		}
		
		gdindex.write(out_gdindex_path);
	}else{
		//多线程处理 to do
	}
}

}
}
