#include "../components/indexer/global_descriptor/index_merger.h"
#include "../config.h"
#include "merge_index.h"
#include "../models/videos.h"

#include <boost/log/trivial.hpp>
#include <list>
#include <string>

using namespace std;

namespace vrs
{
namespace task
{
	
void merge_index::merge(){
	const list<string> gdindex_paths = videos_model.get_all_gdindex_paths();
	const string out_gdindex_path = config::get_global_descriptors_dir_path()+string("/")+config::get_gdindex_filename();
	BOOST_LOG_TRIVIAL(info) << "合并" << gdindex_paths.size() << "个全局描述算子索引为:" << out_gdindex_path;
	components::index_merger::merge(gdindex_paths,out_gdindex_path);
	BOOST_LOG_TRIVIAL(info) << "合并完成";
}
	
}
}
