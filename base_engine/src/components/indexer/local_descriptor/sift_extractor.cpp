#include "sift_extractor.h"
#include "sift_feat.h"
#include "../../../common/utils/io_utils.h"
#include <boost/log/trivial.hpp>
#include <chrono>
#include <ctime>
#include <fstream>
#include <cmath>
#include <sys/stat.h>
#include <thread>

using namespace std;
using namespace vrs::common;

namespace vrs {
namespace components{

void sift_extractor::process_images(unsigned int start_i,
		unsigned int num_images, const vector<string> &images,
		const char *in_folder_path, const char *out_folder_path,
		unsigned int thread_index, bool is_skip_existing) {
	unsigned int count_proc_thread = 0;
	unsigned int end_i = start_i + num_images;
	for (unsigned int i = start_i; i < end_i; i++) {
		string img_filename = images[i];
		if ((count_proc_thread++ % PROGRESS_INTERVAL) == 0) {
			BOOST_LOG_TRIVIAL(info)<< "线程" << thread_index << "正在处理第"
			<< i << "张图片。图片范围:" << start_i << "~" << end_i;
		}

		//生成新的文件名
		int last_dot_index = img_filename.find_last_of('.');
		string out_filename = img_filename.substr(0, last_dot_index) + ".siftb";
		string out_file_path = string(out_folder_path) + "/" + out_filename;

		//检查文件是否已经存在
		if (is_skip_existing
				&& io_utils::is_file_exist(out_file_path)) {
			continue;
		}

		string image_path = string(in_folder_path) + "/" + img_filename;
		if(!sift_feat::image_to_feature(image_path.c_str(),out_file_path.c_str())){
			BOOST_LOG_TRIVIAL(error) <<  "从图像文件生成特征文件";
		}
	}
}

void sift_extractor::extract(const char *in_folder_path,
		const char *out_folder_path, unsigned int num_least_threads,
		bool is_skip_existing) {
	if (!in_folder_path) {
		BOOST_LOG_TRIVIAL(error)<< "sift_extractor:图像输入文件夹为NULL";
		return;
	}
	vector<string> images;
	io_utils::get_dir_files(in_folder_path,".jpg",images);

	unsigned int num_images = images.size();

	//每个线程需要处理的图像数量
	unsigned int computations_per_thread = static_cast<unsigned int>(floor(num_images/num_least_threads));
	unsigned int rest_computations = num_images % num_least_threads;
	if(rest_computations > 0) {
		num_least_threads++; //如果剩余处理数量不为0，那么多分配一个线程去完成剩余任务。
	}
	thread extract_threads[num_least_threads];
	unsigned int start_i = 0,i=0;

	if(rest_computations == 0) {
		for(;i<num_least_threads;i++) {
			extract_threads[i] = thread(process_images,start_i,computations_per_thread,images,in_folder_path,out_folder_path,i,is_skip_existing);
			start_i += computations_per_thread;
		}
	} else {
		for(;i<num_least_threads-1;i++) {
			extract_threads[i] = thread(process_images,start_i,computations_per_thread,images,in_folder_path,out_folder_path,i,is_skip_existing);
			start_i += computations_per_thread;
		}
		extract_threads[i] = thread(process_images,start_i,rest_computations,images,in_folder_path,out_folder_path,i,is_skip_existing);
	}

	for(i=0;i<num_least_threads;i++){
		extract_threads[i].join();
	}
}

}
}